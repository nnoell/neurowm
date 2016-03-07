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

  const XKeyEvent ke = e->xkey;
  int ks = 0;
  KeySym *const key_sym = XGetKeyboardMapping(NeuroSystemGetDisplay(), ke.keycode, 1, &ks);
  for (NeuroIndex i = 0U; key_list[ i ]; ++i) {
    const NeuroKey *k = key_list[ i ];
    if (k->key == *key_sym && k->mod == ke.state) {
      NeuroActionRunActionChain(&k->action_chain);
      NeuroDzenRefresh(true);
    }
  }
  XFree(key_sym);
}

static void do_button_press(XEvent *e) {
  assert(e);
  const NeuroButton *const *const button_list = NeuroConfigGet()->button_list;
  if (!button_list)
    return;

  const XButtonPressedEvent *const ev = &e->xbutton;
  for (NeuroIndex i = 0U; button_list[ i ]; ++i) {
    const NeuroButton *b = button_list[ i ];
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
  const Window w = e->xdestroywindow.window;
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
  const Window w = e->xdestroywindow.window;
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
  const XCrossingEvent *const ev = &e->xcrossing;
  if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != NeuroSystemGetRoot())
    return;
  NeuroClientPtrPtr c = NeuroClientFindWindow(ev->window);
  if (!c)
    return;

  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  if (!NeuroCoreStackGetCurrLayout(ws)->follow_mouse)
    return;
  NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
  NeuroCoreSetCurrStack(ws);
  NeuroWorkspaceClientFocus(c, NeuroClientSelectorSelf, NULL);
  NeuroDzenRefresh(true);
}

static void do_configure_request(XEvent *e) {
  assert(e);
  const XConfigureRequestEvent *const ev = &e->xconfigurerequest;

  XWindowChanges wc;
  wc.x = ev->x;
  wc.y = ev->y;
  wc.width = ev->width;
  wc.height = ev->height;
  wc.sibling = ev->above;
  wc.stack_mode = ev->detail;
  wc.border_width = ev->border_width;
  XConfigureWindow(NeuroSystemGetDisplay(), ev->window, ev->value_mask, &wc);
  NeuroClientPtrPtr c = NeuroClientFindWindow(ev->window);
  if (c) {
    const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
    NeuroLayoutRunCurr(ws);
    NeuroWorkspaceUpdate(ws);
  }
  NeuroDzenRefresh(true);
}

static void do_focus_in(XEvent *e) {
  assert(e);
  NeuroClientPtrPtr c = NeuroClientGetFocused();
  if (!c)
    return;
  if (NEURO_CLIENT_PTR(c)->win == e->xfocus.window)
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
  const XPropertyEvent *const ev = &e->xproperty;

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
    if (NeuroCoreClientIsCurr(c) && NeuroCoreStackIsCurr(NEURO_CLIENT_PTR(c)->ws))
      return;
    XWMHints *wmh = XGetWMHints(NeuroSystemGetDisplay(), NEURO_CLIENT_PTR(c)->win);
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

static const NeuroEventHandlerFn event_handlers_[ LASTEvent ] = {
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
  NeuroClient *const cli = NeuroRuleNewClient(w, &wa);
  if (!cli)
    NeuroSystemError("NeuroEventManageWindow - Could not alloc NeuroClient and set rules");
  NeuroClientPtrPtr c = NeuroCoreAddClientStart(cli);
  if (!c)
    NeuroSystemError("NeuroEventManageWindow - Could not add client");

  // Transient windows
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  const Window win = NEURO_CLIENT_PTR(c)->win;
  Window trans = None;
  if (XGetTransientForHint(NeuroSystemGetDisplay(), win, &trans)) {
    NEURO_CLIENT_PTR(c)->free_setter_fn = NeuroRuleFreeSetterDefault;
    NeuroClientPtrPtr t = NeuroClientFindWindow(trans);
    if (t)
      NeuroGeometryRectangleCenter(NeuroCoreClientGetRegion(c), NeuroCoreClientGetRegion(t));
    else
      NeuroGeometryRectangleCenter(NeuroCoreClientGetRegion(c), NeuroCoreStackGetRegion(ws));
  }

  // Run layout and update ws focus
  NeuroWorkspaceRemoveEnterNotifyMask(ws);

  NeuroLayoutRunCurr(ws);
  XSelectInput(NeuroSystemGetDisplay(), win, NEURO_SYSTEM_CLIENT_MASK);
  NeuroSystemGrabButtons(win, NeuroConfigGet()->button_list);
  XMapWindow(NeuroSystemGetDisplay(), win);
  NeuroWorkspaceUpdate(ws);
  NeuroWorkspaceFocus(ws);

  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroEventUnmanageClient(NeuroClientPtrPtr c) {
  assert(c);
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroClient *cli = NeuroCoreRemoveClient(c);
  NeuroTypeDeleteClient(cli);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceUpdate(ws);
  NeuroWorkspaceFocus(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroEventLoadWindows(void) {
  Window d1 = 0UL, d2 = 0UL, *wins = NULL;
  unsigned int num = 0;
  if (!XQueryTree(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), &d1, &d2, &wins, &num))
    NeuroSystemError("NeuroEventLoadWindows - Could not get windows");
  for (unsigned int i = 0; i < num; ++i) {
    XWindowAttributes wa;
    if (!XGetWindowAttributes(NeuroSystemGetDisplay(), wins[ i ], &wa))
      continue;
    if (wa.map_state != IsViewable)
      continue;
    NeuroEventManageWindow(wins[ i ]);
  }
  if (wins)
    XFree(wins);
}

