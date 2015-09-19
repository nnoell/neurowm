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

static void process_workspace(const ActionWorkspaceFn awsf, int ws) {
  assert(awsf);
  rmvEnterNotifyMaskW(ws);
  awsf(ws);
  addEnterNotifyMaskW(ws);
}

static void process_client(const ActionClientFn acf, ClientPtrPtr c, const ClientSelectorFn csf,
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
const Action const NeuroActionNothing[] = { {
    NeuroActionHandlerNothing, ARG_NULL }};
const Action const NeuroActionQuit[] = { {
    NeuroActionHandlerQuit, ARG_NULL } };
const Action const NeuroActionReload[] = { {
    NeuroActionHandlerReload, ARG_NULL } };
const Action const NeuroActionStartCpuCalc[] = { {
    NeuroActionHandlerStartCpuCalc, ARG_NULL } };
const Action const NeuroActionSpawn[] = { {
    NeuroActionHandlerSpawn, ARG_CMD(NeuroConfigTerminalCommand) } };
const Action const NeuroActionSleep[] = { {
    NeuroActionHandlerSleep, ARG_INT(1) } };
const Action const NeuroActionStopCpuCalc[] = { {
    NeuroActionHandlerStopCpuCalc, ARG_NULL } };
const Action const NeuroActionChangeLayout[] = { {
    NeuroActionHandlerChangeLayout, ARG_INT(1) } };
const Action const NeuroActionResetLayout[] = { {
    NeuroActionHandlerResetLayout, ARG_NULL } };
const Action const NeuroActionToggleLayout[] = { {
    NeuroActionHandlerToggleLayout, ARG_INT(0) } };
const Action const NeuroActionToggleModLayout[] = { {
    NeuroActionHandlerToggleModLayout, ARG_UINT(nullModL) } };
const Action const NeuroActionIncreaseMasterLayout[] = { {
    NeuroActionHandlerIncreaseMasterLayout, ARG_INT(1) } };
const Action const NeuroActionResizeMasterLayout[] = { {
    NeuroActionHandlerResizeMasterLayout, ARG_FLOAT(1.0f) } };
const Action const NeuroActionChangeWorkspace[] = { {
    NeuroActionHandlerChangeWorkspace, ARG_WSF(nextW) } };
const Action const NeuroActionRestoreLastMinimized[] = { {
    NeuroActionHandlerRestoreLastMinimized, ARG_NULL } };
const Action const NeuroActionToggleScatchpad[] = { {
    NeuroActionHandlerToggleScratchpad, ARG_CMD(NeuroConfigScratchpadCommand) } };
const Action const NeuroActionFocusCurrClient[] = { {
    NeuroActionHandlerFocusCurrClient, ARG_CSF(NeuroClientSelectorNext) } };
const Action const NeuroActionSwapCurrClient[] = { {
    NeuroActionHandlerSwapCurrClient, ARG_CSF(NeuroClientSelectorNext) } };
const Action const NeuroActionSendCurrClient[] = { {
    NeuroActionHandlerSendCurrClient, ARG_WSF(nextW) } };
const Action const NeuroActionKillCurrClient[] = { {
    NeuroActionHandlerKillCurrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionTileCurrClient[] = { {
    NeuroActionHandlerTileCurrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionNormalCurrClient[] = { {
    NeuroActionHandlerNormalCurrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFullscreenCurrClient[] = { {
    NeuroActionHandlerFullscreenCurrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionToggleFullscreenCurrClient[] = { {
    NeuroActionHandlerToggleFullscreenCurrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionMinimizeCurrClient[] = { {
    NeuroActionHandlerMinimizeCurrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFreeCurrClient[] = { {
    NeuroActionHandlerFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterDefault) } };
const Action const NeuroActionToggleFreeCurrClient[] = { {
    NeuroActionHandlerToggleFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterDefault) } };
const Action const NeuroActionFocusPtrClient[] = { {
    NeuroActionHandlerFocusPtrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFreeMovePtrClient[] = { {
    NeuroActionHandlerFreeMovePtrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFreeResizePtrClient[] = { {
    NeuroActionHandlerFreeResizePtrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFloatMovePtrClient[] = { {
    NeuroActionHandlerFloatMovePtrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFloatResizePtrClient[] = { {
    NeuroActionHandlerFloatResizePtrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionToggleFullscreenPtrClient[] = { {
    NeuroActionHandlerToggleFullscreenPtrClient, ARG_CSF(NeuroClientSelectorSelf) } };
const Action const NeuroActionFreePtrClient[] = { {
    NeuroActionHandlerFreePtrClient, ARG_FSF(NeuroRuleFreeSetterDefault) } };
const Action const NeuroActionToggleFreePtrClient[] = { {
    NeuroActionHandlerToggleFreePtrClient, ARG_FSF(NeuroRuleFreeSetterDefault) } };

// Action Chains
const Action *const NeuroActionChainNothing[] = {
    NeuroActionNothing, NULL };
const Action *const NeuroActionChainQuit[] = {
    NeuroActionQuit, NULL };
const Action *const NeuroActionChainReload[] = {
    NeuroActionReload, NULL };
const Action *const NeuroActionChainStartCpuCalc[] = {
    NeuroActionStartCpuCalc, NULL };
const Action *const NeuroActionChainSpawn[] = {
    NeuroActionSpawn, NULL };
const Action *const NeuroActionChainSleep[] = {
    NeuroActionSleep, NULL };
const Action *const NeuroActionChainStopCpuCalc[] = {
    NeuroActionStopCpuCalc, NULL };
const Action *const NeuroActionChainChangeLayout[] = {
    NeuroActionChangeLayout, NULL };
const Action *const NeuroActionChainResetLayout[] = {
    NeuroActionResetLayout, NULL };
const Action *const NeuroActionChainToggleLayout[] = {
    NeuroActionToggleLayout, NULL };
const Action *const NeuroActionChainToggleModLayout[] = {
    NeuroActionToggleModLayout, NULL };
const Action *const NeuroActionChainIncreaseMasterLayout[] = {
    NeuroActionIncreaseMasterLayout, NULL };
const Action *const NeuroActionChainResizeMasterLayout[] = {
    NeuroActionResizeMasterLayout, NULL };
const Action *const NeuroActionChainChangeWorkspace[] = {
    NeuroActionChangeWorkspace, NULL };
const Action *const NeuroActionChainRestoreLastMinimized[] = {
    NeuroActionRestoreLastMinimized, NULL };
const Action *const NeuroActionChainToggleScratchpad[] = {
    NeuroActionToggleScatchpad, NULL };
const Action *const NeuroActionChainFocusCurrClient[] = {
    NeuroActionFocusCurrClient, NULL };
const Action *const NeuroActionChainSwapCurrClient[] = {
    NeuroActionSwapCurrClient, NULL };
const Action *const NeuroActionChainSendCurrClient[] = {
    NeuroActionSendCurrClient, NULL };
const Action *const NeuroActionChainSendFollowCurrClient[] = {
    NeuroActionSendCurrClient, NeuroActionChangeWorkspace, NULL };
const Action *const NeuroActionChainKillClient[] = {
    NeuroActionKillCurrClient, NULL };
const Action *const NeuroActionChainTileCurrClient[] = {
    NeuroActionTileCurrClient, NULL };
const Action *const NeuroActionChainNormalCurrClient[] = {
    NeuroActionNormalCurrClient, NULL };
const Action *const NeuroActionChainFullscreenCurrClient[] = {
    NeuroActionFullscreenCurrClient, NULL };
const Action *const NeuroActionChainToggleFullscreenCurrClient[] = {
    NeuroActionToggleFullscreenCurrClient, NULL };
const Action *const NeuroActionChainMinimizeCurrClient[] = {
    NeuroActionMinimizeCurrClient, NULL };
const Action *const NeuroActionChainFreeCurrClient[] = {
    NeuroActionFreeCurrClient, NULL };
const Action *const NeuroActionChainToggleFreeCurrClient[] = {
    NeuroActionToggleFreeCurrClient, NULL };
const Action *const NeuroActionChainFocusPtrClient[] = {
    NeuroActionFocusPtrClient, NULL };
const Action *const NeuroActionChainFreeMovePtrClient[] = {
    NeuroActionFreeMovePtrClient, NULL };
const Action *const NeuroActionChainFreeResizePtrClient[] = {
    NeuroActionFreeResizePtrClient, NULL };
const Action *const NeuroActionChainFloatMovePtrClient[] = {
    NeuroActionFloatMovePtrClient, NULL };
const Action *const NeuroActionChainFloatResizePtrClient[] = {
    NeuroActionFloatResizePtrClient, NULL };
const Action *const NeuroActionChainToggleFullscreenPtrClient[] = {
    NeuroActionToggleFullscreenPtrClient, NULL };
const Action *const NeuroActionChainFreePtrClient[] = {
    NeuroActionFreePtrClient, NULL };
const Action *const NeuroActionChainToggleFreePtrClient[] = {
    NeuroActionToggleFreePtrClient, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void NeuroActionHandlerNothing(GenericArg null_arg) {
  (void)null_arg;
}

void NeuroActionHandlerQuit(GenericArg null_arg) {
  (void)null_arg;
  NeuroNeurowmQuit();
}

void NeuroActionHandlerReload(GenericArg null_arg) {
  (void)null_arg;
  NeuroNeurowmReload();
}

void NeuroActionHandlerChangeNeurowmName(GenericArg string_arg) {
  assert(ARG_STR_GET(string_arg));
  NeuroSystemChangeNeurowmName(ARG_STR_GET(string_arg));
}

void NeuroActionHandlerSpawn(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  NeuroSystemSpawn(ARG_CMD_GET(command_arg), NULL);
}

void NeuroActionHandlerSleep(GenericArg int_arg) {
  assert(ARG_INT_GET(int_arg) >= 0);
  sleep(ARG_INT_GET(int_arg));
}

void NeuroActionHandlerStartCpuCalc(GenericArg null_arg) {
  (void)null_arg;
  NeuroDzenStartCpuCalc();
}

void NeuroActionHandlerStopCpuCalc(GenericArg null_arg) {
  (void)null_arg;
  NeuroDzenStopCpuCalc();
}

// Layout
void NeuroActionHandlerChangeLayout(GenericArg int_arg) {
  const int ws = NeuroCoreGetCurrStack();
  rmvEnterNotifyMaskW(ws);
  NeuroLayoutChange(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void NeuroActionHandlerResetLayout(GenericArg null_arg) {
  (void)null_arg;
  const int ws = NeuroCoreGetCurrStack();
  rmvEnterNotifyMaskW(ws);
  NeuroLayoutReset(ws);
  addEnterNotifyMaskW(ws);
}

void NeuroActionHandlerToggleLayout(GenericArg int_arg) {
  const int ws = NeuroCoreGetCurrStack();
  rmvEnterNotifyMaskW(ws);
  NeuroLayoutToggle(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void NeuroActionHandlerToggleModLayout(GenericArg uint_arg) {
  const int ws = NeuroCoreGetCurrStack();
  rmvEnterNotifyMaskW(ws);
  NeuroLayoutToggleModCurr(ws, ARG_INT_GET(uint_arg));
  addEnterNotifyMaskW(ws);
}

void NeuroActionHandlerIncreaseMasterLayout(GenericArg int_arg) {
  const int ws = NeuroCoreGetCurrStack();
  rmvEnterNotifyMaskW(ws);
  NeuroLayoutIncreaseMaster(ws, ARG_INT_GET(int_arg));
  addEnterNotifyMaskW(ws);
}

void NeuroActionHandlerResizeMasterLayout(GenericArg float_arg) {
  const int ws = NeuroCoreGetCurrStack();
  rmvEnterNotifyMaskW(ws);
  NeuroLayoutResizeMaster(ws, ARG_FLOAT_GET(float_arg));
  addEnterNotifyMaskW(ws);
}

// Workspace
void NeuroActionHandlerChangeWorkspace(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int dst = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  process_workspace(changeW, dst);
}

void NeuroActionHandlerRestoreLastMinimized(GenericArg null_arg) {
  (void)null_arg;
  process_workspace(restoreLastMinimizedW, NeuroCoreGetCurrStack());
}

void NeuroActionHandlerToggleScratchpad(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  ClientPtrPtr c = NeuroCoreGetCurrClientNspStack();
  ClientPtrPtr nspc = NeuroCoreFindNspClient();
  const int ws = NeuroCoreGetCurrStack();
  const int nspws = NeuroCoreGetNspStack();
  if (nspc && CLI_GET(nspc).ws == ws) {
    process_client(sendClientW, nspc, NeuroClientSelectorSelf, (const void *)&nspws);
  } else if (nspc) {
    process_client(sendClientW, nspc, NeuroClientSelectorSelf, (const void *)&ws);
  } else {
    if (!NeuroCoreGetNspStackSize())
      NeuroSystemSpawn(ARG_CMD_GET(command_arg), NULL);
    else
      process_client(sendClientW, c, NeuroClientSelectorSelf, (const void *)&ws);
  }
}

// Curr Client
void NeuroActionHandlerFocusCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(focusClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSwapCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(swapClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSendCurrClient(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int ws = ARG_WSF_GET(WorkspaceSelectorFn_arg)() % NeuroCoreGetSize();
  process_client(sendClientW, NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
}

void NeuroActionHandlerKillCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  killClientW(NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(killClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerTileCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(tileClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerNormalCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(normalClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFullscreenCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(fullScreenClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerToggleFullscreenCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(toggleFullScreenClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerMinimizeCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(minimizeClientW, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeCurrClient(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  process_client(freeClientW, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreeCurrClient(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  process_client(toggleFreeClientW, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

// Ptr Client
void NeuroActionHandlerFocusPtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  process_client(focusClientW, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeMovePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  process_client(freeMoveClientW, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
}

void NeuroActionHandlerFreeResizePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  process_client(freeResizeClientW, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
}

void NeuroActionHandlerFloatMovePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  process_client(floatMoveClientW, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFloatResizePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  process_client(floatResizeClientW, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerToggleFullscreenPtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  process_client(toggleFullScreenClientW, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreePtrClient(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  process_client(freeClientW, NeuroClientGetPointed(&rx, &ry), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreePtrClient(GenericArg freeSetterFn_arg)  {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  process_client(toggleFreeClientW, NeuroClientGetPointed(&rx, &ry), NeuroClientSelectorSelf, p);
}

// Util
void NeuroActionUtilRunAction(const Action *a, const GenericMaybeArg *arg) {
  if (!a)
    return;
  if (arg && !MAYBE_ARG_IS_NOTHING(*arg))
    a->handler(MAYBE_ARG_GET_JUST(*arg));
  else
    a->handler(a->arg);
}

void NeuroActionUtilRunActionChain(const ActionChain *ac) {
  if (!ac)
    return;
  int i;
  for (i = 0; ac->chain[ i ]; ++i)
    NeuroActionUtilRunAction(ac->chain[ i ], &ac->arg);
}
