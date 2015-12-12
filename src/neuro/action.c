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
#include "wm.h"


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
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  awsf(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

static void process_client(const ActionClientFn acf, ClientPtrPtr c, const ClientSelectorFn csf,
    const void *data) {
  assert(acf);
  assert(csf);
  if (!c)
    return;
  NeuroWorkspaceRemoveEnterNotifyMask(CLI_GET(c).ws);
  acf(c, csf, data);
  NeuroWorkspaceAddEnterNotifyMask(CLI_GET(c).ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Actions
const Action NeuroActionNothing = {
    NeuroActionHandlerNothing, ARG_NULL };
const Action NeuroActionQuit = {
    NeuroActionHandlerQuit, ARG_NULL };
const Action NeuroActionReload = {
    NeuroActionHandlerReload, ARG_NULL };
const Action NeuroActionInitCpuCalc = {
    NeuroActionHandlerInitCpuCalc, ARG_NULL };
const Action NeuroActionSpawn = {
    NeuroActionHandlerSpawn, ARG_CMD(NeuroConfigDefaultTerminalCommand) };
const Action NeuroActionSleep = {
    NeuroActionHandlerSleep, ARG_INT(1) };
const Action NeuroActionStopCpuCalc = {
    NeuroActionHandlerStopCpuCalc, ARG_NULL };
const Action NeuroActionChangeLayout = {
    NeuroActionHandlerChangeLayout, ARG_INT(1) };
const Action NeuroActionResetLayout = {
    NeuroActionHandlerResetLayout, ARG_NULL };
const Action NeuroActionToggleLayout = {
    NeuroActionHandlerToggleLayout, ARG_INT(0) };
const Action NeuroActionToggleModLayout = {
    NeuroActionHandlerToggleModLayout, ARG_LMOD(LayoutModNull) };
const Action NeuroActionIncreaseMasterLayout = {
    NeuroActionHandlerIncreaseMasterLayout, ARG_INT(1) };
const Action NeuroActionResizeMasterLayout = {
    NeuroActionHandlerResizeMasterLayout, ARG_FLOAT(1.0f) };
const Action NeuroActionChangeWorkspace = {
    NeuroActionHandlerChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorNext) };
const Action NeuroActionRestoreLastMinimized = {
    NeuroActionHandlerRestoreLastMinimized, ARG_NULL };
const Action NeuroActionToggleScatchpad = {
    NeuroActionHandlerToggleScratchpad, ARG_CMD(NeuroConfigDefaultLauncherCommand) };
const Action NeuroActionFocusCurrClient = {
    NeuroActionHandlerFocusCurrClient, ARG_CSF(NeuroClientSelectorNext) };
const Action NeuroActionSwapCurrClient = {
    NeuroActionHandlerSwapCurrClient, ARG_CSF(NeuroClientSelectorNext) };
const Action NeuroActionSendCurrClient = {
    NeuroActionHandlerSendCurrClient, ARG_WSF(NeuroWorkspaceSelectorNext) };
const Action NeuroActionKillCurrClient = {
    NeuroActionHandlerKillCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionTileCurrClient = {
    NeuroActionHandlerTileCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionNormalCurrClient = {
    NeuroActionHandlerNormalCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFullscreenCurrClient = {
    NeuroActionHandlerFullscreenCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionToggleFullscreenCurrClient = {
    NeuroActionHandlerToggleFullscreenCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionMinimizeCurrClient = {
    NeuroActionHandlerMinimizeCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFreeCurrClient = {
    NeuroActionHandlerFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };
const Action NeuroActionToggleFreeCurrClient = {
    NeuroActionHandlerToggleFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };
const Action NeuroActionFocusPtrClient = {
    NeuroActionHandlerFocusPtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFreeMovePtrClient = {
    NeuroActionHandlerFreeMovePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFreeResizePtrClient = {
    NeuroActionHandlerFreeResizePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFloatMovePtrClient = {
    NeuroActionHandlerFloatMovePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFloatResizePtrClient = {
    NeuroActionHandlerFloatResizePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionToggleFullscreenPtrClient = {
    NeuroActionHandlerToggleFullscreenPtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const Action NeuroActionFreePtrClient = {
    NeuroActionHandlerFreePtrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };
const Action NeuroActionToggleFreePtrClient = {
    NeuroActionHandlerToggleFreePtrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };

// Action Sets
const Action* NeuroActionSetNothing[] = { &NeuroActionNothing, NULL };
const Action* NeuroActionSetQuit[] = { &NeuroActionQuit, NULL };
const Action* NeuroActionSetReload[] = { &NeuroActionReload, NULL };
const Action* NeuroActionSetInitCpuCalc[] = { &NeuroActionInitCpuCalc, NULL };
const Action* NeuroActionSetSpawn[] = { &NeuroActionSpawn, NULL };
const Action* NeuroActionSetSleep[] = { &NeuroActionSleep, NULL };
const Action* NeuroActionSetStopCpuCalc[] = { &NeuroActionStopCpuCalc, NULL };
const Action* NeuroActionSetChangeLayout[] = { &NeuroActionChangeLayout, NULL };
const Action* NeuroActionSetResetLayout[] = { &NeuroActionResetLayout, NULL };
const Action* NeuroActionSetToggleLayout[] = { &NeuroActionToggleLayout, NULL };
const Action* NeuroActionSetToggleModLayout[] = { &NeuroActionToggleModLayout, NULL };
const Action* NeuroActionSetIncreaseMasterLayout[] = { &NeuroActionIncreaseMasterLayout, NULL };
const Action* NeuroActionSetResizeMasterLayout[] = { &NeuroActionResizeMasterLayout, NULL };
const Action* NeuroActionSetChangeWorkspace[] = { &NeuroActionChangeWorkspace, NULL };
const Action* NeuroActionSetRestoreLastMinimized[] = { &NeuroActionRestoreLastMinimized, NULL };
const Action* NeuroActionSetToggleScratchpad[] = { &NeuroActionToggleScatchpad, NULL };
const Action* NeuroActionSetFocusCurrClient[] = { &NeuroActionFocusCurrClient, NULL };
const Action* NeuroActionSetSwapCurrClient[] = { &NeuroActionSwapCurrClient, NULL };
const Action* NeuroActionSetSendCurrClient[] = { &NeuroActionSendCurrClient, NULL };
const Action* NeuroActionSetSendFollowCurrClient[] = { &NeuroActionSendCurrClient, &NeuroActionChangeWorkspace, NULL };
const Action* NeuroActionSetKillClient[] = { &NeuroActionKillCurrClient, NULL };
const Action* NeuroActionSetTileCurrClient[] = { &NeuroActionTileCurrClient, NULL };
const Action* NeuroActionSetNormalCurrClient[] = { &NeuroActionNormalCurrClient, NULL };
const Action* NeuroActionSetFullscreenCurrClient[] = { &NeuroActionFullscreenCurrClient, NULL };
const Action* NeuroActionSetToggleFullscreenCurrClient[] = { &NeuroActionToggleFullscreenCurrClient, NULL };
const Action* NeuroActionSetMinimizeCurrClient[] = { &NeuroActionMinimizeCurrClient, NULL };
const Action* NeuroActionSetFreeCurrClient[] = { &NeuroActionFreeCurrClient, NULL };
const Action* NeuroActionSetToggleFreeCurrClient[] = { &NeuroActionToggleFreeCurrClient, NULL };
const Action* NeuroActionSetFocusPtrClient[] = { &NeuroActionFocusPtrClient, NULL };
const Action* NeuroActionSetFreeMovePtrClient[] = { &NeuroActionFreeMovePtrClient, NULL };
const Action* NeuroActionSetFreeResizePtrClient[] = { &NeuroActionFreeResizePtrClient, NULL };
const Action* NeuroActionSetFloatMovePtrClient[] = { &NeuroActionFloatMovePtrClient, NULL };
const Action* NeuroActionSetFloatResizePtrClient[] = { &NeuroActionFloatResizePtrClient, NULL };
const Action* NeuroActionSetToggleFullscreenPtrClient[] = { &NeuroActionToggleFullscreenPtrClient, NULL };
const Action* NeuroActionSetFreePtrClient[] = { &NeuroActionFreePtrClient, NULL };
const Action* NeuroActionSetToggleFreePtrClient[] = { &NeuroActionToggleFreePtrClient, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void NeuroActionHandlerNothing(GenericArg null_arg) {
  (void)null_arg;
}

void NeuroActionHandlerQuit(GenericArg null_arg) {
  (void)null_arg;
  NeuroWmQuit();
}

void NeuroActionHandlerReload(GenericArg null_arg) {
  (void)null_arg;
  NeuroWmReload();
}

void NeuroActionHandlerChangeWmName(GenericArg string_arg) {
  assert(ARG_STR_GET(string_arg));
  NeuroSystemChangeWmName(ARG_STR_GET(string_arg));
}

void NeuroActionHandlerSpawn(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  NeuroSystemSpawn(ARG_CMD_GET(command_arg), NULL);
}

void NeuroActionHandlerSleep(GenericArg int_arg) {
  assert(ARG_INT_GET(int_arg) >= 0);
  sleep(ARG_INT_GET(int_arg));
}

void NeuroActionHandlerInitCpuCalc(GenericArg null_arg) {
  (void)null_arg;
  NeuroDzenInitCpuCalc();
}

void NeuroActionHandlerStopCpuCalc(GenericArg null_arg) {
  (void)null_arg;
  NeuroDzenStopCpuCalc();
}

// Layout
void NeuroActionHandlerChangeLayout(GenericArg int_arg) {
  const int ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutChange(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResetLayout(GenericArg null_arg) {
  (void)null_arg;
  const int ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutReset(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleLayout(GenericArg int_arg) {
  const int ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggle(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleModLayout(GenericArg uint_arg) {
  const int ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggleModCurr(ws, ARG_INT_GET(uint_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerIncreaseMasterLayout(GenericArg int_arg) {
  const int ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutIncreaseMaster(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResizeMasterLayout(GenericArg float_arg) {
  const int ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutResizeMaster(ws, ARG_FLOAT_GET(float_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

// Workspace
void NeuroActionHandlerChangeWorkspace(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int dst = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  process_workspace(NeuroWorkspaceChange, dst);
}

void NeuroActionHandlerRestoreLastMinimized(GenericArg null_arg) {
  (void)null_arg;
  process_workspace(NeuroWorkspaceRestoreLastMinimized, NeuroCoreGetCurrStack());
}

void NeuroActionHandlerToggleScratchpad(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  ClientPtrPtr c = NeuroCoreGetCurrClientNspStack();
  ClientPtrPtr nspc = NeuroCoreFindNspClient();
  const int ws = NeuroCoreGetCurrStack();
  const int nspws = NeuroCoreGetNspStack();
  if (nspc && CLI_GET(nspc).ws == ws) {
    process_client(NeuroWorkspaceClientSend, nspc, NeuroClientSelectorSelf, (const void *)&nspws);
  } else if (nspc) {
    process_client(NeuroWorkspaceClientSend, nspc, NeuroClientSelectorSelf, (const void *)&ws);
  } else {
    if (!NeuroCoreGetNspStackSize())
      NeuroSystemSpawn(ARG_CMD_GET(command_arg), NULL);
    else
      process_client(NeuroWorkspaceClientSend, c, NeuroClientSelectorSelf, (const void *)&ws);
  }
}

// Curr Client
void NeuroActionHandlerFocusCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientFocus, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSwapCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientSwap, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSendCurrClient(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.ArgFn_.WorkspaceSelectorFn_);
  const int ws = ARG_WSF_GET(WorkspaceSelectorFn_arg)() % NeuroCoreGetSize();
  process_client(NeuroWorkspaceClientSend, NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
}

void NeuroActionHandlerKillCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  NeuroWorkspaceClientKill(NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientKill, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerTileCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientTile, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerNormalCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientNormal, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFullscreenCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientFullscreen, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerToggleFullscreenCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg),
      NULL);
}

void NeuroActionHandlerMinimizeCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  process_client(NeuroWorkspaceClientMinimize, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeCurrClient(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientFree, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreeCurrClient(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientToggleFree, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

// Ptr Client
void NeuroActionHandlerFocusPtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  process_client(NeuroWorkspaceClientFocus, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeMovePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  NeuroWorkspaceClientFreeMove(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeMove, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    (const void *)&fsf);
}

void NeuroActionHandlerFreeResizePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  GenericArg fsf = (GenericArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  NeuroWorkspaceClientFreeResize(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeResize, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    (const void *)&fsf);
}

void NeuroActionHandlerFloatMovePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  NeuroWorkspaceClientFloatMove(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatMove, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    NULL);
}

void NeuroActionHandlerFloatResizePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  NeuroWorkspaceClientFloatResize(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatResize, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    NULL);
}

void NeuroActionHandlerToggleFullscreenPtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.argfn_.selectCliFn);
  int rx, ry;
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetPointed(&rx, &ry),
      ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreePtrClient(GenericArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  process_client(NeuroWorkspaceClientFree, NeuroClientGetPointed(&rx, &ry), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreePtrClient(GenericArg freeSetterFn_arg)  {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  process_client(NeuroWorkspaceClientToggleFree, NeuroClientGetPointed(&rx, &ry), NeuroClientSelectorSelf, p);
}

// Util
void NeuroActionRunAction(const Action *a, const GenericMaybeArg *arg) {
  if (!a)
    return;
  if (arg && !MAYBE_ARG_IS_NOTHING(*arg))
    a->handler(MAYBE_ARG_GET_JUST(*arg));
  else
    a->handler(a->arg);
}

void NeuroActionRunActionChain(const ActionChain *ac) {
  if (!ac || !ac->action_set)
    return;
  int i;
  for (i = 0; ac->action_set[ i ]; ++i)
    NeuroActionRunAction(ac->action_set[ i ], &ac->arg);
}
