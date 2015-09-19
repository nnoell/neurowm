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
  if (XGetWMProtocols(display, w, &protocols, &n)) {
    for (i = 0; !ret && i < n; i++)
      if (protocols[ i ] == wmatoms[ WM_DELETE_WINDOW ])
        ret = True;
    XFree(protocols);
  }
  return ret;
}

static Bool set_title_atom(Client *c, Atom atom) {
  assert(c);
  XTextProperty tp;
  XGetTextProperty(display, c->win, &tp, atom);
  if (!tp.nitems)
    return False;
  if (tp.encoding == XA_STRING) {
    strncpy(c->title, (char *)tp.value, NAME_MAX);
  } else {
    char **list = NULL;
    int n;
    if (XmbTextPropertyToTextList(display, &tp, &list, &n) >= Success && n > 0 && list[ 0 ]) {
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
    if (NeuroGeometryIsPointInRectangleG(NeuroCoreClientGetRegion(c), x, y))
      break;
  return c;
}

static void process_xmotion(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int px, int py,
    XMotionUpdaterFn mpf) {
  const Bool res = XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime);
  if (res != GrabSuccess)
    return;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify)
      mpf(r, ws, cx, cy, cw, ch, ev.xmotion.x, ev.xmotion.y, px, py);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

static void xmotion_move(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py) {
  (void)cw;
  (void)ch;
  r->x = cx + (ex - px);
  r->y = cy + (ey - py);
  NeuroLayoutRunCurr(ws);
  updateW(ws);
}

static void xmotion_resize(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py) {
  (void)cx;
  (void)cy;
  (void)px;
  (void)py;
  r->w = cw + (ex - (cw + r->x));
  r->h = ch + (ey - (ch + r->y));
  NeuroLayoutRunCurr(ws);
  updateW(ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroClientUpdate(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Free windows
  if (CLI_GET(c).freeSetterFn != notFreeR)
    CLI_GET(c).freeSetterFn(NeuroCoreClientGetRegion(c), &screenRegion);

  // Fullscreen windows
  Rectangle r;
  if (CLI_GET(c).isFullScreen)
    memmove(&r, &screenRegion, sizeof(Rectangle));
  else
    memmove(&r, NeuroCoreClientGetRegion(c), sizeof(Rectangle));

  // Set border width and border gap
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  const int borderWidth = l->borderWidthSetterFn(c);
  const int borderGap = l->borderGapSetterFn(c);
  NeuroGeometrySetRectangleBorderWidthAndGap(&r, borderWidth, borderGap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  // Draw
  XSetWindowBorder(display, CLI_GET(c).win, l->borderColorSetterFn(c));
  XSetWindowBorderWidth(display, CLI_GET(c).win, borderWidth);
  XMoveResizeWindow(display, CLI_GET(c).win, r.x, r.y, r.w, r.h);
}

void NeuroClientUpdateClassAndName(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).class[ 0 ] = '\0';
  CLI_GET(c).name[ 0 ] = '\0';
  XClassHint ch = (XClassHint){.res_name = NULL, .res_class = NULL};
  if (!XGetClassHint(display, CLI_GET(c).win, &ch))
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
  if (!set_title_atom(*c, netatoms[ NET_WM_NAME ]))
    set_title_atom(*c, XA_WM_NAME);
}

void NeuroClientHide(ClientPtrPtr c, const void *doRules) {  // Move off screen
  if (!c)
    return;
  if (CLI_GET(c).isHidden)
    return;
  if (*(Bool *)doRules)
    unapplyRuleR(c);
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  regc->x += xRes;
  regc->y += yRes;
  XMoveWindow(display, CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).isHidden = True;
}

void NeuroClientShow(ClientPtrPtr c, const void *doRules) {  // Move back to screen
  if (!c)
    return;
  if (!CLI_GET(c).isHidden)
    return;
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  regc->x -= xRes;
  regc->y -= yRes;
  if (*(Bool *)doRules)
    applyRuleR(c);
  XMoveWindow(display, CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).isHidden = False;
}

void NeuroClientSetUrgent(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).isUrgent = True;
}

void NeuroClientUnsetUrgent(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).isUrgent = False;
}

void NeuroClientKill(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (is_protocol_delete(CLI_GET(c).win)) {
    XEvent ke;
    ke.type = ClientMessage;
    ke.xclient.window = CLI_GET(c).win;
    ke.xclient.message_type = wmatoms[ WM_PROTOCOLS ];
    ke.xclient.format = 32;
    ke.xclient.data.l[ 0 ] = wmatoms[ WM_DELETE_WINDOW ];
    ke.xclient.data.l[ 1 ] = CurrentTime;
    XSendEvent(display, CLI_GET(c).win, False, NoEventMask, &ke);
  } else {
    XKillClient(display, CLI_GET(c).win);
    NeuroEventUnmanageClient(c);
  }
}

void NeuroClientMinimize(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  NeuroCoreSetCurrClient(NeuroCoreClientGetNext(c));
  unapplyRuleR(c);
  Client *cli = NeuroCoreRemoveClient(c);
  if (!cli)
    return;
  if (!NeuroCorePushMinimizedClient(cli))
    exitErrorS("NeuroClientMinimize - could not minimize client");
  XMoveWindow(display, cli->win, xRes + 1, yRes + 1);  // Move client off screen
  NeuroLayoutRunCurr(cli->ws);
  updateFocusW(cli->ws);
}

void NeuroClientTile(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).freeSetterFn == notFreeR)
    return;
  CLI_GET(c).freeSetterFn = notFreeR;
  applyRuleR(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void NeuroClientFree(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  const GenericArgFn *gaf = (const GenericArgFn *)freeSetterFn;
  if (CLI_GET(c).freeSetterFn == gaf->FreeSetterFn_)
    return;
  if (CLI_GET(c).freeSetterFn == notFreeR)
    unapplyRuleR(c);
  CLI_GET(c).freeSetterFn = gaf->FreeSetterFn_;
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void NeuroClientToggleFree(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  if (CLI_GET(c).freeSetterFn != notFreeR)
    NeuroClientTile(c, NULL);
  else
    NeuroClientFree(c, freeSetterFn);
}

void NeuroClientNormal(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (!CLI_GET(c).isFullScreen)
    return;
  CLI_GET(c).isFullScreen = False;
  applyRuleR(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void NeuroClientFullscreen(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).isFullScreen)
    return;
  CLI_GET(c).isFullScreen = True;
  unapplyRuleR(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void NeuroClientToggleFullscreen(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).isFullScreen)
    NeuroClientNormal(c, NULL);
  else
    NeuroClientFullscreen(c, NULL);
}

void NeuroClientFloatMove(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  focusClientW(c, NeuroClientSelectorSelf, NULL);
  Rectangle *r = &(CLI_GET(c).floatRegion);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_move);
}

void NeuroClientFloatResize(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  focusClientW(c, NeuroClientSelectorSelf, NULL);
  Rectangle *r = &(CLI_GET(c).floatRegion);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_resize);
}

void NeuroClientFreeMove(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c || !freeSetterFn)
    return;
  focusClientW(c, NeuroClientSelectorSelf, NULL);
  NeuroClientFree(c, freeSetterFn);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_move);
}

void NeuroClientFreeResize(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c || !freeSetterFn)
    return;
  focusClientW(c, NeuroClientSelectorSelf, NULL);
  NeuroClientFree(c, freeSetterFn);
  Rectangle *r = NeuroCoreClientGetRegion(c);
  int px = 0, py = 0;
  NeuroClientGetPointed(&px, &py);
  process_xmotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, xmotion_resize);
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
  if (!XQueryPointer(display, root, &rootw, &childw, x, y, &xc, &yc, &state))
    return NULL;
  return findWindowClientAllW(childw);
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
  return CLI_GET(c).isUrgent;
}

Bool NeuroClientTesterFixed(const ClientPtrPtr c, const void *data) {
  assert(c);
  (void)data;
  return CLI_GET(c).fixPos != notFixedR;
}

// Color Setters
Color NeuroClientColorSetterCurr(const ClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return currBorderColorS;
  return normBorderColorS;
}

Color NeuroClientColorSetterAll(const ClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return currBorderColorS;
  else if (CLI_GET(c).isUrgent)
    return urgtBorderColorS;
  else if (CLI_GET(c).freeSetterFn != notFreeR)
    return freeBorderColorS;
  else if (NeuroCoreClientIsPrev(c))
    return prevBorderColorS;
  return normBorderColorS;
}

Color NeuroClientColorSetterNone(const ClientPtrPtr c) {
  (void)c;
  return normBorderColorS;
}

// Border Width Setters
int NeuroClientBorderWidthSetterAlways(const ClientPtrPtr c) {
  (void)c;
  return borderWidthS;
}

int NeuroClientBorderWidthSetterNever(const ClientPtrPtr c) {
  (void)c;
  return 0;
}

int NeuroClientBorderWidthSetterSmart(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).isFullScreen)
    return 0;
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return borderWidthS;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (l->arrangerFn == NeuroLayoutArrangerFloat)
    return borderWidthS;
  if (findFixedClientW(CLI_GET(c).ws))
    return borderWidthS;
  Rectangle *rc = NeuroCoreClientGetRegion(c);
  Rectangle *rs = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  if ((rc->w == rs->w && rc->h == rs->h) || (rc->w == screenRegion.w && rc->h == screenRegion.h))
    return 0;
  return borderWidthS;
}

int NeuroClientBorderWidthSetterCurr(const ClientPtrPtr c) {
  assert(c);
  if (NeuroCoreClientIsCurr(c))
    return borderWidthS;
  return 0;
}

// Border Gap Setters
int NeuroClientBorderGapSetterAlways(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == NeuroLayoutArrangerFloat)
    return 0;
  return borderGapS;
}

int NeuroClientBorderGapSetterNever(const ClientPtrPtr c) {
  (void)c;
  return 0;
}

int NeuroClientBorderGapSetterSmart(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == NeuroLayoutArrangerFloat)
    return 0;
  Rectangle *a = NeuroCoreClientGetRegion(c);
  Rectangle *as = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  if ((a->w == as->w && a->h == as->h) || (a->w == screenRegion.w && a->h == screenRegion.h))
    return 0;
  return borderGapS;
}

int NeuroClientBorderGapSetterCurr(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = NeuroCoreStackGetCurrLayout(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == NeuroLayoutArrangerFloat)
    return 0;
  if (!NeuroCoreClientIsCurr(c))
    return 0;
  return borderGapS;
}

