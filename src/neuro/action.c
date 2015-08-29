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
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void processWSActionA(const GenericWSActionFn gwsaf, int ws) {
  assert(gwsaf);
  rmvEnterNotifyMaskW(ws);
  gwsaf(ws);
  addEnterNotifyMaskW(ws);
}

static void processCliActionA(const GenericCliActionFn gcaf, CliPtr c, const ClientSelectorFn csf, const void *data) {
  assert(gcaf);
  assert(csf);
  if (!c)
    return;
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  gcaf(c, csf, data);
  addEnterNotifyMaskW(CLIVAL(c).ws);
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
const Action const freeCurrClientActionA[] = { { freeCurrClientHandlerA, ARG_FLF(defFreeR) } };
const Action const toggleFreeCurrClientActionA[] = { { toggleFreeCurrClientHandlerA, ARG_FLF(defFreeR) } };
const Action const moveFocusPtrClientActionA[] = { { moveFocusPtrClientHandlerA, ARG_CSF(selfC) } };
const Action const freeMovePtrClientActionA[] = { { freeMovePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const freeResizePtrClientActionA[] = { { freeResizePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const movePtrClientActionA[] = { { movePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const resizePtrClientActionA[] = { { resizePtrClientHandlerA, ARG_CSF(selfC) } };
const Action const toggleFullScreenPtrClientActionA[] = { { toggleFullScreenPtrClientHandlerA, ARG_CSF(selfC) } };
const Action const freePtrClientActionA[] = { { freePtrClientHandlerA, ARG_FLF(defFreeR) } };
const Action const toggleFreePtrClientActionA[] = { { toggleFreePtrClientHandlerA, ARG_FLF(defFreeR) } };

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
  assert(string_arg.string_);
  changeWMNameS(string_arg.string_);
}

void spawnHandlerA(GenericArg command_arg) {
  assert(command_arg.command_);
  spawnS(command_arg.command_, NULL);
}

void sleepHandlerA(GenericArg int_arg) {
  assert(int_arg.int_ >= 0);
  sleep(int_arg.int_);
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
  changeLayoutL(ws, int_arg.int_);
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
  togLayoutL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutModHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, int_arg.uint_);
  addEnterNotifyMaskW(ws);
}

void increaseMasterLayoutHandlerA(GenericArg int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void resizeMasterLayoutHandlerA(GenericArg float_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, float_arg.float_);
  addEnterNotifyMaskW(ws);
}


// Workspace
void changeToWorkspaceHandlerA(GenericArg int_arg) {
  processWSActionA(changeToWorkspaceW, int_arg.int_);
}

void changeToRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  processWSActionA(changeToWorkspaceW, WorkspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_());
}

void moveClientToWorkspaceHandlerA(GenericArg int_arg) {
  CliPtr c = getCurrClientCurrStackSS();
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  moveClientToWorkspaceW(c, int_arg.int_);
  addEnterNotifyMaskW(CLIVAL(c).ws);
}

void moveClientToRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  CliPtr c = getCurrClientCurrStackSS();
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  moveClientToWorkspaceW(c, WorkspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_());
  addEnterNotifyMaskW(CLIVAL(c).ws);
}

void restoreLastMinimizedHandlerA(GenericArg null_arg) {
  (void)null_arg;
  processWSActionA(restoreLastMinimizedW, getCurrStackSS());
}

void toggleNSPHandlerA(GenericArg command_arg) {
  assert(command_arg.command_);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  CliPtr c = findNSPClientSS();
  if (c && CLIVAL(c).ws == ws) {
    moveClientToWorkspaceW(c, getNSPStackSS());
  } else if (c) {
    moveClientToWorkspaceW(c, ws);
  } else {
    if (!getSizeNSPSS())
      spawnS(command_arg.command_, NULL);
    else
      moveClientToWorkspaceW(getCurrClientNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}


// Curr Client
void moveFocusCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(moveFocusClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void swapCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(swapClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void killCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  killClientW(getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
  // processCliActionA(killClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void tileCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(tileClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void normalCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(normalClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void fullScreenCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(fullScreenClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void toggleFullScreenCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(toggleFullScreenClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void minimizeCurrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(minimizeClientW, getCurrClientCurrStackSS(), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void freeCurrClientHandlerA(GenericArg freeSetterFn_arg) {
  processCliActionA(freeClientW, getCurrClientCurrStackSS(), selfC, (const void *)&freeSetterFn_arg.GenericArgFn_);
}

void toggleFreeCurrClientHandlerA(GenericArg freeSetterFn_arg) {
  processCliActionA(toggleFreeClientW, getCurrClientCurrStackSS(), selfC, (const void *)&freeSetterFn_arg.GenericArgFn_);
}


// Ptr Client
void moveFocusPtrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(moveFocusClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void freeMovePtrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(freeMoveClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void freeResizePtrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(freeResizeClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void movePtrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(moveClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void resizePtrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(resizeClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void toggleFullScreenPtrClientHandlerA(GenericArg selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(toggleFullScreenClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.GenericArgFn_.ClientSelectorFn_, NULL);
}

void freePtrClientHandlerA(GenericArg freeSetterFn_arg) {
  int rx, ry;
  processCliActionA(freeClientW, getPtrClientW(&rx, &ry), selfC, (const void *)&freeSetterFn_arg.GenericArgFn_);
}

void toggleFreePtrClientHandlerA(GenericArg freeSetterFn_arg)  {
  int rx, ry;
  processCliActionA(toggleFreeClientW, getPtrClientW(&rx, &ry), selfC, (const void *)&freeSetterFn_arg.GenericArgFn_);
}


// Main
void runActionA(const Action *a, const ActionChainAr *arg) {
  if (!a)
    return;
  if (arg && arg->use)
    a->handler(arg->arg);
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