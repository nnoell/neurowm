//----------------------------------------------------------------------------------------------------------------------
// Module      :  workspace
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "workspace.h"
#include "system.h"
#include "config.h"
#include "core.h"
#include "layout.h"
#include "client.h"
#include "rule.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ClientFn
typedef void (*WorkspaceClientFn)(NeuroClientPtrPtr c, const void *data);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static bool is_above_tiled_client(const NeuroClientPtrPtr c) {
  assert(c);
  return (NEURO_CLIENT_PTR(c)->free_setter_fn != NeuroRuleFreeSetterNull) || NEURO_CLIENT_PTR(c)->is_fullscreen;
}

static void focus_client(NeuroClientPtrPtr c) {
  assert(c);
  NeuroClientUnsetUrgent(c, NULL);
  const Window win = NEURO_CLIENT_PTR(c)->win;
  NeuroSystemUngrabButtons(win, NeuroConfigGet()->button_list);
  XSetInputFocus(NeuroSystemGetDisplay(), win, RevertToPointerRoot, CurrentTime);
  XChangeProperty(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_ACTIVE),
      XA_WINDOW, 32, PropModeReplace, (unsigned char *)&(win), 1);
}

static void unfocus_client(NeuroClientPtrPtr c) {
  assert(c);
  NeuroSystemGrabButtons(NEURO_CLIENT_PTR(c)->win, NeuroConfigGet()->button_list);
}

static void process_client(const WorkspaceClientFn wcf, const NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf,
    const void *data) {
  if (!ref || !csf)
    return;
  NeuroClientPtrPtr c = csf(ref);
  if (!c)
    return;
  wcf(c, data);
}

static void send_client(NeuroClientPtrPtr c, const void *data) {
  if (!c || !data)
    return;

  // Get the workspaces
  const NeuroIndex new_ws = *(const NeuroIndex *)data;
  const NeuroIndex old_ws = NEURO_CLIENT_PTR(c)->ws;
  const NeuroIndex curr_ws = NeuroCoreGetCurrStack();
  if (old_ws == new_ws)
    return;

  // 'Default free' windows are set to 'Center free' because of different screen resolutions
  if (NEURO_CLIENT_PTR(c)->free_setter_fn == NeuroRuleFreeSetterDefault)
    NEURO_CLIENT_PTR(c)->free_setter_fn = NeuroRuleFreeSetterCenter;

  // Remove the client from its stack
  NeuroClient *const cli = NeuroCoreRemoveClient(c);

  // Add the client to the new stack
  cli->ws = new_ws;
  const NeuroClientPtrPtr c2 = NeuroCoreAddClientStart(cli);
  if (!c2)
    NeuroSystemError("send_client - Could not add client");

  // Update old and new workspaces
  NeuroLayoutRunCurr(curr_ws);
  NeuroWorkspaceUpdate(curr_ws);
  NeuroLayoutRunCurr(new_ws);
  NeuroWorkspaceUpdate(new_ws);

  // Focus the current workspace
  NeuroWorkspaceFocus(curr_ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroWorkspaceChange(NeuroIndex ws) {
  if (NeuroCoreStackIsCurr(ws))
    return;

  const NeuroIndex curr = NeuroCoreGetCurrStack();
  const NeuroMonitor *const curr_monitor = NeuroCoreStackGetMonitor(curr);
  const NeuroMonitor *const new_monitor = NeuroCoreStackGetMonitor(ws);
  NeuroCoreStackSetMonitor(curr, new_monitor);
  NeuroCoreStackSetMonitor(ws, curr_monitor);

  // Update old and new workspaces
  NeuroLayoutRunCurr(curr);
  NeuroWorkspaceUpdate(curr);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceUpdate(ws);

  // Focus the current workspace
  NeuroWorkspaceUnfocus(curr);
  NeuroCoreSetCurrStack(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroWorkspaceUpdate(NeuroIndex ws) {
  for (NeuroClientPtrPtr c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientUpdate(c, NULL);
}

void NeuroWorkspaceFocus(NeuroIndex ws) {
  NeuroIndex n = NeuroCoreStackGetSize(ws);
  if (n <= 0) {
    XDeleteProperty(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), NeuroSystemGetNetAtom(NEURO_SYSTEM_NETATOM_ACTIVE));
    return;
  }

  Window windows[ n ], d1, d2, *wins = NULL;
  NeuroIndex atc = 0U;
  NeuroClientPtrPtr c = NeuroCoreStackGetHeadClient(ws);
  for ( ; c; c = NeuroCoreClientGetNext(c))
    if (is_above_tiled_client(c))
      ++atc;

  c = NeuroCoreStackGetCurrClient(ws);
  windows[ is_above_tiled_client(c) ? 0U : atc ] = NEURO_CLIENT_PTR(c)->win;
  focus_client(c);
  NeuroClientUpdate(c, NULL);

  if (n > 1) {
    // XQueryTree gets windows by stacking order
    unsigned int num = 0U;
    if (!XQueryTree(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), &d1, &d2, &wins, &num))
      NeuroSystemError("NeuroWorkspaceFocus - Could not get windows");
    NeuroIndex n2 = n;
    for (unsigned int i = 0U; i < num; ++i) {
      NeuroClientPtrPtr c = NeuroWorkspaceClientFindWindow(ws, wins[ i ]);
      if (!c)
        continue;
      if (NeuroCoreClientIsCurr(c))
        continue;
      windows[ is_above_tiled_client(c) ? --atc : --n2 ] = NEURO_CLIENT_PTR(c)->win;
      unfocus_client(c);
      NeuroClientUpdate(c, NULL);
    }
    if (wins)
      XFree(wins);
  }

  XRestackWindows(NeuroSystemGetDisplay(), windows, n);
}

void NeuroWorkspaceUnfocus(NeuroIndex ws) {
  for (NeuroClientPtrPtr c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    unfocus_client(c);
}

void NeuroWorkspaceTile(NeuroIndex ws) {
  for (NeuroClientPtrPtr c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientTile(c, NULL);
}

void NeuroWorkspaceFree(NeuroIndex ws, const void *free_setter_fn) {
  for (NeuroClientPtrPtr c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientFree(c, free_setter_fn);
}

void NeuroWorkspaceMinimize(NeuroIndex ws) {
  for (NeuroClientPtrPtr c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientMinimize(c, NULL);
}

void NeuroWorkspaceRestoreLastMinimized(NeuroIndex ws) {
  if (NeuroCoreStackGetMinimizedNum(ws) <= 0)
    return;
  const NeuroClient *cli = NeuroCorePopMinimizedClient(ws);
  if (!cli)
    NeuroSystemError("NeuroWorkspaceRestoreLastMinimized - Could not restore last minimized client");
  NeuroClientPtrPtr c = NeuroCoreAddClientStart(cli);
  if (!c)
    NeuroSystemError("NeuroWorkspaceRestoreLastMinimized - Could not add client");
  NeuroCoreSetCurrClient(c);
  const Window win = NEURO_CLIENT_PTR(c)->ws;
  NeuroLayoutRunCurr(win);
  NeuroWorkspaceFocus(win);
}

void NeuroWorkspaceAddEnterNotifyMask(NeuroIndex ws) {
  NeuroClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    XSelectInput(NeuroSystemGetDisplay(), NEURO_CLIENT_PTR(c)->win, NEURO_SYSTEM_CLIENT_MASK);
}

void NeuroWorkspaceRemoveEnterNotifyMask(NeuroIndex ws) {
  NeuroClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    XSelectInput(NeuroSystemGetDisplay(), NEURO_CLIENT_PTR(c)->win, NEURO_SYSTEM_CLIENT_MASK_NO_ENTER);
}

// Find functions
NeuroClientPtrPtr NeuroWorkspaceClientFindWindow(NeuroIndex ws, Window w) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterWindow, (const void *)&w);
}

NeuroClientPtrPtr NeuroWorkspaceClientFindUrgent(NeuroIndex ws) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterUrgent, NULL);
}

NeuroClientPtrPtr NeuroWorkspaceClientFindFixed(NeuroIndex ws) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterFixed, NULL);
}

NeuroClientPtrPtr NeuroWorkspaceClientFindPointed(NeuroIndex ws, const NeuroPoint *p) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterPointed, (const void *)p);
}

// Clients
void NeuroWorkspaceClientFocus(const NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  (void)data;
  if (!ref || !csf)
    return;
  NeuroClientPtrPtr c = csf(ref);
  if (!c)
    return;
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  // NeuroCoreSetCurrStack(ws);  // This focuses new windows in another workspaces
  NeuroCoreSetCurrClient(c);
  NeuroWorkspaceFocus(ws);
}

void NeuroWorkspaceClientSwap(const NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  (void)data;
  if (!ref || !csf)
    return;
  NeuroClientPtrPtr c = csf(ref);
  if (!c)
    return;
  if (!NeuroCoreClientSwap(ref, c))
    return;
  NeuroWorkspaceUpdate(NEURO_CLIENT_PTR(c)->ws);
  NeuroWorkspaceClientFocus(ref, csf, NULL);
}

void NeuroWorkspaceClientSend(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *ws) {
  process_client(send_client, ref, csf, ws);
}

void NeuroWorkspaceClientKill(const NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientKill, ref, csf, data);
}

void NeuroWorkspaceClientMinimize(const NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientMinimize, ref, csf, data);
}

void NeuroWorkspaceClientTile(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientTile, ref, csf, data);
}

void NeuroWorkspaceClientFree(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *free_setter_fn) {
  process_client(NeuroClientFree, ref, csf, free_setter_fn);
}

void NeuroWorkspaceClientToggleFree(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf,
    const void *free_setter_fn) {
  process_client(NeuroClientToggleFree, ref, csf, free_setter_fn);
}

void NeuroWorkspaceClientNormal(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientNormal, ref, csf, data);
}

void NeuroWorkspaceClientFullscreen(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFullscreen, ref, csf, data);
}

void NeuroWorkspaceClientToggleFullscreen(const NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf,
    const void *data) {
  process_client(NeuroClientToggleFullscreen, ref, csf, data);
}

void NeuroWorkspaceClientFloatMove(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFloatMove, ref, csf, data);
}

void NeuroWorkspaceClientFloatResize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFloatResize, ref, csf, data);
}

void NeuroWorkspaceClientFreeMove(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFreeMove, ref, csf, data);
}

void NeuroWorkspaceClientFreeResize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFreeResize, ref, csf, data);
}

// NeuroWorkspace Selectors
NeuroIndex NeuroWorkspaceSelector0() {
  return 0;
}

NeuroIndex NeuroWorkspaceSelector1() {
  return 1;
}

NeuroIndex NeuroWorkspaceSelector2() {
  return 2;
}

NeuroIndex NeuroWorkspaceSelector3() {
  return 3;
}

NeuroIndex NeuroWorkspaceSelector4() {
  return 4;
}

NeuroIndex NeuroWorkspaceSelector5() {
  return 5;
}

NeuroIndex NeuroWorkspaceSelector6() {
  return 6;
}

NeuroIndex NeuroWorkspaceSelector7() {
  return 7;
}

NeuroIndex NeuroWorkspaceSelector8() {
  return 8;
}

NeuroIndex NeuroWorkspaceSelector9() {
  return 9;
}

NeuroIndex NeuroWorkspaceSelectorCurr() {
  return NeuroCoreGetCurrStack();
}

NeuroIndex NeuroWorkspaceSelectorPrev() {
  return NeuroCoreGetPrevStack();
}

NeuroIndex NeuroWorkspaceSelectorNext() {
  return NeuroCoreGetNextStack();
}

NeuroIndex NeuroWorkspaceSelectorOld() {
  return NeuroCoreGetOldStack();
}

