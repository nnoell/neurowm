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
#include "stackset.h"
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

static Bool isAboveTiledClient(const ClientPtrPtr c) {
  assert(c);
  return (CLI_GET(c).freeSetterFn != notFreeR) || CLI_GET(c).isFullScreen;
}

static void focusClient(ClientPtrPtr c) {
  assert(c);
  unsetUrgentC(c, NULL);
  ungrabButtonsS(CLI_GET(c).win);
  XSetInputFocus(display, CLI_GET(c).win, RevertToPointerRoot, CurrentTime);
  XChangeProperty(display, root, netatoms[ NET_ACTIVE ], XA_WINDOW, 32, PropModeReplace,
      (unsigned char *)&(CLI_GET(c).win), 1);
  updateC(c, NULL);
}

static void unfocusClient(ClientPtrPtr c) {
  assert(c);
  grabButtonsS(CLI_GET(c).win);
  updateC(c, NULL);
}

static void processClient(const WorkspaceClientFn wcf, const ClientPtrPtr c, const ClientSelectorFn csf,
    const void *data) {
  if (!c || !csf)
    return;
  ClientPtrPtr dst = csf(c);
  if (!dst)
    return;
  wcf(dst, data);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void updateW(int ws) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    updateC(c, NULL);
}

void updateFocusW(int ws) {
  int n = getSizeStackSS(ws);
  if (n <= 0) {
    XDeleteProperty(display, root, netatoms[ NET_ACTIVE ]);
    return;
  }

  Window windows[ n ], d1, d2, *wins = NULL;
  unsigned int i, num;
  int atc = 0;
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    if (isAboveTiledClient(c))
      ++atc;

  c = getCurrClientStackSS(ws);
  windows[ isAboveTiledClient(c) ? 0 : atc ] = CLI_GET(c).win;
  focusClient(c);

  if (n > 1) {
    if (!XQueryTree(display, root, &d1, &d2, &wins, &num))  // XQueryTree gets windows by stacking order
      exitErrorS("updateFocusW - could not get windows");
    int n2 = n;
    for (i = 0; i < num; ++i) {
      ClientPtrPtr c = findWindowClientW(ws, wins[ i ]);
      if (!c)
        continue;
      if (isCurrClientSS(c))
        continue;
      windows[ isAboveTiledClient(c) ? --atc : --n2 ] = CLI_GET(c).win;
      unfocusClient(c);
    }
    if (wins)
      XFree(wins);
  }

  XRestackWindows(display, windows, n);
}

void hideW(int ws, Bool doRules) {
  ClientPtrPtr c;
  for (c=getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    hideC(c, (const void *)&doRules);
}

void showW(int ws, Bool doRules) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    showC(c, (const void *)&doRules);
}

void tileW(int ws) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    tileC(c, NULL);
}

void freeW(int ws, const void *freeSetterFn) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    freeC(c, freeSetterFn);
}

void changeToWorkspaceW(int ws) {
  int old = getCurrStackSS();
  setCurrStackSS(ws);
  int new = getCurrStackSS();
  if (old == new)
    return;
  hideW(old, True);
  showW(new, True);
  runCurrLayoutL(new);
  updateFocusW(new);
}

void moveClientToWorkspaceW(ClientPtrPtr c, int ws) {
  if (!c)
    return;
  if (ws < 0 || ws >= getSizeSS() + 1)
    return;
  int oldws = CLI_GET(c).ws, currws = getCurrStackSS();
  if (oldws == ws)
    return;
  Rectangle oldRegion = (Rectangle){ .x = 0, .y = 0, .h = 0, .w = 0 };
  const Bool isFree = CLI_GET(c).freeSetterFn != notFreeR;
  const Bool doRules = True;
  if (oldws == currws)
    hideC(c, (const void *)&doRules);
  if (isFree)
    memmove(&oldRegion, getRegionClientSS(c), sizeof(Rectangle));
  Client *cli = rmvClientSS(c);
  cli->ws = ws;
  ClientPtrPtr c2 = addClientStartSS(cli);
  if (!c2)
    exitErrorS("moveCliToWorkspaceW - could not add client");
  if (isFree)
    memmove(getRegionClientSS(c2), &oldRegion, sizeof(Rectangle));
  if (ws == currws)
    showC(c2, (const void *)&doRules);
  runCurrLayoutL(currws);
  updateFocusW(currws);
}

void minimizeW(int ws) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    minimizeC(c, NULL);
}

void restoreLastMinimizedW(int ws) {
  if (getMinimizedNumStackSS(ws) <= 0)
    return;
  Client *cli = popMinimizedClientSS(ws);
  if (!cli)
    exitErrorS("restoreLastMinimizedW - could not restore last minimized client");
  ClientPtrPtr c = addClientStartSS(cli);
  if (!c)
    exitErrorS("restoreLastMinimizedW - could not add client");
  applyRuleR(c);
  setCurrClientSS(c);
  runCurrLayoutL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void addEnterNotifyMaskW(int ws) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    XSelectInput(display, CLI_GET(c).win, CLIENT_MASK);
}

void rmvEnterNotifyMaskW(int ws) {
  ClientPtrPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    XSelectInput(display, CLI_GET(c).win, CLIENT_MASK_NO_ENTER);
}


// Clients
void moveFocusClientW(const ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  (void)data;
  if (!c || !csf)
    return;
  ClientPtrPtr dst = csf(c);
  if (!dst)
    return;
  setCurrClientSS(dst);
  updateFocusW(CLI_GET(dst).ws);
}

void swapClientW(const ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  (void)data;
  if (!c || !csf)
    return;
  ClientPtrPtr dst = csf(c);
  if (!dst)
    return;
  if (!swpClientSS(c, dst))
    return;
  updateW(CLI_GET(c).ws);
  moveFocusClientW(c, csf, NULL);
}

void killClientW(const ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(killC, c, csf, data);
}

void minimizeClientW(const ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(minimizeC, c, csf, data);
}

void tileClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(tileC, c, csf, data);
}

void freeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *freeSetterFn) {
  processClient(freeC, c, csf, freeSetterFn);
}

void toggleFreeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *freeSetterFn) {
  processClient(toggleFreeC, c, csf, freeSetterFn);
}

void normalClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(normalC, c, csf, data);
}

void fullScreenClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(fullScreenC, c, csf, data);
}

void toggleFullScreenClientW(const ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(toggleFullScreenC, c, csf, data);
}

void moveClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(moveC, c, csf, data);
}

void resizeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(resizeC, c, csf, data);
}

void freeMoveClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(freeMoveC, c, csf, data);
}

void freeResizeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
  processClient(freeResizeC, c, csf, data);
}


// Workspace Selectors
int prevW() {
  return getPrevStackSS();
}

int nextW() {
  return getNextStackSS();
}

int oldW() {
  return getOldStackSS();
}


// Get functions
ClientPtrPtr getPtrClientW(int *x, int *y) {
  assert(x);
  assert(y);
  Window rootw, childw;
  int xc, yc;
  unsigned state;
  if (!XQueryPointer(display, root, &rootw, &childw, x, y, &xc, &yc, &state))
    return NULL;
  return findWindowClientAllW(childw);
}


// Find functions
ClientPtrPtr findWindowClientAllW(Window w) {
  return findClientSS(testWindowC, (const void *)&w);
}

ClientPtrPtr findWindowClientW(int ws, Window w) {
  return findClientStackSS(ws, testWindowC, (const void *)&w);
}

ClientPtrPtr findUrgentClientW(int ws) {
  return findClientStackSS(ws, testIsUrgentC, NULL);
}

ClientPtrPtr findFixedClientW(int ws) {
  return findClientStackSS(ws, testIsFixedC, NULL);
}

