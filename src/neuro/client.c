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

// MotionProcesserFn
typedef void (*XMotionUpdaterFn)(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Bool isProtocolDelete(Window w) {
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

static Bool setTitleAtom(Client *c, Atom atom) {
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

static ClientPtrPtr queryPoint(int ws, int x, int y) {
  ClientPtrPtr c;
  for (c=getHeadClientStackSS(ws); c; c=getNextClientSS(c))
    if (isPointInRectangleG(getRegionClientSS(c), x, y))
      break;
  return c;
}

static void processXMotion(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int px, int py, XMotionUpdaterFn mpf) {
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

static void XMotionMove(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py) {
  (void)cw;
  (void)ch;
  r->x = cx + (ex - px);
  r->y = cy + (ey - py);
  runCurrL(ws);
  updateW(ws);
}

static void XMotionResize(Rectangle *r, int ws, int cx, int cy, int cw, int ch, int ex, int ey, int px, int py) {
  (void)cx;
  (void)cy;
  (void)px;
  (void)py;
  r->w = cw + (ex - (cw + r->x));
  r->h = ch + (ey - (ch + r->y));
  runCurrL(ws);
  updateW(ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void updateC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Free windows
  if (CLI_GET(c).freeSetterFn != notFreeR)
    CLI_GET(c).freeSetterFn(getRegionClientSS(c), &screenRegion);

  // Fullscreen windows
  Rectangle r;
  if (CLI_GET(c).isFullScreen)
    memmove(&r, &screenRegion, sizeof(Rectangle));
  else
    memmove(&r, getRegionClientSS(c), sizeof(Rectangle));

  // Set border width and border gap
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  const int borderWidth = l->borderWidthSetterFn(c);
  const int borderGap = l->borderGapSetterFn(c);
  setRectangleBorderWidthAndGapG(&r, borderWidth, borderGap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  // Draw
  XSetWindowBorder(display, CLI_GET(c).win, l->borderColorSetterFn(c));
  XSetWindowBorderWidth(display, CLI_GET(c).win, borderWidth);
  XMoveResizeWindow(display, CLI_GET(c).win, r.x, r.y, r.w, r.h);
}

void updateClassAndNameC(ClientPtrPtr c, const void *data) {
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

void updateTitleC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).title[ 0 ] = '\0';
  if (!setTitleAtom(*c, netatoms[ NET_WM_NAME ]))
    setTitleAtom(*c, XA_WM_NAME);
}

void hideC(ClientPtrPtr c, const void *doRules) {  // Move off screen
  if (!c)
    return;
  if (CLI_GET(c).isHidden)
    return;
  if (*(Bool *)doRules)
    unapplyRuleR(c);
  Rectangle *regc = getRegionClientSS(c);
  regc->x += xRes;
  regc->y += yRes;
  XMoveWindow(display, CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).isHidden = True;
}

void showC(ClientPtrPtr c, const void *doRules) {  // Move back to screen
  if (!c)
    return;
  if (!CLI_GET(c).isHidden)
    return;
  Rectangle *regc = getRegionClientSS(c);
  regc->x -= xRes;
  regc->y -= yRes;
  if (*(Bool *)doRules)
    applyRuleR(c);
  XMoveWindow(display, CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).isHidden = False;
}

void setUrgentC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).isUrgent = True;
}

void unsetUrgentC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).isUrgent = False;
}

void killC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (isProtocolDelete(CLI_GET(c).win)) {
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
    unmanageCliE(c);
  }
}

void minimizeC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  setCurrClientSS(getNextClientSS(c));
  unapplyRuleR(c);
  Client *cli = rmvClientSS(c);
  if (!cli)
    return;
  if (!pushMinimizedClientSS(cli))
    exitErrorS("minimizeC - could not minimize client");
  XMoveWindow(display, cli->win, xRes + 1, yRes + 1);  // Move client off screen
  runCurrL(cli->ws);
  updateFocusW(cli->ws);
}

void tileC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).freeSetterFn == notFreeR)
    return;
  CLI_GET(c).freeSetterFn = notFreeR;
  applyRuleR(c);
  runCurrL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void freeC(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  const GenericArgFn *gaf = (const GenericArgFn *)freeSetterFn;
  if (CLI_GET(c).freeSetterFn == gaf->FreeSetterFn_)
    return;
  if (CLI_GET(c).freeSetterFn == notFreeR)
    unapplyRuleR(c);
  CLI_GET(c).freeSetterFn = gaf->FreeSetterFn_;
  runCurrL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void toggleFreeC(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  if (CLI_GET(c).freeSetterFn != notFreeR)
    tileC(c, NULL);
  else
    freeC(c, freeSetterFn);
}

void normalC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (!CLI_GET(c).isFullScreen)
    return;
  CLI_GET(c).isFullScreen = False;
  applyRuleR(c);
  runCurrL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void fullScreenC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).isFullScreen)
    return;
  CLI_GET(c).isFullScreen = True;
  unapplyRuleR(c);
  runCurrL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void toggleFullScreenC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).isFullScreen)
    normalC(c, NULL);
  else
    fullScreenC(c, NULL);
}

void moveC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  focusClientW(c, selfC, NULL);
  Rectangle *r = &(CLI_GET(c).floatRegion);
  int px = 0, py = 0;
  getPointerC(&px, &py);
  processXMotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, XMotionMove);
}

void resizeC(ClientPtrPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  focusClientW(c, selfC, NULL);
  Rectangle *r = &(CLI_GET(c).floatRegion);
  int px = 0, py = 0;
  getPointerC(&px, &py);
  processXMotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, XMotionResize);
}

void freeMoveC(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c || !freeSetterFn)
    return;
  focusClientW(c, selfC, NULL);
  freeC(c, freeSetterFn);
  Rectangle *r = getRegionClientSS(c);
  int px = 0, py = 0;
  getPointerC(&px, &py);
  processXMotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, XMotionMove);
}

void freeResizeC(ClientPtrPtr c, const void *freeSetterFn) {
  if (!c || !freeSetterFn)
    return;
  focusClientW(c, selfC, NULL);
  freeC(c, freeSetterFn);
  Rectangle *r = getRegionClientSS(c);
  int px = 0, py = 0;
  getPointerC(&px, &py);
  processXMotion(r, CLI_GET(c).ws, r->x, r->y, r->w, r->h, px, py, XMotionResize);
}


// Client getters
ClientPtrPtr getFocusedC() {
  return getCurrClientStackSS(getCurrStackSS());
}

ClientPtrPtr getPointerC(int *x, int *y) {
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
ClientPtrPtr selfC(const ClientPtrPtr c) {
  assert(c);
  return c;
}

ClientPtrPtr nextC(const ClientPtrPtr c) {
  assert(c);
  ClientPtrPtr n = getNextClientSS(c);
  if (!n)
    n = getHeadClientStackSS(CLI_GET(c).ws);
  return n;
}

ClientPtrPtr prevC(const ClientPtrPtr c) {
  assert(c);
  ClientPtrPtr p = getPrevClientSS(c);
  if (!p)
    p = getLastClientStackSS(CLI_GET(c).ws);
  return p;
}

ClientPtrPtr oldC(const ClientPtrPtr c) {
  assert(c);
  return getPrevClientStackSS(CLI_GET(c).ws);
}

ClientPtrPtr headC(const ClientPtrPtr c) {
  assert(c);
  return getHeadClientStackSS(CLI_GET(c).ws);
}

ClientPtrPtr lastC(const ClientPtrPtr c) {
  assert(c);
  return getLastClientStackSS(CLI_GET(c).ws);
}

ClientPtrPtr upC(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPoint(CLI_GET(c).ws, r->x+1, r->y-1);
}

ClientPtrPtr downC(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPoint(CLI_GET(c).ws, r->x+1, r->y + r->h + 1);
}

ClientPtrPtr leftC(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPoint(CLI_GET(c).ws, r->x-1, r->y+1);
}

ClientPtrPtr rightC(const ClientPtrPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPoint(CLI_GET(c).ws, r->x + r->w + 1, r->y+1);
}

// Client Testers
Bool testWindowC(const ClientPtrPtr c, const void *w) {
  assert(c);
  assert(w);
  return CLI_GET(c).win == *((Window *)w);
}

Bool testIsUrgentC(const ClientPtrPtr c, const void *p) {
  assert(c);
  (void)p;
  return CLI_GET(c).isUrgent;
}

Bool testIsFixedC(const ClientPtrPtr c, const void *p) {
  assert(c);
  (void)p;
  return CLI_GET(c).fixPos != notFixedR;
}

// Color Setters
Color onlyCurrBorderColorC(const ClientPtrPtr c) {
  assert(c);
  if (isCurrClientSS(c))
    return currBorderColorS;
  return normBorderColorS;
}

Color allBorderColorC(const ClientPtrPtr c) {
  assert(c);
  if (isCurrClientSS(c))
    return currBorderColorS;
  else if (CLI_GET(c).isUrgent)
    return urgtBorderColorS;
  else if (CLI_GET(c).freeSetterFn != notFreeR)
    return freeBorderColorS;
  else if (isPrevClientSS(c))
    return prevBorderColorS;
  return normBorderColorS;
}

Color noBorderColorC(const ClientPtrPtr c) {
  (void)c;
  return normBorderColorS;
}


// Border Width Setters
int alwaysBorderWidthC(const ClientPtrPtr c) {
  (void)c;
  return borderWidthS;
}

int neverBorderWidthC(const ClientPtrPtr c) {
  (void)c;
  return 0;
}

int smartBorderWidthC(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).isFullScreen)
    return 0;
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return borderWidthS;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (l->arrangerFn == floatArrangerL)
    return borderWidthS;
  if (findFixedClientW(CLI_GET(c).ws))
    return borderWidthS;
  Rectangle *rc = getRegionClientSS(c);
  Rectangle *rs = getRegionStackSS(CLI_GET(c).ws);
  if ((rc->w == rs->w && rc->h == rs->h) || (rc->w == screenRegion.w && rc->h == screenRegion.h))
    return 0;
  return borderWidthS;
}

int onlyCurrBorderWidthC(const ClientPtrPtr c) {
  assert(c);
  if (isCurrClientSS(c))
    return borderWidthS;
  return 0;
}


// Border Gap Setters
int alwaysBorderGapC(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == floatArrangerL)
    return 0;
  return borderGapS;
}

int neverBorderGapC(const ClientPtrPtr c) {
  (void)c;
  return 0;
}

int smartBorderGapC(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == floatArrangerL)
    return 0;
  Rectangle *a = getRegionClientSS(c);
  Rectangle *as = getRegionStackSS(CLI_GET(c).ws);
  if ((a->w == as->w && a->h == as->h) || (a->w == screenRegion.w && a->h == screenRegion.h))
    return 0;
  return borderGapS;
}

int onlyCurrBorderGapC(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == floatArrangerL)
    return 0;
  if (!isCurrClientSS(c))
    return 0;
  return borderGapS;
}

