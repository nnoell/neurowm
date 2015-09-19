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
#include "core.h"
#include "dzen.h"
#include "neurowm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ActionClientFn
typedef void (*ActionClientFn)(ClientPtrPtr c, ClientSelectorFn csf, const void *data);

// ActionWorkspaceFn
typedef void (*ActionWorkspaceFn)(int ws);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void processWorkspaceAction(const ActionWorkspaceFn awsf, int ws) {
  assert(awsf);
  rmvEnterNotifyMaskW(ws);
  awsf(ws);
  addEnterNotifyMaskW(ws);
}

static void processClientAction(const ActionClientFn acf, ClientPtrPtr c, const ClientSelectorFn csf,
    const void *data) {
  assert(acf);
  assert(csf);
  if (!c)
    return;
  rmvEnterNotifyMaskW(CLI_GET(c).ws);
  acf(c, csf, data);
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
const Action const stopCpuCalcActionA[] = { { stopCpuCalcHandlerA, ARG_NULL } };
const Action const changeLayoutActionA[] = { { changeLayoutHandlerA, ARG_INT(1) } };
const Action const resetLayoutActionA[] = { { resetLayoutHandlerA, ARG_NULL } };
const Action const toggleLayoutActionA[] = { { toggleLayoutHandlerA, ARG_INT(0) } };
const Action const toggleModLayoutActionA[] = { { toggleModLayoutHandlerA, ARG_UINT(notModL) } };
const Action const increaseMasterLayoutActionA[] = { { increaseMasterLayoutHandlerA, ARG_INT(1) } };
const Action const resizeMasterLayoutActionA[] = { { resizeMasterLayoutHandlerA, ARG_FLOAT(1.0f) } };
const Action const changeWorkspaceActionA[] = { { changeWorkspaceHandlerA, ARG_INT(0) } };
const Action const changeRelWorkspaceActionA[] = { { changeRelWorkspaceHandlerA, ARG_WSF(nextW) } };
const Action const restoreLastMinimizedActionA[] = { { restoreLastMinimizedHandlerA, ARG_NULL } };
const Action const toggleNSPActionA[] = { { toggleNSPHandlerA, ARG_CMD(defNSPCommand) } };
const Action const focusCurrClientActionA[] = { { focusCurrClientHandlerA, ARG_CSF(nextC) } };
const Action const swapCurrClientActionA[] = { { swapCurrClientHandlerA, ARG_CSF(nextC) } };
const Action const sendCurrClientActionA[] = { { sendCurrClientHandlerA, ARG_INT(0) } };
const Action const sendCurrClientRelativeActionA[] = { { sendCurrClientRelativeHandlerA, ARG_WSF(nextW) } };
const Action const killCurrClientActionA[] = { { killCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const tileCurrClientActionA[] = { { tileCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const normalCurrClientActionA[] = { { normalCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const fullScreenCurrClientActionA[] = { { fullScreenCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const toggleFullScreenCurrClientActionA[] = { { toggleFullScreenCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const minimizeCurrClientActionA[] = { { minimizeCurrClientHandlerA, ARG_CSF(selfC) } };
const Action const freeCurrClientActionA[] = { { freeCurrClientHandlerA, ARG_FSF(defFreeR) } };
const Action const toggleFreeCurrClientActionA[] = { { toggleFreeCurrClientHandlerA, ARG_FSF(defFreeR) } };
const Action const focusPtrClientActionA[] = { { focusPtrClientHandlerA, ARG_CSF(selfC) } };
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
const Action *const stopCpuCalcA[] = { stopCpuCalcActionA, NULL };
const Action *const changeLayoutA[] = { changeLayoutActionA, NULL };
const Action *const resetLayoutA[] = { resetLayoutActionA, NULL };
const Action *const toggleLayoutA[] = { toggleLayoutActionA, NULL };
const Action *const toggleModLayoutA[] = { toggleModLayoutActionA, NULL };
const Action *const increaseMasterLayoutA[] = { increaseMasterLayoutActionA, NULL };
const Action *const resizeMasterLayoutA[] = { resizeMasterLayoutActionA, NULL };
const Action *const changeWorkspaceA[] = { changeWorkspaceActionA, NULL };
const Action *const changeRelWorkspaceA[] = { changeRelWorkspaceActionA, NULL };
const Action *const restoreLastMinimizedA[] = { restoreLastMinimizedActionA, NULL };
const Action *const toggleNSPA[] = { toggleNSPActionA, NULL };
const Action *const focusCurrClientA[] = { focusCurrClientActionA, NULL };
const Action *const swapCurrClientA[] = { swapCurrClientActionA, NULL };
const Action *const sendCurrClientA[] = { sendCurrClientActionA, NULL };
const Action *const sendCurrClientRelativeA[] = { sendCurrClientRelativeActionA, NULL };
const Action *const sendCurrClientFollowA[] = { sendCurrClientActionA, changeWorkspaceActionA, NULL };
const Action *const sendCurrClientRelativeFollowA[] = {
    sendCurrClientRelativeActionA, changeRelWorkspaceActionA, NULL };
const Action *const killCurrClientA[] = { killCurrClientActionA, NULL };
const Action *const tileCurrClientA[] = { tileCurrClientActionA, NULL };
const Action *const normalCurrClientA[] = { normalCurrClientActionA, NULL };
const Action *const fullScreenCurrClientA[] = { fullScreenCurrClientActionA, NULL };
const Action *const toggleFullScreenCurrClientA[] = { toggleFullScreenCurrClientActionA, NULL };
const Action *const minimizeCurrClientA[] = { minimizeCurrClientActionA, NULL };
const Action *const freeCurrClientA[] = { freeCurrClientActionA, NULL };
const Action *const toggleFreeCurrClientA[] = { toggleFreeCurrClientActionA, NULL };
const Action *const focusPtrClientA[] = { focusPtrClientActionA, NULL };
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
  quitN();
}

void reloadNeurowmHandlerA(GenericArg null_arg) {
  (void)null_arg;
  reloadN();
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
  startCpuCalcD();
}

void stopCpuCalcHandlerA(GenericArg null_arg) {
  (void)null_arg;
  stopCpuCalcD();
}

// Layout
void changeLayoutHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeL(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void resetLayoutHandlerA(GenericArg null_arg) {
  (void)null_arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetL(ws);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleL(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void toggleModLayoutHandlerA(GenericArg uint_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleModCurrL(ws, ARG_INT_GET(uint_arg));
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
void changeWorkspaceHandlerA(GenericArg int_arg) {
  processWorkspaceAction(changeW, ARG_INT_GET(int_arg));
}

void changeRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int dst = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  processWorkspaceAction(changeW, dst);
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
void focusCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(focusClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void swapCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(swapClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void sendCurrClientHandlerA(GenericArg int_arg) {
  const int ws = ARG_INT_GET(int_arg);
  processClientAction(sendClientW, getFocusedC(), selfC, (const void *)&ws);
}

void sendCurrClientRelativeHandlerA(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int ws = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  processClientAction(sendClientW, getFocusedC(), selfC, (const void *)&ws);
}

void killCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  killClientW(getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // processClientAction(killClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void tileCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(tileClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void normalCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(normalClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void fullScreenCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(fullScreenClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void toggleFullScreenCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(toggleFullScreenClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void minimizeCurrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  processClientAction(minimizeClientW, getFocusedC(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void freeCurrClientHandlerA(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  processClientAction(freeClientW, getFocusedC(), selfC, p);
}

void toggleFreeCurrClientHandlerA(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  processClientAction(toggleFreeClientW, getFocusedC(), selfC, p);
}

// Ptr Client
void focusPtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(focusClientW, getPointerC(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void freeMovePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(defFreeR);
  int rx, ry;
  processClientAction(freeMoveClientW, getPointerC(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
}

void freeResizePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(defFreeR);
  int rx, ry;
  processClientAction(freeResizeClientW, getPointerC(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
}

void movePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(floatMoveClientW, getPointerC(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void resizePtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(floatResizeClientW, getPointerC(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void toggleFullScreenPtrClientHandlerA(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processClientAction(toggleFullScreenClientW, getPointerC(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void freePtrClientHandlerA(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  processClientAction(freeClientW, getPointerC(&rx, &ry), selfC, p);
}

void toggleFreePtrClientHandlerA(GenericArg freeSetterFn_arg)  {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  processClientAction(toggleFreeClientW, getPointerC(&rx, &ry), selfC, p);
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
