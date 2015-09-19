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
#include "core.h"
#include "layout.h"
#include "client.h"
#include "workspace.h"
#include "rule.h"
#include "dzen.h"
#include "action.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void do_key_press(XEvent *e) {
  assert(e);
  XKeyEvent ke = e->xkey;
  int ks;
  KeySym *keysym = XGetKeyboardMapping(display, ke.keycode, 1, &ks);
  const Key *k;
  int i;
  for (i = 0; keyBindingsS[ i ]; ++i) {
    k = keyBindingsS[ i ];
    if (k->key == *keysym && k->mod == ke.state) {
      NeuroActionUtilRunActionChain(&k->actionChain);
      NeuroDzenUpdate(True);
    }
  }
  XFree(keysym);
}

static void do_button_press(XEvent *e) {
  assert(e);
  XButtonPressedEvent *ev = &e->xbutton;
  const Button *b;
  int i;
  for (i = 0; buttonBindingsS[ i ]; ++i) {
    b = buttonBindingsS[ i ];
    if (b->button == ev->button && b->mod == ev->state) {
      NeuroActionUtilRunActionChain(&b->actionChain);
      NeuroDzenUpdate(True);
    }
  }
}

static void do_map_request(XEvent *e) {
  assert(e);
  NeuroEventManageWindow(e->xmaprequest.window);
  NeuroDzenUpdate(True);
}

static void do_destroy_notify(XEvent *e) {
  assert(e);
  Window w = e->xdestroywindow.window;
  ClientPtrPtr c = findWindowClientAllW(w);
  if (c) {
    NeuroEventUnmanageClient(c);
  } else {
    Client *cli = NeuroCoreRemoveMinimizedClient(w);
    if (cli)
      freeClientT(cli);
  }
  NeuroDzenUpdate(True);
}

static void do_unmap_notify(XEvent *e) {
  assert(e);
  Window w = e->xdestroywindow.window;
  ClientPtrPtr c = findWindowClientAllW(w);
  if (c) {
    NeuroEventUnmanageClient(c);
  } else {
    Client *cli = NeuroCoreRemoveMinimizedClient(w);
    if (cli)
      freeClientT(cli);
  }
  NeuroDzenUpdate(True);
}

static void do_enter_notify(XEvent *e) {
  assert(e);
  XCrossingEvent *ev = &e->xcrossing;
  if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != (unsigned int)root)
    return;
  int ws = NeuroCoreGetCurrStack();  // Mouse is always in the current workspace
  if (NeuroCoreStackGetSize(ws) < 2)
    return;
  if (!NeuroCoreStackGetCurrLayout(ws)->followMouse)
    return;
  ClientPtrPtr c = findWindowClientAllW(ev->window);
  if (!c)
    return;
  if (NeuroCoreClientIsCurr(c))
    return;
  focusClientW(c, NeuroClientSelectorSelf, NULL);
  NeuroDzenUpdate(True);
}

static void do_configure_request(XEvent *e) {
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
  ClientPtrPtr c = findWindowClientAllW(ev->window);
  if (c) {
    NeuroLayoutRunCurr(CLI_GET(c).ws);
    updateW(CLI_GET(c).ws);
  }
  NeuroDzenUpdate(True);
}

static void do_focus_in(XEvent *e) {
  assert(e);
  ClientPtrPtr c = NeuroClientGetFocused();
  if (!c)
    return;
  if (CLI_GET(c).win == e->xfocus.window)
    return;
  focusClientW(c, NeuroClientSelectorSelf, NULL);
  NeuroDzenUpdate(True);
}

static void do_client_message(XEvent *e) {
  assert(e);
  ClientPtrPtr c = findWindowClientAllW(e->xclient.window);
  if (!c)
    return;
  if (e->xclient.message_type == netatoms[ NET_WM_STATE ] &&
      ((unsigned)e->xclient.data.l[1] == netatoms[ NET_FULLSCREEN ]
      || (unsigned)e->xclient.data.l[2] == netatoms[ NET_FULLSCREEN ])) {
    if (e->xclient.data.l[0] == 0)  // _NET_WM_STATE_REMOVE
      NeuroClientNormal(c, NULL);
    else if (e->xclient.data.l[0] == 1)  // _NET_WM_STATE_ADD
      NeuroClientFullscreen(c, NULL);
    else if (e->xclient.data.l[0] == 2)  // _NET_WM_STATE_TOGGLE
      NeuroClientToggleFullscreen(c, NULL);
  } else if (e->xclient.message_type == netatoms[ NET_ACTIVE ]) {
    focusClientW(c, NeuroClientSelectorSelf, NULL);
  }
  NeuroDzenUpdate(True);
}

static void do_property_notify(XEvent *e) {
  assert(e);
  XPropertyEvent *ev = &e->xproperty;
  if (ev->atom == XA_WM_NAME || ev->atom == netatoms[ NET_WM_NAME ]) {  // Window title
    ClientPtrPtr c = findWindowClientAllW(ev->window);
    if (!c)
      return;
    NeuroClientUpdateTitle(c, NULL);
    NeuroDzenUpdate(True);
  } else if (ev->atom == XA_WM_HINTS) {  // Urgency hint
    ClientPtrPtr c = findWindowClientAllW(ev->window);
    if (!c)
      return;
    if (NeuroCoreClientIsCurr(c) && NeuroCoreStackIsCurr(CLI_GET(c).ws))
      return;
    XWMHints *wmh = XGetWMHints(display, CLI_GET(c).win);
    if (wmh && (wmh->flags & XUrgencyHint))
      NeuroClientSetUrgent(c, NULL);
    if (wmh)
      XFree(wmh);
    NeuroClientUpdate(c, NULL);
    NeuroDzenUpdate(True);
  }
}

//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

const EventHandler const NeuroEventEventArray[ LASTEvent ] = {
  [ KeyPress ]         = do_key_press,
  [ ButtonPress ]      = do_button_press,
  [ MapRequest ]       = do_map_request,
  [ UnmapNotify ]      = do_unmap_notify,
  [ DestroyNotify ]    = do_destroy_notify,
  [ EnterNotify ]      = do_enter_notify,
  [ ConfigureRequest ] = do_configure_request,
  [ FocusIn ]          = do_focus_in,
  [ ClientMessage ]    = do_client_message,
  [ PropertyNotify ]   = do_property_notify
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroEventManageWindow(Window w) {
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
    exitErrorS("NeuroEventManageWindow - could not alloc Client and set rules");
  ClientPtrPtr c = NeuroCoreAddClientStart(cli);
  if (!c)
    exitErrorS("NeuroEventManageWindow - could not add client");

  // Transient windows
  Window trans = None;
  if (XGetTransientForHint(display, CLI_GET(c).win, &trans)) {
    CLI_GET(c).freeSetterFn = defFreeR;
    ClientPtrPtr t = findWindowClientAllW(trans);
    if (t)  // Always true, but still
      NeuroGeometryCenterRectangleInRegion(NeuroCoreClientGetRegion(c), NeuroCoreClientGetRegion(t));
    else
      NeuroGeometryCenterRectangleInRegion(NeuroCoreClientGetRegion(c), &screenRegion);
  }

  // Set event mask
  XSelectInput(display, CLI_GET(c).win, CLIENT_MASK);

  // Map window
  Bool doRules = False;
  NeuroClientHide(c, (const void*)&doRules);
  XMapWindow(display, CLI_GET(c).win);
  grabButtonsS(CLI_GET(c).win);
  const int ws = CLI_GET(c).ws;
  if (!NeuroCoreStackIsCurr(ws))
    return;
  rmvEnterNotifyMaskW(ws);
  doRules = True;
  NeuroClientShow(c, (const void*)&doRules);
  NeuroLayoutRunCurr(ws);
  updateFocusW(ws);
  addEnterNotifyMaskW(ws);
}

void NeuroEventUnmanageClient(ClientPtrPtr c) {
  assert(c);
  const int ws = CLI_GET(c).ws;
  rmvEnterNotifyMaskW(ws);
  unapplyRuleR(c);
  Client *cli = NeuroCoreRemoveClient(c);
  freeClientT(cli);
  NeuroLayoutRunCurr(ws);
  updateFocusW(ws);
  addEnterNotifyMaskW(ws);
}

void NeuroEventLoadWindows() {
  unsigned int i, num;
  Window d1, d2, *wins = NULL;
  XWindowAttributes wa;
  if (!XQueryTree(display, root, &d1, &d2, &wins, &num))
    exitErrorS("NeuroEventLoadWindows - could not get windows");
  for (i = 0; i < num; ++i) {
    if (!XGetWindowAttributes(display, wins[ i ], &wa))
      continue;
    if (wa.map_state != IsViewable)
      continue;
    NeuroEventManageWindow(wins[ i ]);
  }
  if (wins)
    XFree(wins);
}

