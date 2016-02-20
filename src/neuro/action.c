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
typedef void (*ActionClientFn)(NeuroClientPtrPtr c, NeuroClientSelectorFn csf, const void *data);

// ActionWorkspaceFn
typedef void (*ActionWorkspaceFn)(size_t ws);


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void process_workspace(const ActionWorkspaceFn awsf, size_t ws) {
  assert(awsf);
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  awsf(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

static void process_client(const ActionClientFn acf, NeuroClientPtrPtr c, const NeuroClientSelectorFn csf, const void *data) {
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
const NeuroAction NeuroActionNothing = {
    NeuroActionHandlerNothing, ARG_NULL };
const NeuroAction NeuroActionQuit = {
    NeuroActionHandlerQuit, ARG_NULL };
const NeuroAction NeuroActionReload = {
    NeuroActionHandlerReload, ARG_NULL };
const NeuroAction NeuroActionInitCpuCalc = {
    NeuroActionHandlerInitCpuCalc, ARG_NULL };
const NeuroAction NeuroActionSpawn = {
    NeuroActionHandlerSpawn, ARG_CMD(NeuroConfigDefaultTerminalCommand) };
const NeuroAction NeuroActionSleep = {
    NeuroActionHandlerSleep, ARG_UINT(1) };
const NeuroAction NeuroActionStopCpuCalc = {
    NeuroActionHandlerStopCpuCalc, ARG_NULL };
const NeuroAction NeuroActionChangeLayout = {
    NeuroActionHandlerChangeLayout, ARG_INT(1) };
const NeuroAction NeuroActionResetLayout = {
    NeuroActionHandlerResetLayout, ARG_NULL };
const NeuroAction NeuroActionToggleLayout = {
    NeuroActionHandlerToggleLayout, ARG_IDX(0U) };
const NeuroAction NeuroActionToggleModLayout = {
    NeuroActionHandlerToggleModLayout, ARG_LMOD(NEURO_LAYOUT_MOD_NULL) };
const NeuroAction NeuroActionIncreaseMasterLayout = {
    NeuroActionHandlerIncreaseMasterLayout, ARG_INT(1) };
const NeuroAction NeuroActionResizeMasterLayout = {
    NeuroActionHandlerResizeMasterLayout, ARG_FLOAT(1.0f) };
const NeuroAction NeuroActionChangeWorkspace = {
    NeuroActionHandlerChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorNext) };
const NeuroAction NeuroActionSelectMonitor = {
    NeuroActionHandlerSelectMonitor, ARG_MSF(NeuroMonitorSelectorHead) };
const NeuroAction NeuroActionRestoreLastMinimized = {
    NeuroActionHandlerRestoreLastMinimized, ARG_NULL };
const NeuroAction NeuroActionToggleScatchpad = {
    NeuroActionHandlerToggleScratchpad, ARG_CMD(NeuroConfigDefaultLauncherCommand) };
const NeuroAction NeuroActionFocusCurrClient = {
    NeuroActionHandlerFocusCurrClient, ARG_CSF(NeuroClientSelectorNext) };
const NeuroAction NeuroActionSwapCurrClient = {
    NeuroActionHandlerSwapCurrClient, ARG_CSF(NeuroClientSelectorNext) };
const NeuroAction NeuroActionSendCurrClient = {
    NeuroActionHandlerSendCurrClient, ARG_WSF(NeuroWorkspaceSelectorNext) };
const NeuroAction NeuroActionKillCurrClient = {
    NeuroActionHandlerKillCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionTileCurrClient = {
    NeuroActionHandlerTileCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionNormalCurrClient = {
    NeuroActionHandlerNormalCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFullscreenCurrClient = {
    NeuroActionHandlerFullscreenCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionToggleFullscreenCurrClient = {
    NeuroActionHandlerToggleFullscreenCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionMinimizeCurrClient = {
    NeuroActionHandlerMinimizeCurrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreeCurrClient = {
    NeuroActionHandlerFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };
const NeuroAction NeuroActionToggleFreeCurrClient = {
    NeuroActionHandlerToggleFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };
const NeuroAction NeuroActionFocusPtrClient = {
    NeuroActionHandlerFocusPtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreeMovePtrClient = {
    NeuroActionHandlerFreeMovePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreeResizePtrClient = {
    NeuroActionHandlerFreeResizePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFloatMovePtrClient = {
    NeuroActionHandlerFloatMovePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFloatResizePtrClient = {
    NeuroActionHandlerFloatResizePtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionToggleFullscreenPtrClient = {
    NeuroActionHandlerToggleFullscreenPtrClient, ARG_CSF(NeuroClientSelectorSelf) };
const NeuroAction NeuroActionFreePtrClient = {
    NeuroActionHandlerFreePtrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };
const NeuroAction NeuroActionToggleFreePtrClient = {
    NeuroActionHandlerToggleFreePtrClient, ARG_FSF(NeuroRuleFreeSetterDefault) };

// NeuroAction Lists
const NeuroAction* NeuroActionListNothing[] = { &NeuroActionNothing, NULL };
const NeuroAction* NeuroActionListQuit[] = { &NeuroActionQuit, NULL };
const NeuroAction* NeuroActionListReload[] = { &NeuroActionReload, NULL };
const NeuroAction* NeuroActionListInitCpuCalc[] = { &NeuroActionInitCpuCalc, NULL };
const NeuroAction* NeuroActionListSpawn[] = { &NeuroActionSpawn, NULL };
const NeuroAction* NeuroActionListSleep[] = { &NeuroActionSleep, NULL };
const NeuroAction* NeuroActionListStopCpuCalc[] = { &NeuroActionStopCpuCalc, NULL };
const NeuroAction* NeuroActionListChangeLayout[] = { &NeuroActionChangeLayout, NULL };
const NeuroAction* NeuroActionListResetLayout[] = { &NeuroActionResetLayout, NULL };
const NeuroAction* NeuroActionListToggleLayout[] = { &NeuroActionToggleLayout, NULL };
const NeuroAction* NeuroActionListToggleModLayout[] = { &NeuroActionToggleModLayout, NULL };
const NeuroAction* NeuroActionListIncreaseMasterLayout[] = { &NeuroActionIncreaseMasterLayout, NULL };
const NeuroAction* NeuroActionListResizeMasterLayout[] = { &NeuroActionResizeMasterLayout, NULL };
const NeuroAction* NeuroActionListChangeWorkspace[] = { &NeuroActionChangeWorkspace, NULL };
const NeuroAction* NeuroActionListSelectMonitor[] = { &NeuroActionSelectMonitor, NULL };
const NeuroAction* NeuroActionListRestoreLastMinimized[] = { &NeuroActionRestoreLastMinimized, NULL };
const NeuroAction* NeuroActionListToggleScratchpad[] = { &NeuroActionToggleScatchpad, NULL };
const NeuroAction* NeuroActionListFocusCurrClient[] = { &NeuroActionFocusCurrClient, NULL };
const NeuroAction* NeuroActionListSwapCurrClient[] = { &NeuroActionSwapCurrClient, NULL };
const NeuroAction* NeuroActionListSendCurrClient[] = { &NeuroActionSendCurrClient, NULL };
const NeuroAction* NeuroActionListSendFollowCurrClient[] = { &NeuroActionSendCurrClient, &NeuroActionChangeWorkspace, NULL };
const NeuroAction* NeuroActionListKillClient[] = { &NeuroActionKillCurrClient, NULL };
const NeuroAction* NeuroActionListTileCurrClient[] = { &NeuroActionTileCurrClient, NULL };
const NeuroAction* NeuroActionListNormalCurrClient[] = { &NeuroActionNormalCurrClient, NULL };
const NeuroAction* NeuroActionListFullscreenCurrClient[] = { &NeuroActionFullscreenCurrClient, NULL };
const NeuroAction* NeuroActionListToggleFullscreenCurrClient[] = { &NeuroActionToggleFullscreenCurrClient, NULL };
const NeuroAction* NeuroActionListMinimizeCurrClient[] = { &NeuroActionMinimizeCurrClient, NULL };
const NeuroAction* NeuroActionListFreeCurrClient[] = { &NeuroActionFreeCurrClient, NULL };
const NeuroAction* NeuroActionListToggleFreeCurrClient[] = { &NeuroActionToggleFreeCurrClient, NULL };
const NeuroAction* NeuroActionListFocusPtrClient[] = { &NeuroActionFocusPtrClient, NULL };
const NeuroAction* NeuroActionListFreeMovePtrClient[] = { &NeuroActionFreeMovePtrClient, NULL };
const NeuroAction* NeuroActionListFreeResizePtrClient[] = { &NeuroActionFreeResizePtrClient, NULL };
const NeuroAction* NeuroActionListFloatMovePtrClient[] = { &NeuroActionFloatMovePtrClient, NULL };
const NeuroAction* NeuroActionListFloatResizePtrClient[] = { &NeuroActionFloatResizePtrClient, NULL };
const NeuroAction* NeuroActionListToggleFullscreenPtrClient[] = { &NeuroActionToggleFullscreenPtrClient, NULL };
const NeuroAction* NeuroActionListFreePtrClient[] = { &NeuroActionFreePtrClient, NULL };
const NeuroAction* NeuroActionListToggleFreePtrClient[] = { &NeuroActionToggleFreePtrClient, NULL };


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
  assert(ARG_STR_GET(string_arg));
  NeuroSystemChangeWmName(ARG_STR_GET(string_arg));
}

void NeuroActionHandlerSpawn(NeuroArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  NeuroSystemSpawn(ARG_CMD_GET(command_arg), NULL);
}

void NeuroActionHandlerSleep(NeuroArg uint_arg) {
  sleep(ARG_UINT_GET(uint_arg));
}

void NeuroActionHandlerInitCpuCalc(NeuroArg null_arg) {
  (void)null_arg;
  NeuroDzenInitCpuCalc();
}

void NeuroActionHandlerStopCpuCalc(NeuroArg null_arg) {
  (void)null_arg;
  NeuroDzenStopCpuCalc();
}

// NeuroLayout
void NeuroActionHandlerChangeLayout(NeuroArg int_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutChange(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResetLayout(NeuroArg null_arg) {
  (void)null_arg;
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutReset(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleLayout(NeuroArg idx_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggle(ws, ARG_IDX_GET(idx_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleModLayout(NeuroArg lmod_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggleModCurr(ws, ARG_LMOD_GET(lmod_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerIncreaseMasterLayout(NeuroArg int_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutIncreaseMaster(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResizeMasterLayout(NeuroArg float_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutResizeMaster(ws, ARG_FLOAT_GET(float_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

// NeuroWorkspace
void NeuroActionHandlerChangeWorkspace(NeuroArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_);
  const size_t dst = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  process_workspace(NeuroWorkspaceChange, dst);
}

void NeuroActionHandlerSelectMonitor(NeuroArg MonitorSelectorFn_arg) {
  assert(MonitorSelectorFn_arg.GenericArgFn_.MonitorSelectorFn_);
  const NeuroMonitor *const dest_m = ARG_MSF_GET(MonitorSelectorFn_arg)(NeuroCoreStackGetMonitor(NeuroCoreGetCurrStack()));
  if (!dest_m)
    return;
  const size_t ws = NeuroCoreGetMonitorStack(dest_m);
  NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
  NeuroCoreSetCurrStack(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroActionHandlerRestoreLastMinimized(NeuroArg null_arg) {
  (void)null_arg;
  process_workspace(NeuroWorkspaceRestoreLastMinimized, NeuroCoreGetCurrStack());
}

void NeuroActionHandlerToggleScratchpad(NeuroArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  NeuroClientPtrPtr c = NeuroCoreGetCurrClientNspStack();
  NeuroClientPtrPtr nspc = NeuroCoreFindNspClient();
  const size_t ws = NeuroCoreGetCurrStack();
  const size_t nspws = NeuroCoreGetNspStack();
  if (nspc && CLI_GET(nspc).ws == ws) {
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
      NeuroSystemSpawn(ARG_CMD_GET(command_arg), NULL);
    }
  }
}

// Curr NeuroClient
void NeuroActionHandlerFocusCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientFocus, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSwapCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientSwap, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSendCurrClient(NeuroArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_);
  const size_t ws = ARG_WSF_GET(WorkspaceSelectorFn_arg)() % NeuroCoreGetSize();
  NeuroWorkspaceClientSend(NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
  // process_client(NeuroWorkspaceClientSend, NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
}

void NeuroActionHandlerKillCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroWorkspaceClientKill(NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientKill, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerTileCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientTile, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerNormalCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientNormal, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFullscreenCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientFullscreen, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerToggleFullscreenCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg),
      NULL);
}

void NeuroActionHandlerMinimizeCurrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientMinimize, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeCurrClient(NeuroArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientFree, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreeCurrClient(NeuroArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  process_client(NeuroWorkspaceClientToggleFree, NeuroClientGetFocused(), NeuroClientSelectorSelf, p);
}

// Ptr NeuroClient
void NeuroActionHandlerFocusPtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  NeuroClientPtrPtr c = NeuroClientGetPointed(&rx, &ry);
  const NeuroMonitor *const m = NeuroMonitorFindPtr(rx, ry);
  if (m) {
    for (size_t ws = NeuroCoreGetHeadStack(); ws < NeuroCoreGetSize(); ++ws) {
      if (NeuroCoreStackGetMonitor(ws) == m) {
        NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
        NeuroCoreSetCurrStack(ws);
        // NeuroWorkspaceUpdate(ws);
        break;
      }
    }
  }
  process_client(NeuroWorkspaceClientFocus, c, ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreeMovePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroArg fsf = (NeuroArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  NeuroWorkspaceClientFreeMove(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeMove, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    (const void *)&fsf);
}

void NeuroActionHandlerFreeResizePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroArg fsf = (NeuroArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  NeuroWorkspaceClientFreeResize(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeResize, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    (const void *)&fsf);
}

void NeuroActionHandlerFloatMovePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  NeuroWorkspaceClientFloatMove(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatMove, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    NULL);
}

void NeuroActionHandlerFloatResizePtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  NeuroWorkspaceClientFloatResize(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatResize, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    NULL);
}

void NeuroActionHandlerToggleFullscreenPtrClient(NeuroArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetPointed(&rx, &ry),
      ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFreePtrClient(NeuroArg freeSetterFn_arg) {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  process_client(NeuroWorkspaceClientFree, NeuroClientGetPointed(&rx, &ry), NeuroClientSelectorSelf, p);
}

void NeuroActionHandlerToggleFreePtrClient(NeuroArg freeSetterFn_arg)  {
  const void *p = (const void *)&ARG_GAF_GET(freeSetterFn_arg);
  int rx, ry;
  process_client(NeuroWorkspaceClientToggleFree, NeuroClientGetPointed(&rx, &ry), NeuroClientSelectorSelf, p);
}

// Util
void NeuroActionRunAction(const NeuroAction *a, const NeuroMaybeArg *arg) {
  if (!a)
    return;
  if (arg && !MAYBE_ARG_IS_NOTHING(*arg))
    a->handler(MAYBE_ARG_GET_JUST(*arg));
  else
    a->handler(a->arg);
}

void NeuroActionRunActionChain(const NeuroActionChain *ac) {
  if (!ac || !ac->action_list)
    return;
  for (size_t i = 0U; ac->action_list[ i ]; ++i)
    NeuroActionRunAction(ac->action_list[ i ], &ac->arg);
}
