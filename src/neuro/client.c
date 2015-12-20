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
typedef void (*XMotionUpdaterFn)(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Bool is_protocol_delete(Window w) {
  int i, n;
  Atom *protocols;
  Bool ret = False;
  if (XGetWMProtocols(NeuroSystemGetDisplay(), w, &protocols, &n)) {
    for (i = 0; !ret && i < n; i++)
      if (protocols[ i ] == NeuroSystemGetWmAtom(NeuroSystemWmAtomDeleteWindow))
        ret = True;
    XFree(protocols);
  }
  return ret;
}

static Bool set_title_atom(Client *c, Atom atom) {
  assert(c);
  XTextProperty tp;
  XGetTextProperty(NeuroSystemGetDisplay(), c->win, &tp, atom);
  if (!tp.nitems)
    return False;
  if (tp.encoding == XA_STRING) {
    strncpy(c->title, (char *)tp.value, NAME_MAX);
  } else {
    char **list = NULL;
    int n;
    if (XmbTextPropertyToTextList(NeuroSystemGetDisplay(), &tp, &list, &n) >= Success && n > 0 && list[ 0 ]) {
      strncpy(c->title, list[ 0 ], NAME_MAX);
      XFreeStringList(list);
    }
  }
  XFree(tp.value);
  return True;
}

static ClientPtrPtr query_pointer_client(int ws, int x, int y) {
  ClientPtrPtr c;
  for (c=NeuroCoreStackGetHeadClient(ws); c; c=NeuroCoreClientGetNext(c))
    if (NeuroGeometryIsPointInRectangle(NeuroCoreClientGetRegion(c), x, y))
      break;
  return c;
}

static void process_xmotion(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int px, int py,
    XMotionUpdaterFn mpf) {
  const Bool res = XGrabPointer(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), False,
      ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None,
      NeuroSystemGetCursor(NeuroSystemCursorMove), CurrentTime);
  if (res != GrabSuccess)
    return;
  XEvent ev;
  do {
    XMaskEvent(NeuroSystemGetDisplay(), ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify)
      mpf(r, ws, cx, cy, cw, ch, ev.xmotion.x, ev.xmotion.y, px, py);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(NeuroSystemGetDisplay(), CurrentTime);
}

static void xmotion_move(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py) {
  (void)cw;
  (void)ch;
  r->x = cx + (ex - px);
  r->y = cy + (ey - py);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceUpdate(ws);
}

static void xmotion_resize(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py) {
  (void)cx;
  (void)cy;
  (void)px;
  (void)py;
  r->w = cw + (ex - (cw + r->x));
  r->h = ch + (ey - (ch + r->y));
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceUpdate(ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
void NeuroClientUpdate(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Free windows
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    CLI_GET(c).free_setter_fn(NeuroCoreClientGetRegion(c), NeuroSystemGetScreenRegion());

  // Fullscreen windows
  Rectangle r;
  if (CLI_GET(c).is_fullscreen)
    memmove(&r, NeuroSystemGetScreenRegion(), sizeof(Rectangle));
  else
    memmove(&r, NeuroCoreClientGetRegion(c), sizeof(Rectangle));

  // Set border width and border gap
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  const int border_width = l->border_width_setter_fn(c);
  const int border_gap = l->border_gap_setter_fn(c);
  NeuroGeometrySetRectangleBorderWidthAndGap(&r, border_width, border_gap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  // Draw
  XSetWindowBorder(NeuroSystemGetDisplay(), CLI_GET(c).win, l->border_color_setter_fn(c));
  XSetWindowBorderWidth(NeuroSystemGetDisplay(), CLI_GET(c).win, border_width);
  XMoveResizeWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, r.x, r.y, r.w, r.h);
}

void NeuroClientUpdateClassAndName(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).class[ 0 ] = '\0';
  CLI_GET(c).name[ 0 ] = '\0';
  XClassHint ch = (XClassHint){.res_name = NULL, .res_class = NULL};
  if (!XGetClassHint(NeuroSystemGetDisplay(), CLI_GET(c).win, &ch))
    return;
  strncpy(CLI_GET(c).class, ch.res_class, NAME_MAX);
  strncpy(CLI_GET(c).name, ch.res_name, NAME_MAX);
  if (ch.res_class)
    XFree(ch.res_class);
  if (ch.res_name)
    XFree(ch.res_name);
}

void NeuroClientUpdateTitle(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).title[ 0 ] = '\0';
  if (!set_title_atom(*c, NeuroSystemGetNetAtom(NeuroSystemNetAtomName)))
    set_title_atom(*c, XA_WM_NAME);
}

void NeuroClientHide(ClientPtrPtr c, const void *doRules) {  // Move off screen
  if (!c)
    return;
  if (CLI_GET(c).is_hidden)
    return;
  if (*(Bool *)doRules)
    NeuroRuleUnapply(c);
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  regc->x += NeuroSystemGetScreenRegion()->w;
  regc->y += NeuroSystemGetScreenRegion()->h;
  XMoveWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).is_hidden = True;
}

void NeuroClientShow(ClientPtrPtr c, const void *doRules) {  // Move back to screen
  if (!c)
    return;
  if (!CLI_GET(c).is_hidden)
    return;
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  regc->x -= NeuroSystemGetScreenRegion()->w;
  regc->y -= NeuroSystemGetScreenRegion()->h;
  if (*(Bool *)doRules)
    NeuroRuleApply(c);
  XMoveWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).is_hidden = False;
}

void NeuroClientSetUrgent(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).is_urgent = True;
}

void NeuroClientUnsetUrgent(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).is_urgent = False;
}

void NeuroClientKill(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (is_protocol_delete(CLI_GET(c).win)) {
    XEvent ke;
    ke.type = ClientMessage;
    ke.xclient.window = CLI_GET(c).win;
    ke.xclient.message_type = NeuroSystemGetWmAtom(NeuroSystemWmAtomProtocols);
    ke.xclient.format = 32;
    ke.xclient.data.l[ 0 ] = NeuroSystemGetWmAtom(NeuroSystemWmAtomDeleteWindow);
    ke.xclient.data.l[ 1 ] = CurrentTime;
    XSendEvent(NeuroSystemGetDisplay(), CLI_GET(c).win, False, NoEventMask, &ke);
  } else {
    XKillClient(NeuroSystemGetDisplay(), CLI_GET(c).win);
    NeuroEventUnmanageClient(c);
  }
}

void NeuroClientMinimize(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NeuroCoreSetCurrClient(NeuroCoreClientGetNext(c));
  NeuroRuleUnapply(c);
  Client *cli = NeuroCoreRemoveClient(c);
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

void NeuroClientTile(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).free_setter_fn == NeuroRuleFreeSetterNull)
    return;
  CLI_GET(c).free_setter_fn = NeuroRuleFreeSetterNull;
  NeuroRuleApply(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  NeuroWorkspaceFocus(CLI_GET(c).ws);
}

void NeuroClientFree(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  const GenericArgFn *gaf = (const GenericArgFn *)freeSetterFn;
  if (CLI_GET(c).free_setter_fn == gaf->FreeSetterFn_)
    return;
  if (CLI_GET(c).free_setter_fn == NeuroRuleFreeSetterNull)
    NeuroRuleUnapply(c);
  CLI_GET(c).free_setter_fn = gaf->FreeSetterFn_;
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  NeuroWorkspaceFocus(CLI_GET(c).ws);
}

void NeuroClientToggleFree(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    NeuroClientTile(c, NULL);
  else
    NeuroClientFree(c, freeSetterFn);
}

void NeuroClientNormal(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (!CLI_GET(c).is_fullscreen)
    return;
  CLI_GET(c).is_fullscreen = False;
  NeuroRuleApply(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  NeuroWorkspaceFocus(CLI_GET(c).ws);
}

void NeuroClientFullscreen(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).is_fullscreen)
    return;
  CLI_GET(c).is_fullscreen = True;
  NeuroRuleUnapply(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  NeuroWorkspaceFocus(CLI_GET(c).ws);
}

void NeuroClientToggleFullscreen(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).is_fullscreen)
    NeuroClientNormal(c, NULL);
  else
    NeuroClientFullscreen(c, NULL);
}

void NeuroClientFloatMove(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  Rectangle *r = &(CLI_GET(c).float_region);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_move);
}

void NeuroClientFloatResize(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  Rectangle *r = &(CLI_GET(c).float_region);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_resize);
}

void NeuroClientFreeMove(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c || !freeSetterFn)
    return;
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroClientFree(c, freeSetterFn);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_move);
}

void NeuroClientFreeResize(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c || !freeSetterFn)
    return;
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroClientFree(c, freeSetterFn);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_resize);
}

// Find
ClientPtrPtr NeuroClientFindWindow(Window w) {
  return NeuroCoreFindClient(NeuroClientTesterWindow, (const void *)&w);
}

ClientPtrPtr NeuroClientFindUrgent() {
  return NeuroCoreFindClient(NeuroClientTesterUrgent, NULL);
}

ClientPtrPtr NeuroClientFindFixed() {
  return NeuroCoreFindClient(NeuroClientTesterFixed, NULL);
}

// Client getters
ClientPtrPtr NeuroClientGetFocused() {
  return NeuroCoreStackGetCurrClient(NeuroCoreGetCurrStack());
}

ClientPtrPtr NeuroClientGetPointed(int *x, int *y) {
  assert(x);
  assert(y);
  Window rootw, childw;
  int xc, yc;
  unsigned state;
  if (!XQueryPointer(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), &rootw, &childw, x, y, &xc, &yc, &state))
    return NULL;
  return NeuroClientFindWindow(childw);
}

// Client Selectors
ClientPtrPtr NeuroClientSelectorSelf(const ClientPtrPtr c) {
  assert(c);
  return c;
}

ClientPtrPtr NeuroClientSelectorNext(const ClientPtrPtr c) {
  assert(c);
  ClientPtrPtr n = NeuroCoreClientGetNext(c);
  if (!n)
    n = NeuroCoreStackGetHeadClient(CLI_GET(c).ws);
  return n;
}

ClientPtrPtr NeuroClientSelectorPrev(const ClientPtrPtr c) {
  assert(c);
  ClientPtrPtr p = NeuroCoreClientGetPrev(c);
  if (!p)
    p = NeuroCoreStackGetLastClient(CLI_GET(c).ws);
  return p;
}

ClientPtrPtr NeuroClientSelectorOld(const ClientPtrPtr c) {
  assert(c);
  return NeuroCoreStackGetPrevClient(CLI_GET(c).ws);
}

ClientPtrPtr NeuroClientSelectorHead(const ClientPtrPtr c) {
  assert(c);
  return NeuroCoreStackGetHeadClient(CLI_GET(c).ws);
}

ClientPtrPtr NeuroClientSelectorLast(const ClientPtrPtr c) {
  assert(c);
  return NeuroCoreStackGetLastClient(CLI_GET(c).ws);
}

ClientPtrPtr NeuroClientSelectorUp(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  return query_pointer_client(CLI_GET(c).ws, r->x+1, r->y-1);
}

ClientPtrPtr NeuroClientSelectorDown(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  return query_pointer_client(CLI_GET(c).ws, r->x+1, r->y + r->h + 1);
}

ClientPtrPtr NeuroClientSelectorLeft(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  return query_pointer_client(CLI_GET(c).ws, r->x-1, r->y+1);
}

ClientPtrPtr NeuroClientSelectorRight(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  return query_pointer_client(CLI_GET(c).ws, r->x + r->w + 1, r->y+1);
}

// Client Testers
Bool NeuroClientTesterWindow(const ClientPtrPtr c, const void *w) {
  assert(c);
  assert(w);
  return CLI_GET(c).win == *((Window *)w);
}

Bool NeuroClientTesterUrgent(const ClientPtrPtr c, const void *data) {
  assert(c);
  (void)data;
  return CLI_GET(c).is_urgent;
}

Bool NeuroClientTesterFixed(const ClientPtrPtr c, const void *data) {
  assert(c);
  (void)data;
  return CLI_GET(c).fixed_pos != RuleFixedPositionNull;
}

// Color Setters
Color NeuroClientColorSetterCurr(const ClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return NeuroSystemGetColor(NeuroSystemColorCurrent);
  return NeuroSystemGetColor(NeuroSystemColorNormal);
}

Color NeuroClientColorSetterAll(const ClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return NeuroSystemGetColor(NeuroSystemColorCurrent);
  else if (CLI_GET(c).is_urgent)
    return NeuroSystemGetColor(NeuroSystemColorUrgent);
  else if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    return NeuroSystemGetColor(NeuroSystemColorFree);
  else if (NeuroCoreClientIsPrev(c))
    return NeuroSystemGetColor(NeuroSystemColorOld);
  return NeuroSystemGetColor(NeuroSystemColorNormal);
}

Color NeuroClientColorSetterNone(const ClientPtrPtr c) {
  (void)c;
  return NeuroSystemGetColor(NeuroSystemColorNormal);
}

// Border Width Setters
int NeuroClientBorderWidthSetterAlways(const ClientPtrPtr c) {
  (void)c;
  return NeuroConfigGet()->border_width;
}

int NeuroClientBorderWidthSetterNever(const ClientPtrPtr c) {
  (void)c;
  return 0;
}

int NeuroClientBorderWidthSetterSmart(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).is_fullscreen)
    return 0;
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    return NeuroConfigGet()->border_width;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (l->arranger_fn == NeuroLayoutArrangerFloat)
    return NeuroConfigGet()->border_width;
  if (NeuroWorkspaceClientFindFixed(CLI_GET(c).ws))
    return NeuroConfigGet()->border_width;
  Rectangle *rc = NeuroCoreClientGetRegion(c);
  Rectangle *rs = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  if ((rc->w == rs->w && rc->h == rs->h) ||
      (rc->w == NeuroSystemGetScreenRegion()->w && rc->h == NeuroSystemGetScreenRegion()->h))
    return 0;
  return NeuroConfigGet()->border_width;
}

int NeuroClientBorderWidthSetterCurr(const ClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return NeuroConfigGet()->border_width;
  return 0;
}

// Border Gap Setters
int NeuroClientBorderGapSetterAlways(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (CLI_GET(c).is_fullscreen || CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;
  return NeuroConfigGet()->border_gap;
}

int NeuroClientBorderGapSetterNever(const ClientPtrPtr c) {
  (void)c;
  return 0;
}

int NeuroClientBorderGapSetterSmart(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (CLI_GET(c).is_fullscreen || CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;
  Rectangle *a = NeuroCoreClientGetRegion(c);
  Rectangle *as = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  if ((a->w == as->w && a->h == as->h) ||
      (a->w == NeuroSystemGetScreenRegion()->w && a->h == NeuroSystemGetScreenRegion()->h))
    return 0;
  return NeuroConfigGet()->border_gap;
}

int NeuroClientBorderGapSetterCurr(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    return 0;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (CLI_GET(c).is_fullscreen || CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull ||
      l->arranger_fn == NeuroLayoutArrangerFloat)
    return 0;
  if (!NeuroCoreClientIsCurr(c))
    return 0;
  return NeuroConfigGet()->border_gap;
}

