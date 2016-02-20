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

static void process_client(const ActionClientFn acf, ClientPtrPtr c, const ClientSelectorFn csf, const void *data) {
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
    NeuroActionHandlerSleep, ARG_UINT(1) };
const Action NeuroActionStopCpuCalc = {
    NeuroActionHandlerStopCpuCalc, ARG_NULL };
const Action NeuroActionChangeLayout = {
    NeuroActionHandlerChangeLayout, ARG_INT(1) };
const Action NeuroActionResetLayout = {
    NeuroActionHandlerResetLayout, ARG_NULL };
const Action NeuroActionToggleLayout = {
    NeuroActionHandlerToggleLayout, ARG_IDX(0U) };
const Action NeuroActionToggleModLayout = {
    NeuroActionHandlerToggleModLayout, ARG_LMOD(NEURO_LAYOUT_MOD_NULL) };
const Action NeuroActionIncreaseMasterLayout = {
    NeuroActionHandlerIncreaseMasterLayout, ARG_INT(1) };
const Action NeuroActionResizeMasterLayout = {
    NeuroActionHandlerResizeMasterLayout, ARG_FLOAT(1.0f) };
const Action NeuroActionChangeWorkspace = {
    NeuroActionHandlerChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorNext) };
const Action NeuroActionSelectMonitor = {
    NeuroActionHandlerSelectMonitor, ARG_MSF(NeuroMonitorSelectorHead) };
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

// Action Lists
const Action* NeuroActionListNothing[] = { &NeuroActionNothing, NULL };
const Action* NeuroActionListQuit[] = { &NeuroActionQuit, NULL };
const Action* NeuroActionListReload[] = { &NeuroActionReload, NULL };
const Action* NeuroActionListInitCpuCalc[] = { &NeuroActionInitCpuCalc, NULL };
const Action* NeuroActionListSpawn[] = { &NeuroActionSpawn, NULL };
const Action* NeuroActionListSleep[] = { &NeuroActionSleep, NULL };
const Action* NeuroActionListStopCpuCalc[] = { &NeuroActionStopCpuCalc, NULL };
const Action* NeuroActionListChangeLayout[] = { &NeuroActionChangeLayout, NULL };
const Action* NeuroActionListResetLayout[] = { &NeuroActionResetLayout, NULL };
const Action* NeuroActionListToggleLayout[] = { &NeuroActionToggleLayout, NULL };
const Action* NeuroActionListToggleModLayout[] = { &NeuroActionToggleModLayout, NULL };
const Action* NeuroActionListIncreaseMasterLayout[] = { &NeuroActionIncreaseMasterLayout, NULL };
const Action* NeuroActionListResizeMasterLayout[] = { &NeuroActionResizeMasterLayout, NULL };
const Action* NeuroActionListChangeWorkspace[] = { &NeuroActionChangeWorkspace, NULL };
const Action* NeuroActionListSelectMonitor[] = { &NeuroActionSelectMonitor, NULL };
const Action* NeuroActionListRestoreLastMinimized[] = { &NeuroActionRestoreLastMinimized, NULL };
const Action* NeuroActionListToggleScratchpad[] = { &NeuroActionToggleScatchpad, NULL };
const Action* NeuroActionListFocusCurrClient[] = { &NeuroActionFocusCurrClient, NULL };
const Action* NeuroActionListSwapCurrClient[] = { &NeuroActionSwapCurrClient, NULL };
const Action* NeuroActionListSendCurrClient[] = { &NeuroActionSendCurrClient, NULL };
const Action* NeuroActionListSendFollowCurrClient[] = { &NeuroActionSendCurrClient, &NeuroActionChangeWorkspace, NULL };
const Action* NeuroActionListKillClient[] = { &NeuroActionKillCurrClient, NULL };
const Action* NeuroActionListTileCurrClient[] = { &NeuroActionTileCurrClient, NULL };
const Action* NeuroActionListNormalCurrClient[] = { &NeuroActionNormalCurrClient, NULL };
const Action* NeuroActionListFullscreenCurrClient[] = { &NeuroActionFullscreenCurrClient, NULL };
const Action* NeuroActionListToggleFullscreenCurrClient[] = { &NeuroActionToggleFullscreenCurrClient, NULL };
const Action* NeuroActionListMinimizeCurrClient[] = { &NeuroActionMinimizeCurrClient, NULL };
const Action* NeuroActionListFreeCurrClient[] = { &NeuroActionFreeCurrClient, NULL };
const Action* NeuroActionListToggleFreeCurrClient[] = { &NeuroActionToggleFreeCurrClient, NULL };
const Action* NeuroActionListFocusPtrClient[] = { &NeuroActionFocusPtrClient, NULL };
const Action* NeuroActionListFreeMovePtrClient[] = { &NeuroActionFreeMovePtrClient, NULL };
const Action* NeuroActionListFreeResizePtrClient[] = { &NeuroActionFreeResizePtrClient, NULL };
const Action* NeuroActionListFloatMovePtrClient[] = { &NeuroActionFloatMovePtrClient, NULL };
const Action* NeuroActionListFloatResizePtrClient[] = { &NeuroActionFloatResizePtrClient, NULL };
const Action* NeuroActionListToggleFullscreenPtrClient[] = { &NeuroActionToggleFullscreenPtrClient, NULL };
const Action* NeuroActionListFreePtrClient[] = { &NeuroActionFreePtrClient, NULL };
const Action* NeuroActionListToggleFreePtrClient[] = { &NeuroActionToggleFreePtrClient, NULL };


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

void NeuroActionHandlerSleep(GenericArg uint_arg) {
  sleep(ARG_UINT_GET(uint_arg));
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
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutChange(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResetLayout(GenericArg null_arg) {
  (void)null_arg;
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutReset(ws);
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleLayout(GenericArg idx_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggle(ws, ARG_IDX_GET(idx_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerToggleModLayout(GenericArg lmod_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutToggleModCurr(ws, ARG_LMOD_GET(lmod_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerIncreaseMasterLayout(GenericArg int_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutIncreaseMaster(ws, ARG_INT_GET(int_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

void NeuroActionHandlerResizeMasterLayout(GenericArg float_arg) {
  const size_t ws = NeuroCoreGetCurrStack();
  NeuroWorkspaceRemoveEnterNotifyMask(ws);
  NeuroLayoutResizeMaster(ws, ARG_FLOAT_GET(float_arg));
  NeuroWorkspaceAddEnterNotifyMask(ws);
}

// Workspace
void NeuroActionHandlerChangeWorkspace(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_);
  const size_t dst = ARG_WSF_GET(WorkspaceSelectorFn_arg)();
  process_workspace(NeuroWorkspaceChange, dst);
}

void NeuroActionHandlerSelectMonitor(GenericArg MonitorSelectorFn_arg) {
  assert(MonitorSelectorFn_arg.GenericArgFn_.MonitorSelectorFn_);
  const Monitor *const dest_m = ARG_MSF_GET(MonitorSelectorFn_arg)(NeuroCoreStackGetMonitor(NeuroCoreGetCurrStack()));
  if (!dest_m)
    return;
  const size_t ws = NeuroCoreGetMonitorStack(dest_m);
  NeuroWorkspaceUnfocus(NeuroCoreGetCurrStack());
  NeuroCoreSetCurrStack(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroActionHandlerRestoreLastMinimized(GenericArg null_arg) {
  (void)null_arg;
  process_workspace(NeuroWorkspaceRestoreLastMinimized, NeuroCoreGetCurrStack());
}

void NeuroActionHandlerToggleScratchpad(GenericArg command_arg) {
  assert(ARG_CMD_GET(command_arg));
  ClientPtrPtr c = NeuroCoreGetCurrClientNspStack();
  ClientPtrPtr nspc = NeuroCoreFindNspClient();
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

// Curr Client
void NeuroActionHandlerFocusCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientFocus, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSwapCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientSwap, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerSendCurrClient(GenericArg WorkspaceSelectorFn_arg) {
  assert(WorkspaceSelectorFn_arg.GenericArgFn_.WorkspaceSelectorFn_);
  const size_t ws = ARG_WSF_GET(WorkspaceSelectorFn_arg)() % NeuroCoreGetSize();
  NeuroWorkspaceClientSend(NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
  // process_client(NeuroWorkspaceClientSend, NeuroClientGetFocused(), NeuroClientSelectorSelf, (const void *)&ws);
}

void NeuroActionHandlerKillCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  NeuroWorkspaceClientKill(NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientKill, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerTileCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientTile, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerNormalCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientNormal, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerFullscreenCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientFullscreen, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg), NULL);
}

void NeuroActionHandlerToggleFullscreenCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  process_client(NeuroWorkspaceClientToggleFullscreen, NeuroClientGetFocused(), ARG_CSF_GET(clientSelectorFn_arg),
      NULL);
}

void NeuroActionHandlerMinimizeCurrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
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
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  ClientPtrPtr c = NeuroClientGetPointed(&rx, &ry);
  const Monitor *const m = NeuroMonitorFindPtr(rx, ry);
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

void NeuroActionHandlerFreeMovePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  GenericArg fsf = (GenericArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  NeuroWorkspaceClientFreeMove(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeMove, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    (const void *)&fsf);
}

void NeuroActionHandlerFreeResizePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  GenericArg fsf = (GenericArg)ARG_FSF(NeuroRuleFreeSetterDefault);
  int rx, ry;
  NeuroWorkspaceClientFreeResize(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
      (const void *)&fsf);
  // process_client(NeuroWorkspaceClientFreeResize, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    (const void *)&fsf);
}

void NeuroActionHandlerFloatMovePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  NeuroWorkspaceClientFloatMove(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatMove, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    NULL);
}

void NeuroActionHandlerFloatResizePtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
  int rx, ry;
  NeuroWorkspaceClientFloatResize(NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg), NULL);
  // process_client(NeuroWorkspaceClientFloatResize, NeuroClientGetPointed(&rx, &ry), ARG_CSF_GET(clientSelectorFn_arg),
  //    NULL);
}

void NeuroActionHandlerToggleFullscreenPtrClient(GenericArg clientSelectorFn_arg) {
  assert(clientSelectorFn_arg.GenericArgFn_.ClientSelectorFn_);
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
  if (!ac || !ac->action_list)
    return;
  for (size_t i = 0U; ac->action_list[ i ]; ++i)
    NeuroActionRunAction(ac->action_list[ i ], &ac->arg);
}
