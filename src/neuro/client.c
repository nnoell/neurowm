//----------------------------------------------------------------------------------------------------------------------
// Module      :  client
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "client.h"
#include "geometry.h"
#include "system.h"
#include "config.h"
#include "core.h"
#include "layout.h"
#include "rule.h"
#include "workspace.h"
#include "event.h"

//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// XMotionUpdaterFn
typedef void (*XMotionUpdaterFn)(NeuroRectangle *r, const NeuroRectangle *c, int ex, int ey, const NeuroPoint *p);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static bool is_protocol_delete(Window w) {
  Atom *protocols = NULL;
  bool ret = false;
  int n = 0;
  if (XGetWMProtocols(NeuroSystemGetDisplay(), w, &protocols, &n)) {
    for (int i = 0; !ret && i < n; i++)
      if (protocols[ i ] == NeuroSystemGetWmAtom(NEURO_SYSTEM_WMATOM_DELETEWINDOW))
        ret = true;
    XFree(protocols);
  }
  return ret;
}

static bool set_title_atom(NeuroClient *c, Atom atom) {
  assert(c);
  XTextProperty tp;
  XGetTextProperty(NeuroSystemGetDisplay(), c->win, &tp, atom);
  if (!tp.nitems)
    return false;
  if (tp.encoding == XA_STRING) {
    strncpy(c->title, (char *)tp.value, NEURO_NAME_SIZE_MAX);
  } else {
    char **list = NULL;
    int n = 0;
    if (XmbTextPropertyToTextList(NeuroSystemGetDisplay(), &tp, &list, &n) >= Success && n > 0 && list[ 0 ]) {
      strncpy(c->title, list[ 0 ], NEURO_NAME_SIZE_MAX);
      XFreeStringList(list);
    }
  }
  XFree(tp.value);
  return true;
}

static void process_xmotion(NeuroRectangle *r, NeuroIndex ws, const NeuroRectangle *c, const NeuroPoint *p,
    XMotionUpdaterFn xmuf, Cursor cursor) {
  assert(r);
  assert(c);
  assert(p);

  // Grab the pointer and set a cursor
  if (GrabSuccess != XGrabPointer(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), false,
      ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime))
    return;

  // process until the button is released
  XEvent ev = { 0 };
  do {
    XMaskEvent(NeuroSystemGetDisplay(), ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      xmuf(r, c, ev.xmotion.x, ev.xmotion.y, p);
      NeuroLayoutRunCurr(ws);
      NeuroWorkspaceUpdate(ws);
    }
  } while (ev.type != ButtonRelease);

  // Ungrab the pointer
  XUngrabPointer(NeuroSystemGetDisplay(), CurrentTime);
}

static void xmotion_move(NeuroRectangle *r, const NeuroRectangle *c, int ex, int ey, const NeuroPoint *p) {
  r->p.x = c->p.x + (ex - p->x);
  r->p.y = c->p.y + (ey - p->y);
}

static void xmotion_resize(NeuroRectangle *r, const NeuroRectangle *c, int ex, int ey, const NeuroPoint *p) {
  (void)p;
  r->w = c->w + (ex - (c->w + r->p.x));
  r->h = c->h + (ey - (c->h + r->p.y));
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
void NeuroClientUpdate(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Get workspace and regions
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  const NeuroRectangle *const stack_region = NeuroCoreStackGetRegion(ws);
  NeuroRectangle *const client_region = NeuroCoreClientGetRegion(c);

  // Priority: Fullscreen > Free > Fixed > Tiled
  NeuroRectangle r;
  if (NEURO_CLIENT_PTR(c)->is_fullscreen) {
    NeuroGeometryGetIncreasedRectangle(&r, stack_region, NeuroCoreStackGetGaps(ws));
  } else if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull) {
    NEURO_CLIENT_PTR(c)->free_setter_fn(client_region, stack_region);
    memmove(&r, client_region, sizeof(NeuroRectangle));
  } else if (NEURO_CLIENT_PTR(c)->fixed_pos != NEURO_FIXED_POSITION_NULL) {
    NeuroRuleSetClientRegion(&r, c);
  } else {
    memmove(&r, client_region, sizeof(NeuroRectangle));
  }

  // Set border width and border gap
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(ws);
  const int border_width = l->border_width_setter_fn(c);
  const int border_gap = l->border_gap_setter_fn(c);
  NeuroGeometrySetRectangleBorderWidthAndGap(&r, border_width, border_gap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  // Draw
  const Window win = NEURO_CLIENT_PTR(c)->win;
  XSetWindowBorder(NeuroSystemGetDisplay(), win, l->border_color_setter_fn(c));
  XSetWindowBorderWidth(NeuroSystemGetDisplay(), win, border_width);
  XMoveResizeWindow(NeuroSystemGetDisplay(), win, r.p.x, r.p.y, r.w, r.h);
}

void NeuroClientUpdateClassAndName(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  XClassHint ch;
  if (!XGetClassHint(NeuroSystemGetDisplay(), NEURO_CLIENT_PTR(c)->win, &ch))
    return;

  // Reset class and name
  char *const class = NEURO_CLIENT_PTR(c)->class;
  char *const name = NEURO_CLIENT_PTR(c)->name;
  assert(class);
  assert(name);
  class[ 0 ] = '\0';
  name[ 0 ] = '\0';

  // Set new class and name
  strncpy(class, ch.res_class, NEURO_NAME_SIZE_MAX);
  strncpy(name, ch.res_name, NEURO_NAME_SIZE_MAX);

  // Clean up
  if (ch.res_class)
    XFree(ch.res_class);
  if (ch.res_name)
    XFree(ch.res_name);
}

void NeuroClientUpdateTitle(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Reset title
  char *const title = NEURO_CLIENT_PTR(c)->title;
  assert(title);
  title[ 0 ] = '\0';

  // Set new title
  if (!set_title_atom(*c, NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_NAME)))
    set_title_atom(*c, XA_WM_NAME);
}

void NeuroClientSetUrgent(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NEURO_CLIENT_PTR(c)->is_urgent = true;
}

void NeuroClientUnsetUrgent(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NEURO_CLIENT_PTR(c)->is_urgent = false;
}

void NeuroClientKill(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  const Window win = NEURO_CLIENT_PTR(c)->win;
  if (is_protocol_delete(win)) {
    XEvent ke;
    ke.type = ClientMessage;
    ke.xclient.window = win;
    ke.xclient.message_type = NeuroSystemGetWmAtom(NEURO_SYSTEM_WMATOM_PROTOCOLS);
    ke.xclient.format = 32;
    ke.xclient.data.l[ 0 ] = NeuroSystemGetWmAtom(NEURO_SYSTEM_WMATOM_DELETEWINDOW);
    ke.xclient.data.l[ 1 ] = CurrentTime;
    XSendEvent(NeuroSystemGetDisplay(), win, false, NoEventMask, &ke);
  } else {
    XKillClient(NeuroSystemGetDisplay(), win);
    NeuroEventUnmanageClient(c);
  }
}

void NeuroClientMinimize(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NeuroCoreSetCurrClient(NeuroCoreClientGetNext(c));
  NeuroClient *const cli = NeuroCoreRemoveClient(c);
  if (!cli)
    return;
  if (!NeuroCorePushMinimizedClient(cli))
    NeuroSystemError("NeuroClientMinimize - Could not minimize client");
  // Move client off screen
  XMoveWindow(NeuroSystemGetDisplay(), cli->win, NeuroSystemGetScreenRegion()->w + 1,
      NeuroSystemGetScreenRegion()->h + 1);
  NeuroLayoutRunCurr(cli->ws);
  NeuroWorkspaceFocus(cli->ws);
}

void NeuroClientTile(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (NEURO_CLIENT_PTR(c)->free_setter_fn == NeuroRuleFreeSetterNull)
    return;

  // Tile the client
  NEURO_CLIENT_PTR(c)->free_setter_fn = NeuroRuleFreeSetterNull;
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroClientFree(NeuroClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  const NeuroArgFn *gaf = (const NeuroArgFn *)freeSetterFn;
  if (NEURO_CLIENT_PTR(c)->free_setter_fn == gaf->FreeSetterFn_)
    return;

  // Free the client
  NEURO_CLIENT_PTR(c)->free_setter_fn = gaf->FreeSetterFn_;
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroClientToggleFree(NeuroClientPtrPtr c, const void *free_setter_fn) {
  if (!c)
    return;

  // Toggle free the client
  if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull)
    NeuroClientTile(c, NULL);
  else
    NeuroClientFree(c, free_setter_fn);
}

void NeuroClientNormal(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (!NEURO_CLIENT_PTR(c)->is_fullscreen)
    return;

  // Normal the client
  NEURO_CLIENT_PTR(c)->is_fullscreen = false;
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroClientFullscreen(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (NEURO_CLIENT_PTR(c)->is_fullscreen)
    return;

  // Fullscreen the client
  NEURO_CLIENT_PTR(c)->is_fullscreen = true;
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroClientToggleFullscreen(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Toggle fullscreen the client
  if (NEURO_CLIENT_PTR(c)->is_fullscreen)
    NeuroClientNormal(c, NULL);
  else
    NeuroClientFullscreen(c, NULL);
}

void NeuroClientFloatMove(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Focus the client and return if it is fixed
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  if (NEURO_CLIENT_PTR(c)->fixed_pos != NEURO_FIXED_POSITION_NULL)
    return;

  // Float move the client
  NeuroRectangle *const r = &(NEURO_CLIENT_PTR(c)->float_region), cr;
  memmove(&cr, r, sizeof(NeuroRectangle));
  NeuroPoint p;
  process_xmotion(r, NEURO_CLIENT_PTR(c)->ws, &cr, NeuroSystemGetPointerLocation(&p), xmotion_move,
      NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_MOVE));
}

void NeuroClientFloatResize(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Focus the client and return if it is fixed
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  if (NEURO_CLIENT_PTR(c)->fixed_pos != NEURO_FIXED_POSITION_NULL)
    return;

  // Float resize the client
  NeuroRectangle *const r = &(NEURO_CLIENT_PTR(c)->float_region), cr;
  memmove(&cr, r, sizeof(NeuroRectangle));
  NeuroPoint p;
  process_xmotion(r, NEURO_CLIENT_PTR(c)->ws, &cr, NeuroSystemGetPointerLocation(&p), xmotion_resize,
      NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_RESIZE));
}

void NeuroClientFreeMove(NeuroClientPtrPtr c, const void *free_setter_fn) {
  if (!c || !free_setter_fn)
    return;

  // Focus client and set it to free mode
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroClientFree(c, free_setter_fn);

  // Free move the client
  NeuroRectangle *const r = NeuroCoreClientGetRegion(c), cr;
  memmove(&cr, r, sizeof(NeuroRectangle));
  NeuroPoint p;
  process_xmotion(r, NEURO_CLIENT_PTR(c)->ws, &cr, NeuroSystemGetPointerLocation(&p), xmotion_move,
      NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_MOVE));
}

void NeuroClientFreeResize(NeuroClientPtrPtr c, const void *free_setter_fn) {
  if (!c || !free_setter_fn)
    return;

  // Focus client and set it to free mode
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroClientFree(c, free_setter_fn);

  // Free resize the client
  NeuroRectangle *const r = NeuroCoreClientGetRegion(c), cr;
  memmove(&cr, r, sizeof(NeuroRectangle));
  NeuroPoint p;
  process_xmotion(r, NEURO_CLIENT_PTR(c)->ws, &cr, NeuroSystemGetPointerLocation(&p), xmotion_resize,
      NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_RESIZE));
}

// Find
NeuroClientPtrPtr NeuroClientFindWindow(Window w) {
  return NeuroCoreFindClient(NeuroClientTesterWindow, (const void *)&w);
}

NeuroClientPtrPtr NeuroClientFindUrgent() {
  return NeuroCoreFindClient(NeuroClientTesterUrgent, NULL);
}

NeuroClientPtrPtr NeuroClientFindFixed() {
  return NeuroCoreFindClient(NeuroClientTesterFixed, NULL);
}

NeuroClientPtrPtr NeuroClientFindPointed(const NeuroPoint *p) {
  return NeuroCoreFindClient(NeuroClientTesterPointed, (const void *)p);
}

// Client getters
NeuroClientPtrPtr NeuroClientGetFocused() {
  return NeuroCoreStackGetCurrClient(NeuroCoreGetCurrStack());
}

NeuroClientPtrPtr NeuroClientGetPointedByPointer() {
  NeuroPoint p;
  return NeuroClientFindPointed(NeuroSystemGetPointerLocation(&p));
}

// Client Testers
bool NeuroClientTesterWindow(const NeuroClientPtrPtr c, const void *w) {
  if (!c || !w)
    return false;
  return NEURO_CLIENT_PTR(c)->win == *((const Window *)w);
}

bool NeuroClientTesterUrgent(const NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return false;
  return NEURO_CLIENT_PTR(c)->is_urgent;
}

bool NeuroClientTesterFixed(const NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return false;
  return NEURO_CLIENT_PTR(c)->fixed_pos != NEURO_FIXED_POSITION_NULL;
}

bool NeuroClientTesterPointed(const NeuroClientPtrPtr c, const void *p) {
  if (!c || !p)
    return false;
  return NeuroGeometryIsPointInRectangle(NeuroCoreClientGetRegion(c), (const NeuroPoint *)p);
}

// Client Selectors
NeuroClientPtrPtr NeuroClientSelectorSelf(const NeuroClientPtrPtr c) {
  assert(c);
  return c;
}

NeuroClientPtrPtr NeuroClientSelectorNext(const NeuroClientPtrPtr c) {
  assert(c);
  NeuroClientPtrPtr n = NeuroCoreClientGetNext(c);
  if (!n)
    n = NeuroCoreStackGetHeadClient(NEURO_CLIENT_PTR(c)->ws);
  return n;
}

NeuroClientPtrPtr NeuroClientSelectorPrev(const NeuroClientPtrPtr c) {
  assert(c);
  NeuroClientPtrPtr p = NeuroCoreClientGetPrev(c);
  if (!p)
    p = NeuroCoreStackGetLastClient(NEURO_CLIENT_PTR(c)->ws);
  return p;
}

NeuroClientPtrPtr NeuroClientSelectorOld(const NeuroClientPtrPtr c) {
  assert(c);
  return NeuroCoreStackGetPrevClient(NEURO_CLIENT_PTR(c)->ws);
}

NeuroClientPtrPtr NeuroClientSelectorHead(const NeuroClientPtrPtr c) {
  assert(c);
  return NeuroCoreStackGetHeadClient(NEURO_CLIENT_PTR(c)->ws);
}

NeuroClientPtrPtr NeuroClientSelectorLast(const NeuroClientPtrPtr c) {
  assert(c);
  return NeuroCoreStackGetLastClient(NEURO_CLIENT_PTR(c)->ws);
}

NeuroClientPtrPtr NeuroClientSelectorUpper(const NeuroClientPtrPtr c) {
  assert(c);
  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryGetUpperPoint(&p, &r->p, 1);
  ++p.x;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

NeuroClientPtrPtr NeuroClientSelectorLower(const NeuroClientPtrPtr c) {
  assert(c);
  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryGetLowerPoint(&p, &r->p, r->h + 1);
  ++p.x;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

NeuroClientPtrPtr NeuroClientSelectorLeft(const NeuroClientPtrPtr c) {
  assert(c);
  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryGetLeftPoint(&p, &r->p, 1);
  ++p.y;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

NeuroClientPtrPtr NeuroClientSelectorRight(const NeuroClientPtrPtr c) {
  assert(c);
  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryGetRightPoint(&p, &r->p, r->w + 1);
  ++p.y;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

// Color Setters
NeuroColor NeuroClientColorSetterCurr(const NeuroClientPtrPtr c) {
  assert(c);
  return NeuroCoreClientIsCurr(c) ? NeuroSystemGetColor(NEURO_SYSTEM_COLOR_CURRENT) :
      NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);
}

NeuroColor NeuroClientColorSetterAll(const NeuroClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_CURRENT);
  else if (NEURO_CLIENT_PTR(c)->is_urgent)
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_URGENT);
  else if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull)
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_FREE);
  else if (NeuroCoreClientIsPrev(c))
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_OLD);
  return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);
}

NeuroColor NeuroClientColorSetterNone(const NeuroClientPtrPtr c) {
  (void)c;
  return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);
}

// Border Width Setters
int NeuroClientBorderWidthSetterAlways(const NeuroClientPtrPtr c) {
  (void)c;
  return NeuroConfigGet()->border_width;
}

int NeuroClientBorderWidthSetterNever(const NeuroClientPtrPtr c) {
  (void)c;
  return 0;
}

int NeuroClientBorderWidthSetterSmart(const NeuroClientPtrPtr c) {
  assert(c);

  // No border if fullscreen
  if (NEURO_CLIENT_PTR(c)->is_fullscreen)
    return 0;

  // Border if free
  if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull)
    return NeuroConfigGet()->border_width;

  // Border if float layout
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(ws);
  if (l->arranger_fn == NeuroLayoutArrangerFloat)
    return NeuroConfigGet()->border_width;

  // Border if workspace has a fixed client
  if (NeuroWorkspaceClientFindFixed(ws))
    return NeuroConfigGet()->border_width;

  // No border if the client fits all stack region
  const NeuroRectangle *const rc = NeuroCoreClientGetRegion(c);
  const NeuroRectangle *const rs = NeuroCoreStackGetRegion(ws);
  if ((rc->w == rs->w && rc->h == rs->h) ||
      (rc->w == NeuroSystemGetScreenRegion()->w && rc->h == NeuroSystemGetScreenRegion()->h))
    return 0;

  // Border otherwise
  return NeuroConfigGet()->border_width;
}

int NeuroClientBorderWidthSetterCurr(const NeuroClientPtrPtr c) {
  assert(c);
  return NeuroCoreClientIsCurr(c) ? NeuroConfigGet()->border_width : 0;
}

// Border Gap Setters
int NeuroClientBorderGapSetterAlways(const NeuroClientPtrPtr c) {
  assert(c);

  // No gap if client is free
  if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;

  // No gap if float layout
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(NEURO_CLIENT_PTR(c)->ws);
  if (NEURO_CLIENT_PTR(c)->is_fullscreen || NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;

  // Gap otherwise
  return NeuroConfigGet()->border_gap;
}

int NeuroClientBorderGapSetterNever(const NeuroClientPtrPtr c) {
  (void)c;
  return 0;
}

int NeuroClientBorderGapSetterSmart(const NeuroClientPtrPtr c) {
  assert(c);

  // No gap if client is free
  if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;

  // No gap if float layout
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(ws);
  if (NEURO_CLIENT_PTR(c)->is_fullscreen || NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;

  // No gapr if the client fits all stack region
  const NeuroRectangle *const a = NeuroCoreClientGetRegion(c);
  const NeuroRectangle *const as = NeuroCoreStackGetRegion(ws);
  if ((a->w == as->w && a->h == as->h) ||
      (a->w == NeuroSystemGetScreenRegion()->w && a->h == NeuroSystemGetScreenRegion()->h))
    return 0;

  // Gap otherwise
  return NeuroConfigGet()->border_gap;
}

int NeuroClientBorderGapSetterCurr(const NeuroClientPtrPtr c) {
  assert(c);

  // No gap if client is free
  if (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;

  // No gap if float layout
  NeuroLayout *l = NeuroCoreStackGetCurrLayout(NEURO_CLIENT_PTR(c)->ws);
  if (NEURO_CLIENT_PTR(c)->is_fullscreen || NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;

  // Gap if current, no gap otherwise
  return NeuroCoreClientIsCurr(c) ? NeuroConfigGet()->border_gap : 0;
}

