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
#include "area.h"
#include "base.h"
#include "stackset.h"
#include "layout.h"
#include "rule.h"
#include "workspace.h"
#include "event.h"


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

static Bool setTitleAtomC(Client *c, Atom atom) {
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

static CliPtr queryPointerC(int *x, int *y) {
  Window rootw, childw;
  int xc, yc;
  unsigned state;
  if (!XQueryPointer(display, root, &rootw, &childw, x, y, &xc, &yc, &state))
    return NULL;
  return findWindowClientAllW(childw);
}

static CliPtr queryPointC(int ws, int x, int y) {
  CliPtr c;
  for (c=getHeadCliStackSS(ws); c; c=getNextCliSS(c))
    if (isPointInArea(getRegionCliSS(c), x, y))
      break;
  return c;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void updateC(const CliPtr c) {
  if (!c)
    return;

  // Free windows
  if (CLIVAL(c).freeLocFunc != notFreeR)
    CLIVAL(c).freeLocFunc(getRegionCliSS(c), &screenArea);

  // Fullscreen windows
  Area r;
  if (CLIVAL(c).isFullScreen)
    memmove(&r, &screenArea, sizeof(Area));
  else
    memmove(&r, getRegionCliSS(c), sizeof(Area));

  Layout *l = getCurrLayoutStackSS(CLIVAL(c).ws);
  int borderWidth = l->borderWidthFunc(c);
  int borderGap = l->borderGapFunc(c);
  if ((!CLIVAL(c).isFullScreen && !CLIVAL(c).freeLocFunc && l->arrangeFunc != floatArrL) || CLIVAL(c).isNSP)
    setAreaBorderAndSpaceA(&r, borderWidth, borderGap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  XSetWindowBorder(display, CLIVAL(c).win, l->borderColorFunc(c));
  XSetWindowBorderWidth(display, CLIVAL(c).win, borderWidth);
  XMoveResizeWindow(display, CLIVAL(c).win, r.x, r.y, r.w, r.h);
}

void killC(const CliPtr c) {
  if (!c)
    return;
  if (isProtocolDelete(CLIVAL(c).win)) {
    XEvent ke;
    ke.type = ClientMessage;
    ke.xclient.window = CLIVAL(c).win;
    ke.xclient.message_type = wmatoms[ WM_PROTOCOLS ];
    ke.xclient.format = 32;
    ke.xclient.data.l[ 0 ] = wmatoms[ WM_DELETE_WINDOW ];
    ke.xclient.data.l[ 1 ] = CurrentTime;
    XSendEvent(display, CLIVAL(c).win, False, NoEventMask, &ke);
  } else {
    XKillClient(display, CLIVAL(c).win);
    unmanageCliE(c);
  }
}

void updateClassAndNameC(CliPtr c) {
  if (!c)
    return;
  CLIVAL(c).class[ 0 ] = '\0';
  CLIVAL(c).name[ 0 ] = '\0';
  XClassHint ch = (XClassHint){.res_name = NULL, .res_class = NULL};
  if (!XGetClassHint(display, CLIVAL(c).win, &ch))
    return;
  strncpy(CLIVAL(c).class, ch.res_class, NAME_MAX);
  strncpy(CLIVAL(c).name, ch.res_name, NAME_MAX);
  if (ch.res_class)
    XFree(ch.res_class);
  if (ch.res_name)
    XFree(ch.res_name);
}

void updateTitleC(CliPtr c) {
  if (!c)
    return;
  CLIVAL(c).title[ 0 ] = '\0';
  if (!setTitleAtomC(CLI(c), netatoms[ NET_WM_NAME ]))
    setTitleAtomC(CLI(c), XA_WM_NAME);
}

void hideC(CliPtr c, Bool doRules) {  // Move off screen
  if (!c)
    return;
  if (CLIVAL(c).isHidden)
    return;
  if (doRules)
    unapplyRuleR(c);
  Area *regc = getRegionCliSS(c);
  regc->x += xRes;
  regc->y += yRes;
  XMoveWindow(display, CLIVAL(c).win, regc->x, regc->y);
  CLIVAL(c).isHidden = True;
}

void showC(CliPtr c, Bool doRules) {  // Move back to screen
  if (!c)
    return;
  if (!CLIVAL(c).isHidden)
    return;
  Area *regc = getRegionCliSS(c);
  regc->x -= xRes;
  regc->y -= yRes;
  if (doRules)
    applyRuleR(c);
  XMoveWindow(display, CLIVAL(c).win, regc->x, regc->y);
  CLIVAL(c).isHidden = False;
}

void setUrgentC(CliPtr c) {
  CLIVAL(c).isUrgent = True;
}

void unsetUrgentC(CliPtr c) {
  CLIVAL(c).isUrgent = False;
}

void minimizeC(CliPtr c) {
  if (!c)
    return;
  setCurrCliSS(getNextCliSS(c));
  unapplyRuleR(c);
  Client *cli = rmvCliSS(c);
  if (!cli)
    return;
  pushMinimizedCliSS(cli);
  XMoveWindow(display, cli->win, xRes + 1, yRes + 1);  // Move client off screen
  runCurrLayoutL(cli->ws);
  updateFocusW(cli->ws);
}

void tileC(CliPtr c) {
  if (!c)
    return;
  if (!CLIVAL(c).freeLocFunc)
    return;
  CLIVAL(c).freeLocFunc = notFreeR;
  applyRuleR(c);
  runCurrLayoutL(CLIVAL(c).ws);
  updateFocusW(CLIVAL(c).ws);
}

void freeC(CliPtr c, const FreeLocF ff) {
  if (!c)
    return;
  if (CLIVAL(c).freeLocFunc == ff)
    return;
  CLIVAL(c).freeLocFunc = ff;
  unapplyRuleR(c);
  runCurrLayoutL(CLIVAL(c).ws);
  updateFocusW(CLIVAL(c).ws);
}

void toggleFreeC(CliPtr c, const FreeLocF ff) {
  if (!c)
    return;
  if (CLIVAL(c).freeLocFunc)
    tileC(c);
  else
    freeC(c, ff);
}

void normalC(CliPtr c) {
  if (!c)
    return;
  if (!CLIVAL(c).isFullScreen)
    return;
  CLIVAL(c).isFullScreen = False;
  applyRuleR(c);
  runCurrLayoutL(CLIVAL(c).ws);
  updateFocusW(CLIVAL(c).ws);
}

void fullScreenC(CliPtr c) {
  if (!c)
    return;
  if (CLIVAL(c).isFullScreen)
    return;
  CLIVAL(c).isFullScreen = True;
  unapplyRuleR(c);
  runCurrLayoutL(CLIVAL(c).ws);
  updateFocusW(CLIVAL(c).ws);
}

void toggleFullScreenC(CliPtr c) {
  if (!c)
    return;
  if (CLIVAL(c).isFullScreen)
    normalC(c);
  else
    fullScreenC(c);
}

void movePointerC() {
  int rx, ry;
  CliPtr c = queryPointerC(&rx, &ry);
  if (!c)
    return;
  moveFocusW(c, selfC);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime) != GrabSuccess)
    return;

  Area *r = &(CLIVAL(c).floatRegion);
  int cx = r->x, cy = r->y;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->x = cx + (ev.xmotion.x - rx);
      r->y = cy + (ev.xmotion.y - ry);
      runCurrLayoutL(CLIVAL(c).ws);
      updateW(CLIVAL(c).ws);
    }
    XRaiseWindow(display, CLIVAL(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

void resizePointerC() {
  int rx, ry;
  CliPtr c = queryPointerC(&rx, &ry);
  if (!c)
    return;
  moveFocusW(c, selfC);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime) != GrabSuccess)
    return;

  Area *r = &(CLIVAL(c).floatRegion);
  XWarpPointer(display, None, CLIVAL(c).win, 0, 0, 0, 0, r->w, r->h);
  int cw = r->w, ch = r->h;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->w = cw + (ev.xmotion.x - (cw + r->x));
      r->h = ch + (ev.xmotion.y - (ch + r->y));
      runCurrLayoutL(CLIVAL(c).ws);
      updateW(CLIVAL(c).ws);
    }
    XRaiseWindow(display, CLIVAL(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

void freeMovePointerC() {
  int rx, ry;
  CliPtr c = queryPointerC(&rx, &ry);
  if (!c)
    return;
  if (!CLIVAL(c).freeLocFunc)
    unapplyRuleR(c);
  CLIVAL(c).freeLocFunc = defFreeR;
  runCurrLayoutL(CLIVAL(c).ws);
  moveFocusW(c, selfC);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime) != GrabSuccess)
    return;

  Area *r = getRegionCliSS(c);
  int cx = r->x, cy = r->y;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->x = cx + (ev.xmotion.x - rx);
      r->y = cy + (ev.xmotion.y - ry);
      updateW(CLIVAL(c).ws);
    }
    XRaiseWindow(display, CLIVAL(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

void freeResizePointerC() {
  int rx, ry;
  CliPtr c = queryPointerC(&rx, &ry);
  if (!c)
    return;
  if (!CLIVAL(c).freeLocFunc)
    unapplyRuleR(c);
  CLIVAL(c).freeLocFunc = defFreeR;
  runCurrLayoutL(CLIVAL(c).ws);
  moveFocusW(c, selfC);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurResize ], CurrentTime) != GrabSuccess)
    return;

  Area *r = getRegionCliSS(c);
  XWarpPointer(display, None, CLIVAL(c).win, 0, 0, 0, 0, r->w, r->h);
  int cw = r->w, ch = r->h;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->w = cw + (ev.xmotion.x - (cw + r->x));
      r->h = ch + (ev.xmotion.y - (ch + r->y));
      updateW(CLIVAL(c).ws);
    }
    XRaiseWindow(display, CLIVAL(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

// Select Functions
CliPtr selfC(const CliPtr c) {
  return c;
}

CliPtr nextC(const CliPtr c) {
  CliPtr n = getNextCliSS(c);
  if (!n)
    n = getHeadCliStackSS(CLIVAL(c).ws);
  return n;
}

CliPtr prevC(const CliPtr c) {
  CliPtr p = getPrevCliSS(c);
  if (!p)
    p = getLastCliStackSS(CLIVAL(c).ws);
  return p;
}

CliPtr oldC(const CliPtr c) {
  return getPrevCliStackSS(CLIVAL(c).ws);
}

CliPtr headC(const CliPtr c) {
  return getHeadCliStackSS(CLIVAL(c).ws);
}

CliPtr lastC(const CliPtr c) {
  return getLastCliStackSS(CLIVAL(c).ws);
}

CliPtr upC(const CliPtr c) {
  Area *r = getRegionCliSS(c);
  return queryPointC(CLIVAL(c).ws, r->x+1, r->y-1);
}

CliPtr downC(const CliPtr c) {
  Area *r = getRegionCliSS(c);
  return queryPointC(CLIVAL(c).ws, r->x+1, r->y + r->h + 1);
}

CliPtr leftC(const CliPtr c) {
  Area *r = getRegionCliSS(c);
  return queryPointC(CLIVAL(c).ws, r->x-1, r->y+1);
}

CliPtr rightC(const CliPtr c) {
  Area *r = getRegionCliSS(c);
  return queryPointC(CLIVAL(c).ws, r->x + r->w + 1, r->y+1);
}

CliPtr pointerC(const CliPtr c) {
  (void)c;
  int x, y;
  return queryPointerC(&x, &y);
}

// Test functions
Bool testWindowC(const CliPtr c, const void *w) {
  return CLIVAL(c).win == *((Window *)w);
}

Bool testIsUrgentC(const CliPtr c, const void *p) {
  (void)p;
  return CLIVAL(c).isUrgent;
}

Bool testIsFixedC(const CliPtr c, const void *p) {
  (void)p;
  return CLIVAL(c).fixPos != notFixedR;
}

// Border Color
Color onlyCurrBorderColorC(const CliPtr c) {
  if (isCurrCliSS(c))
    return currBorderColorB;
  return normBorderColorB;
}

Color allBorderColorC(const CliPtr c) {
  if (isCurrCliSS(c))
    return currBorderColorB;
  else if (CLIVAL(c).isUrgent)
    return urgtBorderColorB;
  else if (CLIVAL(c).freeLocFunc)
    return freeBorderColorB;
  else if (isPrevCliSS(c))
    return prevBorderColorB;
  return normBorderColorB;
}

Color noBorderColorC(const CliPtr c) {
  (void)c;
  return normBorderColorB;
}

// Border Width
int alwaysBorderWidthC(const CliPtr c) {
  (void)c;
  return borderWidthB;
}

int smartBorderWidthC(const CliPtr c) {
  if (CLIVAL(c).isFullScreen)
    return 0;
  if (CLIVAL(c).freeLocFunc)
    return borderWidthB;
  Layout *l = getCurrLayoutStackSS(CLIVAL(c).ws);
  if (l->arrangeFunc == floatArrL)
    return borderWidthB;
  if (findFixedClientW(CLIVAL(c).ws))
    return borderWidthB;
  Area *a = getRegionCliSS(c);
  Area *as = getRegionStackSS(CLIVAL(c).ws);
  if (a->w == as->w && a->h == as->h)
    return 0;
  return borderWidthB;
}

int onlyCurrBorderWidthC(const CliPtr c) {
  if (isCurrCliSS(c))
    return borderWidthB;
  return 0;
}

// Border Gap
int alwaysBorderGapC(const CliPtr c) {
  (void)c;
  return borderGapB;
}

int smartBorderGapC(const CliPtr c) {
  Area *a = getRegionCliSS(c);
  Area *as = getRegionStackSS(CLIVAL(c).ws);
  if (a->w == as->w && a->h == as->h)
    return 0;
  return borderGapB;
}

int onlyCurrBorderGapC(const CliPtr c) {
  if (isCurrCliSS(c))
    return borderGapB;
  return 0;
}

