//----------------------------------------------------------------------------------------------------------------------
// Module      :  action
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
#include "action.h"
#include "system.h"
#include "stackset.h"
#include "dzenpanel.h"
#include "neurowm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ActionClientFn
typedef void (*ActionClientFn)(ClientPtrPtr c, ClientSelectorFn gcf, const void *data);

// ActionWorkspaceFn
typedef void (*ActionWorkspaceFn)(int ws);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void processWorkspaceAction(const ActionWorkspaceFn gwsaf, int ws) {
  assert(gwsaf);
  rmvEnterNotifyMaskW(ws);
  gwsaf(ws);
  addEnterNotifyMaskW(ws);
}

static void processClientAction(const ActionClientFn gcaf, ClientPtrPtr c, const ClientSelectorFn csf,
    const void *data) {
  assert(gcaf);
  assert(csf);
  if (!c)
    return;
  rmvEnterNotifyMaskW(CLI_GET(c).ws);
  gcaf(c, csf, data);
  addEnterNotifyMaskW(CLI_GET(c).ws);
}

//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Actions
const Action const nothingActionA[] = { { nothingHandlerA, ARG_NULL }};
const Action const quitNeurowmActionA[] = { { quitNeurowmHandlerA, ARG_NULL } };
const Action const reloadNeurowmActionA[] = { { reloadNeurowmHandlerA, ARG_NULL } };
const Action const startCpuCalcActionA[] = { { startCpuCalcHandlerA, ARG_NULL } };
const Action const spawnActionA[] = { { spawnHandlerA, ARG_CMD(defTerminalCommand) } };
const Action const sleepActionA[] = { { sleepHandlerA, ARG_INT(1) } };
const Action const endCpuCalcActionA[] = { { endCpuCalcHandlerA, ARG_NULL } };
const Action const changeLayoutActionA[] = { { changeLayoutHandlerA, ARG_INT(1) } };
const Action const resetLayoutActionA[] = { { resetLayoutHandlerA, ARG_NULL } };
const Action const toggleLayoutActionA[] = { { toggleLayoutHandlerA, ARG_INT(0) } };
const Action const toggleLayoutModActionA[] = { { toggleLayoutModHandlerA, ARG_UINT(notModL) } };
const Action const increaseMasterLayoutActionA[] = { { increaseMasterLayoutHandlerA, ARG_INT(1) } };
const Action const resizeMasterLayoutActionA[] = { { resizeMasterLayoutHandlerA, ARG_FLOAT(1.0f) } };
const Action const changeToWorkspaceActionA[] = { { changeToWorkspaceHandlerA, ARG_INT(0) } };
const Action const changeToRelWorkspaceActionA[] = { { changeToRelWorkspaceHandlerA, ARG_WSF(nextW) } };
const Action const moveClientToWorkspaceActionA[] = { { moveClientToWorkspaceHandlerA, ARG_INT(0) } };
const Action const moveClientToRelWorkspaceActionA[] = { { moveClientToRelWorkspaceHandlerA, ARG_WSF(nextW) } };
const Action const restoreLastMinimizedActionA[] = { { restoreLastMinimizedHandlerA, ARG_NULL } };
const Action const toggleNSPActionA[] = { { toggleNSPHandlerA, ARG_CMD(defNSPCommand) } };
const Action const moveFocusCurrClientActionA[] = { { moveFocusCurrClientHandlerA, ARG_CSF(nextC) } };
const Action const swapCurrClientActionA[] = { { swapCurrClientHandlerA, ARG_CSF(nextC) } };
const Action const killCurrClientActionA[] = { { killCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const tileCurrClientActionA[] = { { tileCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const normalCurrClientActionA[] = { { normalCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const fullScreenCurrClientActionA[] = { { fullScreenCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const toggleFullScreenCurrClientActionA[] = { { toggleFullScreenCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const minimizeCurrClientActionA[] = { { minimizeCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const freeCurrClientActionA[] = { { freeCurrClientHandlerA, ARG_FSF(defFreeR) } };
const Action const toggleFreeCurrClientActionA[] = { { toggleFreeCurrClientHandlerA, ARG_FSF(defFreeR) } };
const Action const moveFocusPtrClientActionA[] = { { moveFocusPtrClientHandlerA, ARG_CSF(selfC) } };
const Action const freeMovePtrClientActionA[] = { { freeMovePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const freeResizePtrClientActionA[] = { { freeResizePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const movePtrClientActionA[] = { { movePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const resizePtrClientActionA[] = { { resizePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const toggleFullScreenPtrClientActionA[] = { { toggleFullScreenPtrClientHandlerA, ARG_CSF(selfC) } };
const Action const freePtrClientActionA[] = { { freePtrClientHandlerA, ARG_FSF(defFreeR) } };
const Action const toggleFreePtrClientActionA[] = { { toggleFreePtrClientHandlerA, ARG_FSF(defFreeR) } };

// Action Chains
const Action *const nothingA[] = { nothingActionA, NULL };
const Action *const quitNeurowmA[] = { quitNeurowmActionA, NULL };
const Action *const reloadNeurowmA[] = { reloadNeurowmActionA, NULL };
const Action *const startCpuCalcA[] = { startCpuCalcActionA, NULL };
const Action *const spawnA[] = { spawnActionA, NULL };
const Action *const sleepA[] = { sleepActionA, NULL };
const Action *const endCpuCalcA[] = { endCpuCalcActionA, NULL };
const Action *const changeLayoutA[] = { changeLayoutActionA, NULL };
const Action *const resetLayoutA[] = { resetLayoutActionA, NULL };
const Action *const toggleLayoutA[] = { toggleLayoutActionA, NULL };
const Action *const toggleLayoutModA[] = { toggleLayoutModActionA, NULL };
const Action *const increaseMasterLayoutA[] = { increaseMasterLayoutActionA, NULL };
const Action *const resizeMasterLayoutA[] = { resizeMasterLayoutActionA, NULL };
const Action *const changeToWorkspaceA[] = { changeToWorkspaceActionA, NULL };
const Action *const changeToRelWorkspaceA[] = { changeToRelWorkspaceActionA, NULL };
const Action *const moveClientToWorkspaceA[] = { moveClientToWorkspaceActionA, NULL };
const Action *const moveClientToRelWorkspaceA[] = { moveClientToRelWorkspaceActionA, NULL };
const Action *const moveClientToWorkspaceFollowA[] = { moveClientToWorkspaceActionA, changeToWorkspaceActionA, NULL };
const Action *const moveClientToRelWorkspaceFollowA[] = {
  moveClientToRelWorkspaceActionA, changeToRelWorkspaceActionA, NULL
};
const Action *const restoreLastMinimizedA[] = { restoreLastMinimizedActionA, NULL };
const Action *const toggleNSPA[] = { toggleNSPActionA, NULL };
const Action *const moveFocusCurrClientA[] = { moveFocusCurrClientActionA, NULL };
const Action *const swapCurrClientA[] = { swapCurrClientActionA, NULL };
const Action *const killCurrClientA[] = { killCurrClientActionA, NULL };
const Action *const tileCurrClientA[] = { tileCurrClientActionA, NULL };
const Action *const normalCurrClientA[] = { normalCurrClientActionA, NULL };
const Action *const fullScreenCurrClientA[] = { fullScreenCurrClientActionA, NULL };
const Action *const toggleFullScreenCurrClientA[] = { toggleFullScreenCurrClientActionA, NULL };
const Action *const minimizeCurrClientA[] = { minimizeCurrClientActionA, NULL };
const Action *const freeCurrClientA[] = { freeCurrClientActionA, NULL };
const Action *const toggleFreeCurrClientA[] = { toggleFreeCurrClientActionA, NULL };
const Action *const moveFocusPtrClientA[] = { moveFocusPtrClientActionA, NULL };
const Action *const freeMovePtrClientA[] = { freeMovePtrClientActionA, NULL };
const Action *const freeResizePtrClientA[] = { freeResizePtrClientActionA, NULL };
const Action *const movePtrClientA[] = { movePtrClientActionA, NULL };
const Action *const resizePtrClientA[] = { resizePtrClientActionA, NULL };
const Action *const toggleFullScreenPtrClientA[] = { toggleFullScreenPtrClientActionA, NULL };
const Action *const freePtrClientA[] = { freePtrClientActionA, NULL };
const Action *const toggleFreePtrClientA[] = { toggleFreePtrClientActionA, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void nothingHandlerA(GenericArg null_arg) {
  (void)null_arg;
}

void quitNeurowmHandlerA(GenericArg null_arg) {
  (void)null_arg;
  quitNeurowmN();
}

void reloadNeurowmHandlerA(GenericArg null_arg) {
  (void)null_arg;
  reloadNeurowmN();
}

void changeNeurowmNameHandlerA(GenericArg string_arg) {
  assert(ARG_STR_GET(string_arg));
  changeWMNameS(ARG_STR_GET(string_arg));
}

void spawnHandlerA(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  spawnS(ARG_CMD_GET(command_arg), NULL);
}

void sleepHandlerA(GenericArg int_arg) {
  assert(ARG_INT_GET(int_arg) >= 0);
  sleep(ARG_INT_GET(int_arg));
}

void startCpuCalcHandlerA(GenericArg null_arg) {
  (void)null_arg;
  startCpuCalcDP();
}

void endCpuCalcHandlerA(GenericArg null_arg) {
  (void)null_arg;
  endCpuCalcDP();
}


// Layout
void changeLayoutHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void resetLayoutHandlerA(GenericArg null_arg) {
  (void)null_arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetLayoutL(ws);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void toggleLayoutModHandlerA(GenericArg uint_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, ARG_INT_GET(uint_arg));
  addEnterNotifyMaskW(ws);
}

void increaseMasterLayoutHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void resizeMasterLayoutHandlerA(GenericArg float_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, ARG_FLOAT_GET(float_arg));
  addEnterNotifyMaskW(ws);
}


// Workspace
void changeToWorkspaceHandlerA(GenericArg int_arg) {
  processWorkspaceAction(changeToWorkspaceW, ARG_INT_GET(int_arg));
}

void changeToRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int dst = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  processWorkspaceAction(changeToWorkspaceW, dst);
}

void moveClientToWorkspaceHandlerA(GenericArg int_arg) {
  ClientPtrPtr c = getCurrClientCurrStackSS();
  const int ws = ARG_INT_GET(int_arg);
  processClientAction(sendClientW, c, selfC, (const void *)&ws);
}

void moveClientToRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  ClientPtrPtr c = getCurrClientCurrStackSS();
  const int ws = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  processClientAction(sendClientW, c, selfC, (const void *)&ws);
}

void restoreLastMinimizedHandlerA(GenericArg null_arg) {
  (void)null_arg;
  processWorkspaceAction(restoreLastMinimizedW, getCurrStackSS());
}

void toggleNSPHandlerA(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  ClientPtrPtr c = getCurrClientNSPStackSS();
  ClientPtrPtr nspc = findNSPClientSS();
  const int ws = getCurrStackSS();
  const int nspws = getNSPStackSS();
  if (nspc && CLI_GET(nspc).ws == ws) {
    processClientAction(sendClientW, nspc, selfC, (const void *)&nspws);
  } else if (nspc) {
    processClientAction(sendClientW, nspc, selfC, (const void *)&ws);
  } else {
    if (!getSizeNSPSS())
      spawnS(ARG_CMD_GET(command_arg), NULL);
    else
      processClientAction(sendClientW, c, selfC, (const void *)&ws);
  }
}


// Curr Client
void moveFocusCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(focusClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void swapCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(swapClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void killCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  killClientW(getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // processClientAction(killClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void tileCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(tileClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void normalCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(normalClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void fullScreenCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(fullScreenClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void toggleFullScreenCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(toggleFullScreenClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void minimizeCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(minimizeClientW, getCurrClientCurrStackSS(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void freeCurrClientHandlerA(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  processClientAction(freeClientW, getCurrClientCurrStackSS(), selfC, p);
}

void toggleFreeCurrClientHandlerA(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  processClientAction(toggleFreeClientW, getCurrClientCurrStackSS(), selfC, p);
}


// Ptr Client
void moveFocusPtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(focusClientW, getPtrClientW(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void freeMovePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(defFreeR);
  int rx, ry;
  processClientAction(freeMoveClientW, getPtrClientW(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
}

void freeResizePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(defFreeR);
  int rx, ry;
  processClientAction(freeResizeClientW, getPtrClientW(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
}

void movePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(floatMoveClientW, getPtrClientW(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void resizePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(floatResizeClientW, getPtrClientW(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void toggleFullScreenPtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(toggleFullScreenClientW, getPtrClientW(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void freePtrClientHandlerA(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  processClientAction(freeClientW, getPtrClientW(&rx, &ry), selfC, p);
}

void toggleFreePtrClientHandlerA(GenericArg freeSetterFn_arg)  {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  processClientAction(toggleFreeClientW, getPtrClientW(&rx, &ry), selfC, p);
}


// Main
void runActionA(const Action *a, const GenericMaybeArg *arg) {
  if (!a)
    return;
  if (arg && !MAYBE_ARG_IS_NOTHING(*arg))
    a->handler(MAYBE_ARG_GET_JUST(*arg));
  else
    a->handler(a->arg);
}

void runActionChainA(const ActionChain *ac) {
  if (!ac)
    return;
  int i;
  for (i = 0; ac->chain[ i ]; ++i)
    runActionA(ac->chain[ i ], &ac->arg);
}
