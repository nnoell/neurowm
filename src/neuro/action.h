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

#ifndef NEURO_ACTION_H_
#define NEURO_ACTION_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager (Actions)
extern const Action NeuroActionNothing;
extern const Action NeuroActionQuit;
extern const Action NeuroActionReload;
extern const Action NeuroActionSpawn;
extern const Action NeuroActionSleep;
extern const Action NeuroActionInitCpuCalc;
extern const Action NeuroActionStopCpuCalc;

// Layout (Actions)
extern const Action NeuroActionChangeLayout;
extern const Action NeuroActionResetLayout;
extern const Action NeuroActionToggleLayout;
extern const Action NeuroActionToggleModLayout;
extern const Action NeuroActionIncreaseMasterLayout;
extern const Action NeuroActionResizeMasterLayout;

// Workspace (Actions)
extern const Action NeuroActionChangeWorkspace;
extern const Action NeuroActionRestoreLastMinimized;
extern const Action NeuroActionToggleScatchpad;

// CurrClient (Actions)
extern const Action NeuroActionFocusCurrClient;
extern const Action NeuroActionSwapCurrClient;
extern const Action NeuroActionSendCurrClient;
extern const Action NeuroActionKillCurrClient;
extern const Action NeuroActionTileCurrClient;
extern const Action NeuroActionNormalCurrClient;
extern const Action NeuroActionFullscreenCurrClient;
extern const Action NeuroActionToggleFullscreenCurrClient;
extern const Action NeuroActionMinimizeCurrClient;
extern const Action NeuroActionFreeCurrClient;
extern const Action NeuroActionToggleFreeCurrClient;

// PtrClient (Actions)
extern const Action NeuroActionFocusPtrClient;
extern const Action NeuroActionFreeMovePtrClient;
extern const Action NeuroActionFreeResizePtrClient;
extern const Action NeuroActionFloatMovePtrClient;
extern const Action NeuroActionFloatResizePtrClient;
extern const Action NeuroActionToggleFullscreenPtrClient;
extern const Action NeuroActionFreePtrClient;
extern const Action NeuroActionToggleFreePtrClient;

// Window Manager (Action Lists)
extern const Action* NeuroActionListNothing[];
extern const Action* NeuroActionListQuit[];
extern const Action* NeuroActionListReload[];
extern const Action* NeuroActionListSpawn[];
extern const Action* NeuroActionListSleep[];
extern const Action* NeuroActionListInitCpuCalc[];
extern const Action* NeuroActionListStopCpuCalc[];

// Layout (Action Lists)
extern const Action* NeuroActionListChangeLayout[];
extern const Action* NeuroActionListResetLayout[];
extern const Action* NeuroActionListToggleLayout[];
extern const Action* NeuroActionListToggleModLayout[];
extern const Action* NeuroActionListIncreaseMasterLayout[];
extern const Action* NeuroActionListResizeMasterLayout[];

// Workspace (Action Lists)
extern const Action* NeuroActionListChangeWorkspace[];
extern const Action* NeuroActionListRestoreLastMinimized[];
extern const Action* NeuroActionListToggleScratchpad[];

// CurrClient (Action Lists)
extern const Action* NeuroActionListFocusCurrClient[];
extern const Action* NeuroActionListSwapCurrClient[];
extern const Action* NeuroActionListSendCurrClient[];
extern const Action* NeuroActionListSendFollowCurrClient[];
extern const Action* NeuroActionListKillClient[];
extern const Action* NeuroActionListTileCurrClient[];
extern const Action* NeuroActionListNormalCurrClient[];
extern const Action* NeuroActionListFullscreenCurrClient[];
extern const Action* NeuroActionListToggleFullscreenCurrClient[];
extern const Action* NeuroActionListMinimizeCurrClient[];
extern const Action* NeuroActionListFreeCurrClient[];
extern const Action* NeuroActionListToggleFreeCurrClient[];

// PtrClient (Action Lists)
extern const Action* NeuroActionListFocusPtrClient[];
extern const Action* NeuroActionListFreeMovePtrClient[];
extern const Action* NeuroActionListFreeResizePtrClient[];
extern const Action* NeuroActionListFloatMovePtrClient[];
extern const Action* NeuroActionListFloatResizePtrClient[];
extern const Action* NeuroActionListToggleFullscreenPtrClient[];
extern const Action* NeuroActionListFreePtrClient[];
extern const Action* NeuroActionListToggleFreePtrClient[];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager (Handlers)
void NeuroActionHandlerNothing(GenericArg null_arg);
void NeuroActionHandlerQuit(GenericArg null_arg);
void NeuroActionHandlerReload(GenericArg null_arg);
void NeuroActionHandlerChangeWmName(GenericArg string_arg);
void NeuroActionHandlerSpawn(GenericArg command_arg);
void NeuroActionHandlerSleep(GenericArg int_arg);
void NeuroActionHandlerInitCpuCalc(GenericArg null_arg);
void NeuroActionHandlerStopCpuCalc(GenericArg null_arg);

// Layout (Handlers)
void NeuroActionHandlerChangeLayout(GenericArg int_arg);
void NeuroActionHandlerResetLayout(GenericArg null_arg);
void NeuroActionHandlerToggleLayout(GenericArg int_arg);
void NeuroActionHandlerToggleModLayout(GenericArg uint_arg);
void NeuroActionHandlerIncreaseMasterLayout(GenericArg int_arg);
void NeuroActionHandlerResizeMasterLayout(GenericArg float_arg);

// Workspace (Handlers)
void NeuroActionHandlerChangeWorkspace(GenericArg WorkspaceSelectorFn_arg);
void NeuroActionHandlerRestoreLastMinimized(GenericArg null_arg);
void NeuroActionHandlerToggleScratchpad(GenericArg command_arg);

// CurrClient (Handlers)
void NeuroActionHandlerFocusCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerSwapCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerSendCurrClient(GenericArg WorkspaceSelectorFn_arg);
void NeuroActionHandlerKillCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerTileCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerNormalCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFullscreenCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerToggleFullscreenCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerMinimizeCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFreeCurrClient(GenericArg freeSetterFn_arg);
void NeuroActionHandlerToggleFreeCurrClient(GenericArg freeSetterFn_arg);

// PtrClient (Handlers)
void NeuroActionHandlerFocusPtrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFreeMovePtrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFreeResizePtrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFloatMovePtrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFloatResizePtrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerToggleFullscreenPtrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerFreePtrClient(GenericArg freeSetterFn_arg);
void NeuroActionHandlerToggleFreePtrClient(GenericArg freeSetterFn_arg);

// Run
void NeuroActionRunAction(const Action *a, const GenericMaybeArg *arg);
void NeuroActionRunActionChain(const ActionChain *ac);

#endif  // NEURO_ACTION_H_

