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
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  const NeuroRectangle *const stack_region = NeuroCoreStackGetRegion(client->ws);
  NeuroRectangle *const client_region = NeuroCoreClientGetRegion(c);

  // Priority: Fullscreen > Free > Fixed > Tiled
  NeuroRectangle r;
  if (client->is_fullscreen) {
    NeuroGeometryRectangleGetIncreased(&r, stack_region, NeuroCoreStackGetGaps(client->ws));
  } else if (client->free_setter_fn != NeuroRuleFreeSetterNull) {
    client->free_setter_fn(client_region, stack_region);
    memmove(&r, client_region, sizeof(NeuroRectangle));
  } else if (client->fixed_pos != NEURO_FIXED_POSITION_NULL) {
    NeuroRuleSetClientRegion(&r, c);
  } else {
    memmove(&r, client_region, sizeof(NeuroRectangle));
  }

  // Set border width and border gap
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(client->ws);
  const int border_width = l->border_width_setter_fn(c);
  const int border_gap = l->border_gap_setter_fn(c);
  NeuroGeometryRectangleSetBorderWidthAndGap(&r, border_width, border_gap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  // Draw
  XSetWindowBorder(NeuroSystemGetDisplay(), client->win, l->border_color_setter_fn(c));
  XSetWindowBorderWidth(NeuroSystemGetDisplay(), client->win, border_width);
  XMoveResizeWindow(NeuroSystemGetDisplay(), client->win, r.p.x, r.p.y, r.w, r.h);
}

void NeuroClientUpdateClassAndName(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  XClassHint ch;
  if (!XGetClassHint(NeuroSystemGetDisplay(), client->win, &ch))
    return;

  // Reset class and name
  assert(client->class);
  client->class[ 0 ] = '\0';
  assert(client->name);
  client->name[ 0 ] = '\0';

  // Set new class and name
  strncpy(client->class, ch.res_class, NEURO_NAME_SIZE_MAX);
  strncpy(client->name, ch.res_name, NEURO_NAME_SIZE_MAX);

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
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  assert(client->title);
  client->title[ 0 ] = '\0';

  // Set new title
  if (!set_title_atom(client, NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_NAME)))
    set_title_atom(client, XA_WM_NAME);
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
    NeuroSystemError(__func__, "Could not minimize client");
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

  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->free_setter_fn == NeuroRuleFreeSetterNull)
    return;

  // Tile the client
  client->free_setter_fn = NeuroRuleFreeSetterNull;
  NeuroLayoutRunCurr(client->ws);
  NeuroWorkspaceFocus(client->ws);
}

void NeuroClientFree(NeuroClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;

  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  const NeuroArgFn *gaf = (const NeuroArgFn *)freeSetterFn;
  if (client->free_setter_fn == gaf->FreeSetterFn_)
    return;

  // Free the client
  client->free_setter_fn = gaf->FreeSetterFn_;
  NeuroLayoutRunCurr(client->ws);
  NeuroWorkspaceFocus(client->ws);
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

  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (!client->is_fullscreen)
    return;

  // Normal the client
  client->is_fullscreen = false;
  NeuroLayoutRunCurr(client->ws);
  NeuroWorkspaceFocus(client->ws);
}

void NeuroClientFullscreen(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->is_fullscreen)
    return;

  // Fullscreen the client
  client->is_fullscreen = true;
  NeuroLayoutRunCurr(client->ws);
  NeuroWorkspaceFocus(client->ws);
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

  // Focus the client
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);

  // Return if the client is fixed
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->fixed_pos != NEURO_FIXED_POSITION_NULL)
    return;

  // Float move the client
  NeuroRectangle *const r = &client->float_region, cr;
  memmove(&cr, r, sizeof(NeuroRectangle));
  NeuroPoint p;
  NeuroSystemGetPointerWindowLocation(&p, NULL);
  process_xmotion(r, client->ws, &cr, &p, xmotion_move, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_MOVE));
}

void NeuroClientFloatResize(NeuroClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Focus the client
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);

  // Return if the client is fixed
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->fixed_pos != NEURO_FIXED_POSITION_NULL)
    return;

  // Float resize the client
  NeuroRectangle *const r = &client->float_region, cr;
  memmove(&cr, r, sizeof(NeuroRectangle));
  NeuroPoint p;
  NeuroSystemGetPointerWindowLocation(&p, NULL);
  process_xmotion(r, client->ws, &cr, &p, xmotion_resize, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_RESIZE));
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
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  NeuroPoint p;
  NeuroSystemGetPointerWindowLocation(&p, NULL);
  process_xmotion(r, client->ws, &cr, &p, xmotion_move, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_MOVE));
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
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  NeuroPoint p;
  NeuroSystemGetPointerWindowLocation(&p, NULL);
  process_xmotion(r, client->ws, &cr, &p, xmotion_resize, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_RESIZE));
}

// Find
NeuroClientPtrPtr NeuroClientFindWindow(Window w) {
  return NeuroCoreFindClient(NeuroClientTesterWindow, (const void *)&w);
}

NeuroClientPtrPtr NeuroClientFindUrgent(void) {
  return NeuroCoreFindClient(NeuroClientTesterUrgent, NULL);
}

NeuroClientPtrPtr NeuroClientFindFixed(void) {
  return NeuroCoreFindClient(NeuroClientTesterFixed, NULL);
}

// Note: This might return a lower window in the stacking order, use NeuroClientGetPointedByPointer() to always get
// the upper window in the stacking order
NeuroClientPtrPtr NeuroClientFindPointed(const NeuroPoint *p) {
  return NeuroCoreFindClient(NeuroClientTesterPointed, (const void *)p);
}

NeuroClientPtrPtr NeuroClientFindHidden(void) {
  return NeuroCoreFindClient(NeuroClientTesterHidden, NULL);
}

// Client getters
NeuroClientPtrPtr NeuroClientGetFocused(void) {
  return NeuroCoreStackGetCurrClient(NeuroCoreGetCurrStack());
}

NeuroClientPtrPtr NeuroClientGetPointedByPointer(void) {
  Window w;
  NeuroSystemGetPointerWindowLocation(NULL, &w);
  return NeuroClientFindWindow(w);
}

// Client Testers
bool NeuroClientTesterWindow(const NeuroClientPtrPtr c, const void *w) {
  return c && w && NEURO_CLIENT_PTR(c)->win == *((const Window *)w);
}

bool NeuroClientTesterUrgent(const NeuroClientPtrPtr c, const void *data) {
  (void)data;
  return c && NEURO_CLIENT_PTR(c)->is_urgent;
}

bool NeuroClientTesterFixed(const NeuroClientPtrPtr c, const void *data) {
  (void)data;
  return c && NEURO_CLIENT_PTR(c)->fixed_pos != NEURO_FIXED_POSITION_NULL;
}

bool NeuroClientTesterPointed(const NeuroClientPtrPtr c, const void *p) {
  return c && p && NeuroGeometryIsPointInRectangle(NeuroCoreClientGetRegion(c), (const NeuroPoint *)p);
}

bool NeuroClientTesterHidden(const NeuroClientPtrPtr c, const void *data) {
  (void)data;
  return c && NeuroGeometryIsPointInRectangle(NeuroSystemGetHiddenRegion(), &NeuroCoreClientGetRegion(c)->p);
}

bool NeuroClientTesterFullscreen(const NeuroClientPtrPtr c, const void *data) {
  (void)data;
  return c && NEURO_CLIENT_PTR(c)->is_fullscreen;
}

// Client Selectors
NeuroClientPtrPtr NeuroClientSelectorSelf(const NeuroClientPtrPtr c) {
  return c;
}

NeuroClientPtrPtr NeuroClientSelectorNext(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  // Get the head client if next is null
  NeuroClientPtrPtr next = NeuroCoreClientGetNext(c);
  if (!next)
    next = NeuroCoreStackGetHeadClient(NEURO_CLIENT_PTR(c)->ws);

  return next;
}

NeuroClientPtrPtr NeuroClientSelectorPrev(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  // Get the last client if prev is null
  NeuroClientPtrPtr prev = NeuroCoreClientGetPrev(c);
  if (!prev)
    prev = NeuroCoreStackGetLastClient(NEURO_CLIENT_PTR(c)->ws);

  return prev;
}

NeuroClientPtrPtr NeuroClientSelectorOld(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  return NeuroCoreStackGetPrevClient(NEURO_CLIENT_PTR(c)->ws);
}

NeuroClientPtrPtr NeuroClientSelectorHead(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  return NeuroCoreStackGetHeadClient(NEURO_CLIENT_PTR(c)->ws);
}

NeuroClientPtrPtr NeuroClientSelectorLast(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  return NeuroCoreStackGetLastClient(NEURO_CLIENT_PTR(c)->ws);
}

NeuroClientPtrPtr NeuroClientSelectorUpper(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryPointGetUpper(&p, &r->p, 1);
  ++p.x;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

NeuroClientPtrPtr NeuroClientSelectorLower(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryPointGetLower(&p, &r->p, r->h + 1);
  ++p.x;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

NeuroClientPtrPtr NeuroClientSelectorLeft(const NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryPointGetLeft(&p, &r->p, 1);
  ++p.y;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

NeuroClientPtrPtr NeuroClientSelectorRight(const NeuroClientPtrPtr c) {
  assert(c);
  const NeuroRectangle *const r = NeuroCoreClientGetRegion(c);
  NeuroPoint p;
  NeuroGeometryPointGetRight(&p, &r->p, r->w + 1);
  ++p.y;  // Needed to avoid border issues
  return NeuroWorkspaceClientFindPointed(NEURO_CLIENT_PTR(c)->ws, &p);
}

// Color Setters
NeuroColor NeuroClientColorSetterCurr(const NeuroClientPtrPtr c) {
  if (!c)
    NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);

  return NeuroCoreClientIsCurr(c) ? NeuroSystemGetColor(NEURO_SYSTEM_COLOR_CURRENT) :
      NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);
}

NeuroColor NeuroClientColorSetterAll(const NeuroClientPtrPtr c) {
  if (!c)
    NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);

  if (NeuroCoreClientIsCurr(c))
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_CURRENT);

  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->is_urgent)
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_URGENT);

  if (client->free_setter_fn != NeuroRuleFreeSetterNull)
    return NeuroSystemGetColor(NEURO_SYSTEM_COLOR_FREE);

  return NeuroCoreClientIsPrev(c) ? NeuroSystemGetColor(NEURO_SYSTEM_COLOR_OLD) :
      NeuroSystemGetColor(NEURO_SYSTEM_COLOR_NORMAL);
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
  if (!c)
    return 0;

  // No border if fullscreen
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->is_fullscreen)
    return 0;

  // Border if free
  if (client->free_setter_fn != NeuroRuleFreeSetterNull)
    return NeuroConfigGet()->border_width;

  // Border if float layout
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(client->ws);
  if (l->arranger_fn == NeuroLayoutArrangerFloat)
    return NeuroConfigGet()->border_width;

  // Border if workspace has a fixed client
  if (NeuroWorkspaceClientFindFixed(client->ws))
    return NeuroConfigGet()->border_width;

  // No border if the client fits all stack region
  const NeuroRectangle *const rc = NeuroCoreClientGetRegion(c);
  const NeuroRectangle *const rs = NeuroCoreStackGetRegion(client->ws);
  if ((rc->w == rs->w && rc->h == rs->h) ||
      (rc->w == NeuroSystemGetScreenRegion()->w && rc->h == NeuroSystemGetScreenRegion()->h))
    return 0;

  // Border otherwise
  return NeuroConfigGet()->border_width;
}

int NeuroClientBorderWidthSetterCurr(const NeuroClientPtrPtr c) {
  if (!c)
    return 0;
  return NeuroCoreClientIsCurr(c) ? NeuroConfigGet()->border_width : 0;
}

// Border Gap Setters
int NeuroClientBorderGapSetterAlways(const NeuroClientPtrPtr c) {
  if (!c)
    return 0;

  // No gap if client is free
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;

  // No gap if float layout
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(client->ws);
  if (client->is_fullscreen || client->free_setter_fn != NeuroRuleFreeSetterNull ||
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
  if (!c)
    return 0;

  // No gap if client is free
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;

  // No gap if float layout
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(client->ws);
  if (client->is_fullscreen || client->free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;

  // No gapr if the client fits all stack region
  const NeuroRectangle *const a = NeuroCoreClientGetRegion(c);
  const NeuroRectangle *const as = NeuroCoreStackGetRegion(client->ws);
  if ((a->w == as->w && a->h == as->h) ||
      (a->w == NeuroSystemGetScreenRegion()->w && a->h == NeuroSystemGetScreenRegion()->h))
    return 0;

  // Gap otherwise
  return NeuroConfigGet()->border_gap;
}

int NeuroClientBorderGapSetterCurr(const NeuroClientPtrPtr c) {
  if (!c)
    return 0;

  // No gap if client is free
  NeuroClient *const client = NEURO_CLIENT_PTR(c);
  if (client->free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;

  // No gap if float layout
  NeuroLayout *const l = NeuroCoreStackGetCurrLayout(client->ws);
  if (client->is_fullscreen || client->free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;

  // Gap if current, no gap otherwise
  return NeuroCoreClientIsCurr(c) ? NeuroConfigGet()->border_gap : 0;
}

