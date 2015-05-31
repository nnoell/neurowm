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
#include "area.h"
#include "base.h"
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
  XKeyEvent ke = e->xkey;
  int ks;
  KeySym *keysym = XGetKeyboardMapping(display, ke.keycode, 1, &ks);
  const Key *k;
  int i;
  for (i = 0; keyBindingsB[ i ]; ++i) {
    k = keyBindingsB[ i ];
    if (k->keySym == *keysym && k->mod == ke.state) {
      k->handler(k->arg);
      updateDP(True);
    }
  }
  XFree(keysym);
}

static void buttonPressE(XEvent *e) {
  XButtonPressedEvent *ev = &e->xbutton;
  const Button *b;
  int i;
  for (i = 0; buttonBindingsB[ i ]; ++i) {
    b = buttonBindingsB[ i ];
    if (b->handler && b->button == ev->button && b->mod == ev->state) {
      b->handler(b->arg);
      updateDP(True);
    }
  }
}

static void mapRequestE(XEvent *e) {
  manageWindowE(e->xmaprequest.window);
  updateDP(True);
}

static void destroyNotifyE(XEvent *e) {
  Window w = e->xdestroywindow.window;
  CliPtr c = findWindowClientAllW(w);
  if (c) {
    unmanageCliE(c);
  } else {
    Client *cli = rmvMinimizedCliSS(w);
    if (!cli)
      return;
    freeClientG(cli);
    XSync(display, True);
  }
  updateDP(True);
}

static void unmapNotifyE(XEvent *e) {
  Window w = e->xdestroywindow.window;
  CliPtr c = findWindowClientAllW(w);
  if (c) {
    unmanageCliE(c);
  } else {
    Client *cli = rmvMinimizedCliSS(w);
    if (!cli)
      return;
    freeClientG(cli);
    XSync(display, True);
  }
  updateDP(True);
}

static void enterNotifyE(XEvent *e) {
  XCrossingEvent *ev = &e->xcrossing;
  if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != (unsigned int)root)
    return;
  int ws = getCurrStackSS();  // Mouse is always in the current workspace
  if (getSizeStackSS( ws ) < 2)
    return;
  if (!getCurrLayoutStackSS( ws )->followMouse)
    return;
  CliPtr c = findWindowClientAllW(ev->window);
  if (!c)
    return;
  if (isCurrCliSS( c ))
    return;
  moveFocusW(c, selfC);
  XSync(display, True);
  updateDP(True);
}

static void configureRequestE(XEvent *e) {
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
  CliPtr c = getCurrCliCurrStackSS();
  if (!c)
    return;
  if (CLIVAL(c).win == e->xfocus.window)
    return;
  moveFocusW(c, selfC);
  updateDP(True);
}

static void clientMessageE(XEvent *e) {
  CliPtr c = findWindowClientAllW(e->xclient.window);
  if (!c)
    return;
  if (e->xclient.message_type == netatoms[ NET_WM_STATE ] &&
      ((unsigned)e->xclient.data.l[1] == netatoms[ NET_FULLSCREEN ]
      || (unsigned)e->xclient.data.l[2] == netatoms[ NET_FULLSCREEN ])) {
    if (e->xclient.data.l[0] == 0)  // _NET_WM_STATE_REMOVE
      normalC(c);
    else if (e->xclient.data.l[0] == 1)  // _NET_WM_STATE_ADD
      fullScreenC(c);
    else if (e->xclient.data.l[0] == 2)  // _NET_WM_STATE_TOGGLE
      toggleFullScreenC(c);
  } else if (e->xclient.message_type == netatoms[ NET_ACTIVE ]) {
    moveFocusW(c, selfC);
  }
  updateDP(True);
}

static void propertyNotifyE(XEvent *e) {
  XPropertyEvent *ev = &e->xproperty;
  if (ev->atom == XA_WM_NAME || ev->atom == netatoms[ NET_WM_NAME ]) {  // Window title
    CliPtr c = findWindowClientAllW(ev->window);
    if (!c)
      return;
    updateTitleC(c);
    updateDP(True);
  } else if (ev->atom == XA_WM_HINTS) {  // Urgency hint
    CliPtr c = findWindowClientAllW(ev->window);
    if (!c)
      return;
    if (isCurrCliSS(c) && isCurrStackSS(CLIVAL(c).ws))
      return;
    XWMHints *wmh = XGetWMHints(display, CLIVAL(c).win);
    if (wmh && (wmh->flags & XUrgencyHint))
      setUrgentC(c);
    if (wmh)
      XFree(wmh);
    updateC(c);
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
    exitErrorG("manageWindowE - could not alloc Client and set rules");
  CliPtr c = addCliStartSS(cli);
  if (!c)
    exitErrorG("manageWindowE - could not add client");

  // Transient windows
  Window trans = None;
  if (XGetTransientForHint(display, CLIVAL(c).win, &trans)) {
    CLIVAL(c).freeLocFunc = defFreeR;
    CliPtr t = findWindowClientAllW(trans);
    if (t)  // Always true, but still
      centerAreaInRegA(getRegionCliSS(c), getRegionCliSS(t));
    else
      centerAreaInRegA(getRegionCliSS(c), &screenArea);
  }

  // Map window
  hideC(c, False);
  XSelectInput(display, CLIVAL(c).win, CLIENT_MASK);
  XMapWindow(display, CLIVAL(c).win);
  grabButtonsB(CLIVAL(c).win);
  if (isCurrStackSS(CLIVAL(c).ws)) {
    showC(c, True);
    runCurrLayoutL(CLIVAL(c).ws);
    updateFocusW(CLIVAL(c).ws);
  }

  XSync(display, True);
}

void unmanageCliE(CliPtr c) {
  int ws = CLIVAL(c).ws;
  Client *cli = rmvCliSS(c);
  freeClientG(cli);
  runCurrLayoutL(ws);
  updateFocusW(ws);
  XSync(display, True);
}

void loadWindowsE() {
  unsigned int i, num;
  Window d1, d2, *wins = NULL;
  XWindowAttributes wa;
  if (!XQueryTree(display, root, &d1, &d2, &wins, &num))
    exitErrorG("loadWindowsE - could not get windows");
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

