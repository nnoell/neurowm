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
#include "core.h"
#include "layout.h"
#include "client.h"
#include "rule.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ClientFn
typedef void (*WorkspaceClientFn)(ClientPtrPtr c, const void *data);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Bool is_above_tiled_client(const ClientPtrPtr c) {
  assert(c);
  return (CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull) || CLI_GET(c).isFullScreen;
}

static void focus_client(ClientPtrPtr c) {
  assert(c);
  NeuroClientUnsetUrgent(c, NULL);
  NeuroSystemUngrabButtons(CLI_GET(c).win);
  XSetInputFocus(NeuroSystemGetDisplay(), CLI_GET(c).win, RevertToPointerRoot, CurrentTime);
  XChangeProperty(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), NeuroSystemGetNetAtom(NeuroSystemNetAtomActive),
      XA_WINDOW, 32, PropModeReplace, (unsigned char *)&(CLI_GET(c).win), 1);
  NeuroClientUpdate(c, NULL);
}

static void unfocus_client(ClientPtrPtr c) {
  assert(c);
  NeuroSystemGrabButtons(CLI_GET(c).win);
  NeuroClientUpdate(c, NULL);
}

static void process_client(const WorkspaceClientFn wcf, const ClientPtrPtr ref, const ClientSelectorFn csf,
    const void *data) {
  if (!ref || !csf)
    return;
  ClientPtrPtr c = csf(ref);
  if (!c)
    return;
  wcf(c, data);
}

static void send_client(ClientPtrPtr c, const void *data) {
  if (!c || !data)
    return;
  const int ws = *(const int *)data;
  if (ws < 0 || ws >= NeuroCoreGetSize() + 1)
    return;
  int oldws = CLI_GET(c).ws, currws = NeuroCoreGetCurrStack();
  if (oldws == ws)
    return;
  Rectangle oldRegion = (Rectangle){ .x = 0, .y = 0, .h = 0, .w = 0 };
  const Bool isFree = CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull;
  const Bool doRules = True;
  if (oldws == currws)
    NeuroClientHide(c, (const void *)&doRules);
  if (isFree)
    memmove(&oldRegion, NeuroCoreClientGetRegion(c), sizeof(Rectangle));
  Client *cli = NeuroCoreRemoveClient(c);
  cli->ws = ws;
  ClientPtrPtr c2 = NeuroCoreAddClientStart(cli);
  if (!c2)
    NeuroSystemError("send_client - Could not add client");
  if (isFree)
    memmove(NeuroCoreClientGetRegion(c2), &oldRegion, sizeof(Rectangle));
  if (ws == currws)
    NeuroClientShow(c2, (const void *)&doRules);
  NeuroLayoutRunCurr(currws);
  NeuroWorkspaceFocus(currws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroWorkspaceChange(int ws) {
  int old = NeuroCoreGetCurrStack();
  NeuroCoreSetCurrStack(ws);
  int new = NeuroCoreGetCurrStack();
  if (old == new)
    return;
  NeuroWorkspaceHide(old, True);
  NeuroWorkspaceShow(new, True);
  NeuroLayoutRunCurr(new);
  NeuroWorkspaceFocus(new);
}

void NeuroWorkspaceUpdate(int ws) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientUpdate(c, NULL);
}

void NeuroWorkspaceFocus(int ws) {
  int n = NeuroCoreStackGetSize(ws);
  if (n <= 0) {
    XDeleteProperty(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), NeuroSystemGetNetAtom(NeuroSystemNetAtomActive));
    return;
  }

  Window windows[ n ], d1, d2, *wins = NULL;
  unsigned int i, num;
  int atc = 0;
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    if (is_above_tiled_client(c))
      ++atc;

  c = NeuroCoreStackGetCurrClient(ws);
  windows[ is_above_tiled_client(c) ? 0 : atc ] = CLI_GET(c).win;
  focus_client(c);

  if (n > 1) {
    // XQueryTree gets windows by stacking order
    if (!XQueryTree(NeuroSystemGetDisplay(), NeuroSystemGetRoot(), &d1, &d2, &wins, &num))
      NeuroSystemError("NeuroWorkspaceFocus - Could not get windows");
    int n2 = n;
    for (i = 0; i < num; ++i) {
      ClientPtrPtr c = NeuroWorkspaceClientFindWindow(ws, wins[ i ]);
      if (!c)
        continue;
      if (NeuroCoreClientIsCurr(c))
        continue;
      windows[ is_above_tiled_client(c) ? --atc : --n2 ] = CLI_GET(c).win;
      unfocus_client(c);
    }
    if (wins)
      XFree(wins);
  }

  XRestackWindows(NeuroSystemGetDisplay(), windows, n);
}

void NeuroWorkspaceHide(int ws, Bool doRules) {
  ClientPtrPtr c;
  for (c=NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientHide(c, (const void *)&doRules);
}

void NeuroWorkspaceShow(int ws, Bool doRules) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientShow(c, (const void *)&doRules);
}

void NeuroWorkspaceTile(int ws) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientTile(c, NULL);
}

void NeuroWorkspaceFree(int ws, const void *freeSetterFn) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientFree(c, freeSetterFn);
}

void NeuroWorkspaceMinimize(int ws) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    NeuroClientMinimize(c, NULL);
}

void NeuroWorkspaceRestoreLastMinimized(int ws) {
  if (NeuroCoreStackGetMinimizedNum(ws) <= 0)
    return;
  Client *cli = NeuroCorePopMinimizedClient(ws);
  if (!cli)
    NeuroSystemError("NeuroWorkspaceRestoreLastMinimized - Could not restore last minimized client");
  ClientPtrPtr c = NeuroCoreAddClientStart(cli);
  if (!c)
    NeuroSystemError("NeuroWorkspaceRestoreLastMinimized - Could not add client");
  NeuroRuleApply(c);
  NeuroCoreSetCurrClient(c);
  NeuroLayoutRunCurr(CLI_GET(c).ws);
  NeuroWorkspaceFocus(CLI_GET(c).ws);
}

void NeuroWorkspaceAddEnterNotifyMask(int ws) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    XSelectInput(NeuroSystemGetDisplay(), CLI_GET(c).win, CLIENT_MASK);
}

void NeuroWorkspaceRemoveEnterNotifyMask(int ws) {
  ClientPtrPtr c;
  for (c = NeuroCoreStackGetHeadClient(ws); c; c = NeuroCoreClientGetNext(c))
    XSelectInput(NeuroSystemGetDisplay(), CLI_GET(c).win, CLIENT_MASK_NO_ENTER);
}

// Find functions
ClientPtrPtr NeuroWorkspaceClientFindWindow(int ws, Window w) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterWindow, (const void *)&w);
}

ClientPtrPtr NeuroWorkspaceClientFindUrgent(int ws) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterUrgent, NULL);
}

ClientPtrPtr NeuroWorkspaceClientFindFixed(int ws) {
  return NeuroCoreStackFindClient(ws, NeuroClientTesterFixed, NULL);
}

// Clients
void NeuroWorkspaceClientFocus(const ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  (void)data;
  if (!ref || !csf)
    return;
  ClientPtrPtr c = csf(ref);
  if (!c)
    return;
  NeuroCoreSetCurrClient(c);
  NeuroWorkspaceFocus(CLI_GET(c).ws);
}

void NeuroWorkspaceClientSwap(const ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  (void)data;
  if (!ref || !csf)
    return;
  ClientPtrPtr c = csf(ref);
  if (!c)
    return;
  if (!NeuroCoreClientSwap(ref, c))
    return;
  NeuroWorkspaceUpdate(CLI_GET(c).ws);
  NeuroWorkspaceClientFocus(ref, csf, NULL);
}

void NeuroWorkspaceClientSend(ClientPtrPtr ref, const ClientSelectorFn csf, const void *ws) {
  process_client(send_client, ref, csf, ws);
}

void NeuroWorkspaceClientKill(const ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientKill, ref, csf, data);
}

void NeuroWorkspaceClientMinimize(const ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientMinimize, ref, csf, data);
}

void NeuroWorkspaceClientTile(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientTile, ref, csf, data);
}

void NeuroWorkspaceClientFree(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn) {
  process_client(NeuroClientFree, ref, csf, freeSetterFn);
}

void NeuroWorkspaceClientToggleFree(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn) {
  process_client(NeuroClientToggleFree, ref, csf, freeSetterFn);
}

void NeuroWorkspaceClientNormal(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientNormal, ref, csf, data);
}

void NeuroWorkspaceClientFullscreen(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFullscreen, ref, csf, data);
}

void NeuroWorkspaceClientToggleFullscreen(const ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientToggleFullscreen, ref, csf, data);
}

void NeuroWorkspaceClientFloatMove(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFloatMove, ref, csf, data);
}

void NeuroWorkspaceClientFloatResize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFloatResize, ref, csf, data);
}

void NeuroWorkspaceClientFreeMove(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFreeMove, ref, csf, data);
}

void NeuroWorkspaceClientFreeResize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data) {
  process_client(NeuroClientFreeResize, ref, csf, data);
}

// Workspace Selectors
int NeuroWorkspaceSelector0() {
  return 0;
}

int NeuroWorkspaceSelector1() {
  return 1;
}

int NeuroWorkspaceSelector2() {
  return 2;
}

int NeuroWorkspaceSelector3() {
  return 3;
}

int NeuroWorkspaceSelector4() {
  return 4;
}

int NeuroWorkspaceSelector5() {
  return 5;
}

int NeuroWorkspaceSelector6() {
  return 6;
}

int NeuroWorkspaceSelector7() {
  return 7;
}

int NeuroWorkspaceSelector8() {
  return 8;
}

int NeuroWorkspaceSelector9() {
  return 9;
}

int NeuroWorkspaceSelectorCurr() {
  return NeuroCoreGetCurrStack();
}

int NeuroWorkspaceSelectorPrev() {
  return NeuroCoreGetPrevStack();
}

int NeuroWorkspaceSelectorNext() {
  return NeuroCoreGetNextStack();
}

int NeuroWorkspaceSelectorOld() {
  return NeuroCoreGetOldStack();
}

