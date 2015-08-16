//----------------------------------------------------------------------------------------------------------------------
// Module      :  event
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "event.h"
#include "geometry.h"
#include "system.h"
#include "stackset.h"
#include "layout.h"
#include "client.h"
#include "workspace.h"
#include "rule.h"
#include "dzenpanel.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void keyPressE(XEvent *e) {
  assert(e);
  XKeyEvent ke = e->xkey;
  int ks;
  KeySym *keysym = XGetKeyboardMapping(display, ke.keycode, 1, &ks);
  const Key *k;
  int i;
  for (i = 0; keyBindingsS[ i ]; ++i) {
    k = keyBindingsS[ i ];
    if (k->key == *keysym && k->mod == ke.state) {
      k->action.func(k->action.arg);
      updateDP(True);
    }
  }
  XFree(keysym);
}

static void buttonPressE(XEvent *e) {
  assert(e);
  XButtonPressedEvent *ev = &e->xbutton;
  const Button *b;
  int i;
  for (i = 0; buttonBindingsS[ i ]; ++i) {
    b = buttonBindingsS[ i ];
    if (b->action.func && b->button == ev->button && b->mod == ev->state) {
      b->action.func(b->action.arg);
      updateDP(True);
    }
  }
}

static void mapRequestE(XEvent *e) {
  assert(e);
  manageWindowE(e->xmaprequest.window);
  updateDP(True);
}

static void destroyNotifyE(XEvent *e) {
  assert(e);
  Window w = e->xdestroywindow.window;
  CliPtr c = findWindowClientAllW(w);
  if (c) {
    unmanageCliE(c);
  } else {
    Client *cli = rmvMinimizedClientSS(w);
    if (cli)
      freeClientT(cli);
  }
  updateDP(True);
}

static void unmapNotifyE(XEvent *e) {
  assert(e);
  Window w = e->xdestroywindow.window;
  CliPtr c = findWindowClientAllW(w);
  if (c) {
    unmanageCliE(c);
  } else {
    Client *cli = rmvMinimizedClientSS(w);
    if (cli)
      freeClientT(cli);
  }
  updateDP(True);
}

static void enterNotifyE(XEvent *e) {
  assert(e);
  XCrossingEvent *ev = &e->xcrossing;
  if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != (unsigned int)root)
    return;
  int ws = getCurrStackSS();  // Mouse is always in the current workspace
  if (getSizeStackSS(ws) < 2)
    return;
  if (!getCurrLayoutStackSS(ws)->followMouse)
    return;
  CliPtr c = findWindowClientAllW(ev->window);
  if (!c)
    return;
  if (isCurrClientSS(c))
    return;
  moveFocusClientW(c, selfC, NULL);
  updateDP(True);
}

static void configureRequestE(XEvent *e) {
  assert(e);
  XConfigureRequestEvent *ev = &e->xconfigurerequest;
  XWindowChanges wc;
  wc.x = ev->x;
  wc.y = ev->y;
  wc.width = ev->width;
  wc.height = ev->height;
  wc.sibling = ev->above;
  wc.stack_mode = ev->detail;
  XConfigureWindow(display, ev->window, ev->value_mask, &wc);
  CliPtr c = findWindowClientAllW(ev->window);
  if (c) {
    runCurrLayoutL(CLIVAL(c).ws);
    updateW(CLIVAL(c).ws);
  }
  updateDP(True);
}

static void focusInE(XEvent *e) {
  assert(e);
  CliPtr c = getCurrClientCurrStackSS();
  if (!c)
    return;
  if (CLIVAL(c).win == e->xfocus.window)
    return;
  moveFocusClientW(c, selfC, NULL);
  updateDP(True);
}

static void clientMessageE(XEvent *e) {
  assert(e);
  CliPtr c = findWindowClientAllW(e->xclient.window);
  if (!c)
    return;
  if (e->xclient.message_type == netatoms[ NET_WM_STATE ] &&
      ((unsigned)e->xclient.data.l[1] == netatoms[ NET_FULLSCREEN ]
      || (unsigned)e->xclient.data.l[2] == netatoms[ NET_FULLSCREEN ])) {
    if (e->xclient.data.l[0] == 0)  // _NET_WM_STATE_REMOVE
      normalC(c, NULL);
    else if (e->xclient.data.l[0] == 1)  // _NET_WM_STATE_ADD
      fullScreenC(c, NULL);
    else if (e->xclient.data.l[0] == 2)  // _NET_WM_STATE_TOGGLE
      toggleFullScreenC(c, NULL);
  } else if (e->xclient.message_type == netatoms[ NET_ACTIVE ]) {
    moveFocusClientW(c, selfC, NULL);
  }
  updateDP(True);
}

static void propertyNotifyE(XEvent *e) {
  assert(e);
  XPropertyEvent *ev = &e->xproperty;
  if (ev->atom == XA_WM_NAME || ev->atom == netatoms[ NET_WM_NAME ]) {  // Window title
    CliPtr c = findWindowClientAllW(ev->window);
    if (!c)
      return;
    updateTitleC(c, NULL);
    updateDP(True);
  } else if (ev->atom == XA_WM_HINTS) {  // Urgency hint
    CliPtr c = findWindowClientAllW(ev->window);
    if (!c)
      return;
    if (isCurrClientSS(c) && isCurrStackSS(CLIVAL(c).ws))
      return;
    XWMHints *wmh = XGetWMHints(display, CLIVAL(c).win);
    if (wmh && (wmh->flags & XUrgencyHint))
      setUrgentC(c, NULL);
    if (wmh)
      XFree(wmh);
    updateC(c, NULL);
    updateDP(True);
  }
}

//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

EventHandler eventsE[ LASTEvent ] = {
  [ KeyPress ]         = keyPressE,
  [ ButtonPress ]      = buttonPressE,
  [ MapRequest ]       = mapRequestE,
  [ UnmapNotify ]      = unmapNotifyE,
  [ DestroyNotify ]    = destroyNotifyE,
  [ EnterNotify ]      = enterNotifyE,
  [ ConfigureRequest ] = configureRequestE,
  [ FocusIn ]          = focusInE,
  [ ClientMessage ]    = clientMessageE,
  [ PropertyNotify ]   = propertyNotifyE
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void manageWindowE(Window w) {
  // Check if window is valid
  XWindowAttributes wa;
  if (!XGetWindowAttributes(display, w, &wa))
    return;
  if (wa.override_redirect)
    return;
  if (findWindowClientAllW(w))
    return;

  // Add client to the stackset
  Client *cli = allocCliAndSetRulesR(w, &wa);
  if (!cli)
    exitErrorS("manageWindowE - could not alloc Client and set rules");
  CliPtr c = addClientStartSS(cli);
  if (!c)
    exitErrorS("manageWindowE - could not add client");

  // Transient windows
  Window trans = None;
  if (XGetTransientForHint(display, CLIVAL(c).win, &trans)) {
    CLIVAL(c).freeLocFn = defFreeR;
    CliPtr t = findWindowClientAllW(trans);
    if (t)  // Always true, but still
      centerRectangleInRegionG(getRegionClientSS(c), getRegionClientSS(t));
    else
      centerRectangleInRegionG(getRegionClientSS(c), &screenRegion);
  }

  // Set event mask
  XSelectInput(display, CLIVAL(c).win, CLIENT_MASK);

  // Map window
  Bool doRules = False;
  hideC(c, (const void*)&doRules);
  XMapWindow(display, CLIVAL(c).win);
  grabButtonsS(CLIVAL(c).win);
  const int ws = CLIVAL(c).ws;
  if (!isCurrStackSS(ws))
    return;
  rmvEnterNotifyMaskW(ws);
  doRules = True;
  showC(c, (const void*)&doRules);
  runCurrLayoutL(ws);
  updateFocusW(ws);
  addEnterNotifyMaskW(ws);
}

void unmanageCliE(CliPtr c) {
  assert(c);
  const int ws = CLIVAL(c).ws;
  rmvEnterNotifyMaskW(ws);
  unapplyRuleR(c);
  Client *cli = rmvClientSS(c);
  freeClientT(cli);
  runCurrLayoutL(ws);
  updateFocusW(ws);
  addEnterNotifyMaskW(ws);
}

void loadWindowsE() {
  unsigned int i, num;
  Window d1, d2, *wins = NULL;
  XWindowAttributes wa;
  if (!XQueryTree(display, root, &d1, &d2, &wins, &num))
    exitErrorS("loadWindowsE - could not get windows");
  for (i = 0; i < num; ++i) {
    if (!XGetWindowAttributes(display, wins[ i ], &wa))
      continue;
    if (wa.map_state != IsViewable)
      continue;
    manageWindowE(wins[ i ]);
  }
  if (wins)
    XFree(wins);
}

