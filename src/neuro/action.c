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
#include "action.h"
#include "system.h"
#include "core.h"
#include "dzen.h"
#include "wm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ActionClientFn
typedef void (*ActionClientFn)(NeuroClientPtrPtr c, NeuroClientSelectorFn csf, const void *data);

// ActionWorkspaceFn
typedef void (*ActionWorkspaceFn)(NeuroIndex ws);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void process_workspace(const ActionWorkspaceFn awsf, NeuroIndex ws) {
  assert(awsf);
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  awsf(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

static void process_client(const ActionClientFn acf, NeuroClientPtrPtr c, const NeuroClientSelectorFn csf,
  const void *data) {
  assert(acf);
  assert(csf);
  if (!c)
    return;
  const NeuroIndex ws = NEURO_CLIENT_PTR(c)->ws;
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  acf(c, csf, data);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Actions
const NeuroAction NeuroActionNothing = {
    NeuroActionHandlerNothing, NEURO_ARG_NULL };
const NeuroAction NeuroActionQuit = {
    NeuroActionHandlerQuit, NEURO_ARG_NULL };
const NeuroAction NeuroActionReload = {
    NeuroActionHandlerReload, NEURO_ARG_NULL };
const NeuroAction NeuroActionInitCpuCalc = {
    NeuroActionHandlerInitCpuCalc, NEURO_ARG_NULL };
const NeuroAction NeuroActionSpawn = {
    NeuroActionHandlerSpawn, NEURO_ARG_CMD(NeuroConfigDefaultTerminalCommand) };
const NeuroAction NeuroActionSleep = {
    NeuroActionHandlerSleep, NEURO_ARG_UINT(1) };
const NeuroAction NeuroActionStopCpuCalc = {
    NeuroActionHandlerStopCpuCalc, NEURO_ARG_NULL };
const NeuroAction NeuroActionChangeLayout = {
    NeuroActionHandlerChangeLayout, NEURO_ARG_INT(1) };
const NeuroAction NeuroActionResetLayout = {
    NeuroActionHandlerResetLayout, NEURO_ARG_NULL };
const NeuroAction NeuroActionToggleLayout = {
    NeuroActionHandlerToggleLayout, NEURO_ARG_IDX(0U) };
const NeuroAction NeuroActionToggleModLayout = {
    NeuroActionHandlerToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_NULL) };
const NeuroAction NeuroActionIncreaseMasterLayout = {
    NeuroActionHandlerIncreaseMasterLayout, NEURO_ARG_INT(1) };
const NeuroAction NeuroActionResizeMasterLayout = {
    NeuroActionHandlerResizeMasterLayout, NEURO_ARG_FLOAT(1.0f) };
const NeuroAction NeuroActionChangeWorkspace = {
    NeuroActionHandlerChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorNext) };
const NeuroAction NeuroActionSelectMonitor = {
    NeuroActionHandlerSelectMonitor, NEURO_ARG_MSF(NeuroMonitorSelectorHead) };
const NeuroAction NeuroActionRestoreLastMinimized = {
    NeuroActionHandlerRestoreLastMinimized, NEURO_ARG_NULL };
const NeuroAction NeuroActionToggleScatchpad = {
    NeuroActionHandlerToggleScratchpad, NEURO_ARG_CMD(NeuroConfigDefaultLauncherCommand) };
const NeuroAction NeuroActionFocusCurrClient = {
    NeuroActionHandlerFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext) };
const NeuroAction NeuroActionSwapCurrClient = {
    NeuroActionHandlerSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext) };
const NeuroAction NeuroActionSendCurrClient = {
    NeuroActionHandlerSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelectorNext) };
const NeuroAction NeuroActionKillCurrClient = {
    NeuroActionHandlerKillCurrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionTileCurrClient = {
    NeuroActionHandlerTileCurrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionNormalCurrClient = {
    NeuroActionHandlerNormalCurrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFullscreenCurrClient = {
    NeuroActionHandlerFullscreenCurrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionToggleFullscreenCurrClient = {
    NeuroActionHandlerToggleFullscreenCurrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionMinimizeCurrClient = {
    NeuroActionHandlerMinimizeCurrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreeCurrClient = {
    NeuroActionHandlerFreeCurrClient, NEURO_ARG_FSF(NeuroRuleFreeSetterDefault) };
const NeuroAction NeuroActionToggleFreeCurrClient = {
    NeuroActionHandlerToggleFreeCurrClient, NEURO_ARG_FSF(NeuroRuleFreeSetterDefault) };
const NeuroAction NeuroActionFocusPtrClient = {
    NeuroActionHandlerFocusPtrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreeMovePtrClient = {
    NeuroActionHandlerFreeMovePtrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreeResizePtrClient = {
    NeuroActionHandlerFreeResizePtrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFloatMovePtrClient = {
    NeuroActionHandlerFloatMovePtrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFloatResizePtrClient = {
    NeuroActionHandlerFloatResizePtrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionToggleFullscreenPtrClient = {
    NeuroActionHandlerToggleFullscreenPtrClient, NEURO_ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreePtrClient = {
    NeuroActionHandlerFreePtrClient, NEURO_ARG_FSF(NeuroRuleFreeSetterDefault) };
const NeuroAction NeuroActionToggleFreePtrClient = {
    NeuroActionHandlerToggleFreePtrClient, NEURO_ARG_FSF(NeuroRuleFreeSetterDefault) };

// Action Lists
const NeuroAction* NeuroActionListNothing[] = {
    &NeuroActionNothing, NULL };
const NeuroAction* NeuroActionListQuit[] = {
    &NeuroActionQuit, NULL };
const NeuroAction* NeuroActionListReload[] = {
    &NeuroActionReload, NULL };
const NeuroAction* NeuroActionListInitCpuCalc[] = {
    &NeuroActionInitCpuCalc, NULL };
const NeuroAction* NeuroActionListSpawn[] = {
    &NeuroActionSpawn, NULL };
const NeuroAction* NeuroActionListSleep[] = {
    &NeuroActionSleep, NULL };
const NeuroAction* NeuroActionListStopCpuCalc[] = {
    &NeuroActionStopCpuCalc, NULL };
const NeuroAction* NeuroActionListChangeLayout[] = {
    &NeuroActionChangeLayout, NULL };
const NeuroAction* NeuroActionListResetLayout[] = {
    &NeuroActionResetLayout, NULL };
const NeuroAction* NeuroActionListToggleLayout[] = {
    &NeuroActionToggleLayout, NULL };
const NeuroAction* NeuroActionListToggleModLayout[] = {
    &NeuroActionToggleModLayout, NULL };
const NeuroAction* NeuroActionListIncreaseMasterLayout[] = {
    &NeuroActionIncreaseMasterLayout, NULL };
const NeuroAction* NeuroActionListResizeMasterLayout[] = {
    &NeuroActionResizeMasterLayout, NULL };
const NeuroAction* NeuroActionListChangeWorkspace[] = {
    &NeuroActionChangeWorkspace, NULL };
const NeuroAction* NeuroActionListSelectMonitor[] = {
    &NeuroActionSelectMonitor, NULL };
const NeuroAction* NeuroActionListRestoreLastMinimized[] = {
    &NeuroActionRestoreLastMinimized, NULL };
const NeuroAction* NeuroActionListToggleScratchpad[] = {
    &NeuroActionToggleScatchpad, NULL };
const NeuroAction* NeuroActionListFocusCurrClient[] = {
    &NeuroActionFocusCurrClient, NULL };
const NeuroAction* NeuroActionListSwapCurrClient[] = {
    &NeuroActionSwapCurrClient, NULL };
const NeuroAction* NeuroActionListSendCurrClient[] = {
    &NeuroActionSendCurrClient, NULL };
const NeuroAction* NeuroActionListSendFollowCurrClient[] = {
    &NeuroActionSendCurrClient, &NeuroActionChangeWorkspace, NULL };
const NeuroAction* NeuroActionListKillClient[] = {
    &NeuroActionKillCurrClient, NULL };
const NeuroAction* NeuroActionListTileCurrClient[] = {
    &NeuroActionTileCurrClient, NULL };
const NeuroAction* NeuroActionListNormalCurrClient[] = {
    &NeuroActionNormalCurrClient, NULL };
const NeuroAction* NeuroActionListFullscreenCurrClient[] = {
    &NeuroActionFullscreenCurrClient, NULL };
const NeuroAction* NeuroActionListToggleFullscreenCurrClient[] = {
    &NeuroActionToggleFullscreenCurrClient, NULL };
const NeuroAction* NeuroActionListMinimizeCurrClient[] = {
    &NeuroActionMinimizeCurrClient, NULL };
const NeuroAction* NeuroActionListFreeCurrClient[] = {
    &NeuroActionFreeCurrClient, NULL };
const NeuroAction* NeuroActionListToggleFreeCurrClient[] = {
    &NeuroActionToggleFreeCurrClient, NULL };
const NeuroAction* NeuroActionListFocusPtrClient[] = {
    &NeuroActionFocusPtrClient, NULL };
const NeuroAction* NeuroActionListFreeMovePtrClient[] = {
    &NeuroActionFreeMovePtrClient, NULL };
const NeuroAction* NeuroActionListFreeResizePtrClient[] = {
    &NeuroActionFreeResizePtrClient, NULL };
const NeuroAction* NeuroActionListFloatMovePtrClient[] = {
    &NeuroActionFloatMovePtrClient, NULL };
const NeuroAction* NeuroActionListFloatResizePtrClient[] = {
    &NeuroActionFloatResizePtrClient, NULL };
const NeuroAction* NeuroActionListToggleFullscreenPtrClient[] = {
    &NeuroActionToggleFullscreenPtrClient, NULL };
const NeuroAction* NeuroActionListFreePtrClient[] = {
    &NeuroActionFreePtrClient, NULL };
const NeuroAction* NeuroActionListToggleFreePtrClient[] = {
    &NeuroActionToggleFreePtrClient, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void NeuroActionHandlerNothing(NeuroArg null_arg) {
  (void)null_arg;
}

void NeuroActionHandlerQuit(NeuroArg null_arg) {
  (void)null_arg;
  NeuroWmQuit();
}

void NeuroActionHandlerReload(NeuroArg null_arg) {
  (void)null_arg;
  NeuroWmReload();
}

void NeuroActionHandlerChangeWmName(NeuroArg string_arg) {
  assert(NEURO_ARG_STR_GET(string_arg));
  NeuroSystemChangeWmName(NEURO_ARG_STR_GET(string_arg));
}

void NeuroActionHandlerSpawn(NeuroArg command_arg) {
  assert(NEURO_ARG_CMD_GET(command_arg));
  NeuroSystemSpawn(NEURO_ARG_CMD_GET(command_arg), NULL);
}

void NeuroActionHandlerSleep(NeuroArg uint_arg) {
  sleep(NEURO_ARG_UINT_GET(uint_arg));
}

void NeuroActionHandlerInitCpuCalc(NeuroArg null_arg) {
  (void)null_arg;
  NeuroDzenInitCpuCalc();
}

void NeuroActionHandlerStopCpuCalc(NeuroArg null_arg) {
  (void)null_arg;
  NeuroDzenStopCpuCalc();
}

// Layout
void NeuroActionHandlerChangeLayout(NeuroArg int_arg) {
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutChange(ws, NEURO_ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResetLayout(NeuroArg null_arg) {
  (void)null_arg;
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutReset(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleLayout(NeuroArg idx_arg) {
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggle(ws, NEURO_ARG_IDX_GET(idx_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleModLayout(NeuroArg lmod_arg) {
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggleModCurr(ws, NEURO_ARG_LMOD_GET(lmod_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerIncreaseMasterLayout(NeuroArg int_arg) {
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutIncreaseMaster(ws, NEURO_ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResizeMasterLayout(NeuroArg float_arg) {
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutResizeMaster(ws, NEURO_ARG_FLOAT_GET(float_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

// NeuroWorkspace
void NeuroActionHandlerChangeWorkspace(NeuroArg workspaceSelectorFn_arg) {
  assert(workspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_);
  const NeuroIndex dst = NEURO_ARG_WSF_GET(workspaceSelectorFn_arg)();
  process_workspace(NeuroWorkspaceChange, dst);
}

void NeuroActionHandlerSelectMonitor(NeuroArg MonitorSelectorFn_arg) {
  assert(MonitorSelectorFn_arg.GenericArgFn_.MonitorSelectorFn_);
  const NeuroMonitor *const dest_m =
      NEURO_ARG_MSF_GET(MonitorSelectorFn_arg)(NeuroCoreStackGetMonitor(NeuroCoreGetCurrStack()));
  if (!dest_m)
    return;
  const NeuroIndex ws = NeuroCoreGetMonitorStack(dest_m);
  NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
  NeuroCoreSetCurrStack(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroActionHandlerRestoreLastMinimized(NeuroArg null_arg) {
  (void)null_arg;
  process_workspace(NeuroWorkspaceRestoreLastMinimized, NeuroCoreGetCurrStack());
}

void NeuroActionHandlerToggleScratchpad(NeuroArg command_arg) {
  assert(NEURO_ARG_CMD_GET(command_arg));
  NeuroClientPtrPtr c = NeuroCoreGetCurrClientNspStack();
  NeuroClientPtrPtr nspc = NeuroCoreFindNspClient();
  const NeuroIndex ws = NeuroCoreGetCurrStack();
  const NeuroIndex nspws = NeuroCoreGetNspStack();
  if (nspc && NEURO_CLIENT_PTR(nspc)->ws == ws) {
    NeuroWorkspaceClientSend(nspc, NeuroClientSelectorSelf, (const void *)&nspws);
    // process_client(NeuroWorkspaceClientSend, nspc, NeuroClientSelectorSelf, (const void *)&nspws);
  } else if (nspc) {
    NeuroWorkspaceClientSend(nspc, NeuroClientSelectorSelf, (const void *)&ws);
    // process_client(NeuroWorkspaceClientSend, nspc, NeuroClientSelectorSelf, (const void *)&ws);
  } else {
    if (NeuroCoreGetNspStackSize()) {
      NeuroWorkspaceClientSend(c, NeuroClientSelectorSelf, (const void *)&ws);
      // process_client(NeuroWorkspaceClientSend, c, NeuroClientSelectorSelf, (const void *)&ws);
    } else {
      NeuroSystemSpawn(NEURO_ARG_CMD_GET(command_arg), NULL);
    }
  }
}

// Curr NeuroClient
void NeuroActionHandlerFocusCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientFocus, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSwapCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientSwap, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSendCurrClient(NeuroArg workspaceSelectorFn_arg) {
  assert(workspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_);
  const NeuroIndex ws = NEURO_ARG_WSF_GET(workspaceSelectorFn_arg)() % NeuroCoreGetSize();
  NeuroWorkspaceClientSend(NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
  // process_client(NeuroWorkspaceClientSend, NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
}

void NeuroActionHandlerKillCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroWorkspaceClientKill(NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientKill, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerTileCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientTile, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerNormalCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientNormal, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFullscreenCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientFullscreen, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg),
      NULL);
}

void NeuroActionHandlerToggleFullscreenCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg),
      NULL);
}

void NeuroActionHandlerMinimizeCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientMinimize, NeuroClientGetFocused(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeCurrClient(NeuroArg freeSetterFn_arg) {
  const void *const p = (const void *)&NEURO_ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientFree, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreeCurrClient(NeuroArg freeSetterFn_arg) {
  const void *const p = (const void *)&NEURO_ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientToggleFree, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

// Ptr Client
void NeuroActionHandlerFocusPtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);

  // Select the monitor where the pointer is
  NeuroPoint p = { 0 };
  const NeuroMonitor *const m = NeuroMonitorFindPointed(NeuroSystemGetPointerLocation(&p));
  for (NeuroIndex ws = NeuroCoreGetHeadStack(); ws < NeuroCoreGetSize(); ++ws) {
    if (NeuroCoreStackGetMonitor(ws) == m) {
      NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
      NeuroCoreSetCurrStack(ws);
      // NeuroWorkspaceUpdate(ws);
      break;
    }
  }

  // Focus the client under the pointer
  process_client(NeuroWorkspaceClientFocus, NeuroClientFindPointed(&p), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeMovePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroArg fsf = (NeuroArg)NEURO_ARG_FSF(NeuroRuleFreeSetterDefault);
  NeuroWorkspaceClientFreeMove(NeuroClientGetPointedByPointer(), NEURO_ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeMove, NeuroClientGetPointedByPointer(),
  //     NEURO_ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
}

void NeuroActionHandlerFreeResizePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroArg fsf = (NeuroArg)NEURO_ARG_FSF(NeuroRuleFreeSetterDefault);
  NeuroWorkspaceClientFreeResize(NeuroClientGetPointedByPointer(), NEURO_ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeResize, NeuroClientGetPointedByPointer(),
  //      NEURO_ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
}

void NeuroActionHandlerFloatMovePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroWorkspaceClientFloatMove(NeuroClientGetPointedByPointer(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatMove, NeuroClientGetPointedByPointer(),
  //     NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFloatResizePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroWorkspaceClientFloatResize(NeuroClientGetPointedByPointer(), NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatResize, NeuroClientGetPointedByPointer(),
  //     NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerToggleFullscreenPtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetPointedByPointer(),
      NEURO_ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreePtrClient(NeuroArg freeSetterFn_arg) {
  const void *const p = (const void *)&NEURO_ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientFree, NeuroClientGetPointedByPointer(), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreePtrClient(NeuroArg freeSetterFn_arg)  {
  const void *const p = (const void *)&NEURO_ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientToggleFree, NeuroClientGetPointedByPointer(), NeuroClientSelectorSelf, p);
}

// Util
void NeuroActionRunAction(const NeuroAction *a, const NeuroMaybeArg *arg) {
  if (!a)
    return;
  if (arg && !NEURO_MAYBE_ARG_IS_NOTHING(*arg))
    a->handler(NEURO_MAYBE_ARG_GET_JUST(*arg));
  else
    a->handler(a->arg);
}

void NeuroActionRunActionChain(const NeuroActionChain *ac) {
  if (!ac || !ac->action_list)
    return;
  for (NeuroIndex i = 0U; ac->action_list[ i ]; ++i)
    NeuroActionRunAction(ac->action_list[ i ], &ac->arg);
}
