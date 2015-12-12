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

// Window Manager (Action Sets)
extern const Action* NeuroActionSetNothing[];
extern const Action* NeuroActionSetQuit[];
extern const Action* NeuroActionSetReload[];
extern const Action* NeuroActionSetSpawn[];
extern const Action* NeuroActionSetSleep[];
extern const Action* NeuroActionSetInitCpuCalc[];
extern const Action* NeuroActionSetStopCpuCalc[];

// Layout (Action Sets)
extern const Action* NeuroActionSetChangeLayout[];
extern const Action* NeuroActionSetResetLayout[];
extern const Action* NeuroActionSetToggleLayout[];
extern const Action* NeuroActionSetToggleModLayout[];
extern const Action* NeuroActionSetIncreaseMasterLayout[];
extern const Action* NeuroActionSetResizeMasterLayout[];

// Workspace (Action Sets)
extern const Action* NeuroActionSetChangeWorkspace[];
extern const Action* NeuroActionSetRestoreLastMinimized[];
extern const Action* NeuroActionSetToggleScratchpad[];

// CurrClient (Action Sets)
extern const Action* NeuroActionSetFocusCurrClient[];
extern const Action* NeuroActionSetSwapCurrClient[];
extern const Action* NeuroActionSetSendCurrClient[];
extern const Action* NeuroActionSetSendFollowCurrClient[];
extern const Action* NeuroActionSetKillClient[];
extern const Action* NeuroActionSetTileCurrClient[];
extern const Action* NeuroActionSetNormalCurrClient[];
extern const Action* NeuroActionSetFullscreenCurrClient[];
extern const Action* NeuroActionSetToggleFullscreenCurrClient[];
extern const Action* NeuroActionSetMinimizeCurrClient[];
extern const Action* NeuroActionSetFreeCurrClient[];
extern const Action* NeuroActionSetToggleFreeCurrClient[];

// PtrClient (Action Sets)
extern const Action* NeuroActionSetFocusPtrClient[];
extern const Action* NeuroActionSetFreeMovePtrClient[];
extern const Action* NeuroActionSetFreeResizePtrClient[];
extern const Action* NeuroActionSetFloatMovePtrClient[];
extern const Action* NeuroActionSetFloatResizePtrClient[];
extern const Action* NeuroActionSetToggleFullscreenPtrClient[];
extern const Action* NeuroActionSetFreePtrClient[];
extern const Action* NeuroActionSetToggleFreePtrClient[];


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

