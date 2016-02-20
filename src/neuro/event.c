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
#include "config.h"
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
#include "monitor.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void do_key_press(XEvent *e) {
  assert(e);
  const NeuroKey *const *const key_list = NeuroConfigGet()->key_list;
  if (!key_list)
    return;
  XKeyEvent ke = e->xkey;
  int ks;
  KeySym *keysym = XGetKeyboardMapping(NeuroSystemGetDisplay(), ke.keycode, 1, &ks);
  const NeuroKey *k;
  for (size_t i = 0U; key_list[ i ]; ++i) {
    k = key_list[ i ];
    if (k->key == *keysym && k->mod == ke.state) {
      NeuroActionRunActionChain(&k->action_chain);
      NeuroDzenRefresh(true);
    }
  }
  XFree(keysym);
}

static void do_button_press(XEvent *e) {
  assert(e);
  const NeuroButton *const *const button_list = NeuroConfigGet()->button_list;
  if (!button_list)
    return;
  XButtonPressedEvent *ev = &e->xbutton;
  const NeuroButton *b;
  for (size_t i = 0U; button_list[ i ]; ++i) {
    b = button_list[ i ];
    if (b->button == ev->button && b->mod == ev->state) {
      NeuroActionRunActionChain(&b->action_chain);
      NeuroDzenRefresh(true);
    }
  }
}

static void do_map_request(XEvent *e) {
  assert(e);
  NeuroEventManageWindow(e->xmaprequest.window);
  NeuroDzenRefresh(true);
}

static void do_destroy_notify(XEvent *e) {
  assert(e);
  Window w = e->xdestroywindow.window;
  NeuroClientPtrPtr c = NeuroClientFindWindow(w);
  if (c) {
    NeuroEventUnmanageClient(c);
  } else {
    NeuroClient *cli = NeuroCoreRemoveMinimizedClient(w);
    NeuroTypeDeleteClient(cli);
  }
  NeuroDzenRefresh(true);
}

static void do_unmap_notify(XEvent *e) {
  assert(e);
  Window w = e->xdestroywindow.window;
  NeuroClientPtrPtr c = NeuroClientFindWindow(w);
  if (c) {
    NeuroEventUnmanageClient(c);
  } else {
    NeuroClient *cli = NeuroCoreRemoveMinimizedClient(w);
    NeuroTypeDeleteClient(cli);
  }
  NeuroDzenRefresh(true);
}

static void do_enter_notify(XEvent *e) {
  assert(e);
  XCrossingEvent *ev = &e->xcrossing;
  if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != NeuroSystemGetRoot())
    return;
  NeuroClientPtrPtr c = NeuroClientFindWindow(ev->window);
  if (!c)
    return;
  size_t ws = CLI_GET(c).ws;
  if (!NeuroCoreStackGetCurrLayout(ws)->follow_mouse)
    return;
  NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
  NeuroCoreSetCurrStack(ws);
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroDzenRefresh(true);
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
  XConfigureWindow(NeuroSystemGetDisplay(), ev->window, ev->value_mask, &wc);
  NeuroClientPtrPtr c = NeuroClientFindWindow(ev->window);
  if (c) {
    NeuroLayoutRunCurr(CLI_GET(c).ws);
    NeuroWorkspaceUpdate(CLI_GET(c).ws);
  }
  NeuroDzenRefresh(true);
}

static void do_focus_in(XEvent *e) {
  assert(e);
  NeuroClientPtrPtr c = NeuroClientGetFocused();
  if (!c)
    return;
  if (CLI_GET(c).win == e->xfocus.window)
    return;
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroDzenRefresh(true);
}

static void do_client_message(XEvent *e) {
  assert(e);
  NeuroClientPtrPtr c = NeuroClientFindWindow(e->xclient.window);
  if (!c)
    return;
  if (e->xclient.message_type == NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_STATE) &&
      ((unsigned)e->xclient.data.l[1] == NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_FULLSCREEN)
      || (unsigned)e->xclient.data.l[2] == NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_FULLSCREEN))) {
    if (e->xclient.data.l[0] == 0)  // _NET_WM_STATE_REMOVE
      NeuroClientNormal(c, NULL);
    else if (e->xclient.data.l[0] == 1)  // _NET_WM_STATE_ADD
      NeuroClientFullscreen(c, NULL);
    else if (e->xclient.data.l[0] == 2)  // _NET_WM_STATE_TOGGLE
      NeuroClientToggleFullscreen(c, NULL);
  } else if (e->xclient.message_type == NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_ACTIVE)) {
    NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  }
  NeuroDzenRefresh(true);
}

static void do_property_notify(XEvent *e) {
  assert(e);
  XPropertyEvent *ev = &e->xproperty;
  if (ev->atom == XA_WM_NAME || ev->atom == NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_NAME)) {  // Window title
    NeuroClientPtrPtr c = NeuroClientFindWindow(ev->window);
    if (!c)
      return;
    NeuroClientUpdateTitle(c, NULL);
    NeuroDzenRefresh(true);
  } else if (ev->atom == XA_WM_HINTS) {  // Urgency hint
    NeuroClientPtrPtr c = NeuroClientFindWindow(ev->window);
    if (!c)
      return;
    if (NeuroCoreClientIsCurr(c) && NeuroCoreStackIsCurr(CLI_GET(c).ws))
      return;
    XWMHints *wmh = XGetWMHints(NeuroSystemGetDisplay(), CLI_GET(c).win);
    if (wmh && (wmh->flags & XUrgencyHint))
      NeuroClientSetUrgent(c, NULL);
    if (wmh)
      XFree(wmh);
    NeuroClientUpdate(c, NULL);
    NeuroDzenRefresh(true);
  }
}


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static const NeuroEventHandlerFn const event_handlers_[ LASTEvent ] = {
  [ KeyPress ] = do_key_press,
  [ ButtonPress ] = do_button_press,
  [ MapRequest ] = do_map_request,
  [ UnmapNotify ] = do_unmap_notify,
  [ DestroyNotify ] = do_destroy_notify,
  [ EnterNotify ] = do_enter_notify,
  [ ConfigureRequest ] = do_configure_request,
  [ FocusIn ] = do_focus_in,
  [ ClientMessage ] = do_client_message,
  [ PropertyNotify ] = do_property_notify
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

NeuroEventHandlerFn NeuroEventGetHandler(NeuroEventType t) {
  return event_handlers_[ t ];
}

void NeuroEventManageWindow(Window w) {
  // Check if window is valid
  XWindowAttributes wa;
  if (!XGetWindowAttributes(NeuroSystemGetDisplay(), w, &wa))
    return;
  if (wa.override_redirect)
    return;
  if (NeuroClientFindWindow(w))
    return;

  // Add client to the stack list
  NeuroClient *cli = NeuroRuleNewClient(w, &wa);
  if (!cli)
    NeuroSystemError("NeuroEventManageWindow - Could not alloc NeuroClient and set rules");
  NeuroClientPtrPtr c = NeuroCoreAddClientStart(cli);
  if (!c)
    NeuroSystemError("NeuroEventManageWindow - Could not add client");

  // Transient windows
  const size_t ws = CLI_GET(c).ws;
  Window trans = None;
  if (XGetTransientForHint(NeuroSystemGetDisplay(), CLI_GET(c).win, &trans)) {
    CLI_GET(c).free_setter_fn = NeuroRuleFreeSetterDefault;
    NeuroClientPtrPtr t = NeuroClientFindWindow(trans);
    if (t)
      NeuroGeometryCenterRectangleInRegion(NeuroCoreClientGetRegion(c), NeuroCoreClientGetRegion(t));
    else
      NeuroGeometryCenterRectangleInRegion(NeuroCoreClientGetRegion(c), NeuroCoreStackGetRegion(ws));
  }

  // Run layout and update ws focus
  NeuroWorkspaceRemoveEnterNotifyMask(ws);

  NeuroLayoutRunCurr(ws);
  XSelectInput(NeuroSystemGetDisplay(), CLI_GET(c).win, CLIENT_MASK);
  NeuroSystemGrabButtons(CLI_GET(c).win, NeuroConfigGet()->button_list);
  XMapWindow(NeuroSystemGetDisplay(), CLI_GET(c).win);
  NeuroWorkspaceUpdate(ws);
  NeuroWorkspaceFocus(ws);

  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroEventUnmanageClient(NeuroClientPtrPtr c) {
  assert(c);
  const size_t ws = CLI_GET(c).ws;
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroClient *cli = NeuroCoreRemoveClient(c);
  NeuroTypeDeleteClient(cli);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceUpdate(ws);
  NeuroWorkspaceFocus(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroEventLoadWindows() {
  Window d1, d2, *wins = NULL;
  XWindowAttributes wa;
  unsigned int num = 0;
  if (!XQueryTree(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), &d1, &d2, &wins, &num))
    NeuroSystemError("NeuroEventLoadWindows - Could not get windows");
  for (unsigned int i = 0; i < num; ++i) {
    if (!XGetWindowAttributes(NeuroSystemGetDisplay(), wins[ i ], &wa))
      continue;
    if (wa.map_state != IsViewable)
      continue;
    NeuroEventManageWindow(wins[ i ]);
  }
  if (wins)
    XFree(wins);
}

