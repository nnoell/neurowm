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
extern const Action const NeuroActionNothing[];
extern const Action const NeuroActionQuit[];
extern const Action const NeuroActionReload[];
extern const Action const NeuroActionSpawn[];
extern const Action const NeuroActionSleep[];
extern const Action const NeuroActionStartCpuCalc[];
extern const Action const NeuroActionStopCpuCalc[];

// Layout (Actions)
extern const Action const NeuroActionChangeLayout[];
extern const Action const NeuroActionResetLayout[];
extern const Action const NeuroActionToggleLayout[];
extern const Action const NeuroActionToggleModLayout[];
extern const Action const NeuroActionIncreaseMasterLayout[];
extern const Action const NeuroActionResizeMasterLayout[];

// Workspace (Actions)
extern const Action const NeuroActionChangeWorkspace[];
extern const Action const NeuroActionRelativeChangeWorkspace[];
extern const Action const NeuroActionRestoreLastMinimized[];
extern const Action const NeuroActionToggleScatchpad[];

// CurrClient (Actions)
extern const Action const NeuroActionFocusCurrClient[];
extern const Action const NeuroActionSwapCurrClient[];
extern const Action const NeuroActionSendCurrClient[];
extern const Action const NeuroActionRelativeSendCurrClient[];
extern const Action const NeuroActionKillCurrClient[];
extern const Action const NeuroActionTileCurrClient[];
extern const Action const NeuroActionNormalCurrClient[];
extern const Action const NeuroActionFullscreenCurrClient[];
extern const Action const NeuroActionToggleFullscreenCurrClient[];
extern const Action const NeuroActionMinimizeCurrClient[];
extern const Action const NeuroActionFreeCurrClient[];
extern const Action const NeuroActionToggleFreeCurrClient[];

// PtrClient (Actions)
extern const Action const NeuroActionFocusPtrClient[];
extern const Action const NeuroActionFreeMovePtrClient[];
extern const Action const NeuroActionFreeResizePtrClient[];
extern const Action const NeuroActionFloatMovePtrClient[];
extern const Action const NeuroActionFloatResizePtrClient[];
extern const Action const NeuroActionToggleFullscreenPtrClient[];
extern const Action const NeuroActionFreePtrClient[];
extern const Action const NeuroActionToggleFreePtrClient[];

// Window Manager (Action Chains)
extern const Action *const NeuroActionChainNothing[];
extern const Action *const NeuroActionChainQuit[];
extern const Action *const NeuroActionChainReload[];
extern const Action *const NeuroActionChainSpawn[];
extern const Action *const NeuroActionChainSleep[];
extern const Action *const NeuroActionChainStartCpuCalc[];
extern const Action *const NeuroActionChainStopCpuCalc[];

// Layout (Action Chains)
extern const Action *const NeuroActionChainChangeLayout[];
extern const Action *const NeuroActionChainResetLayout[];
extern const Action *const NeuroActionChainToggleLayout[];
extern const Action *const NeuroActionChainToggleModLayout[];
extern const Action *const NeuroActionChainIncreaseMasterLayout[];
extern const Action *const NeuroActionChainResizeMasterLayout[];

// Workspace (Action Chains)
extern const Action *const NeuroActionChainChangeWorkspace[];
extern const Action *const NeuroActionChainRelativeChangeWorkspace[];
extern const Action *const NeuroActionChainRestoreLastMinimized[];
extern const Action *const NeuroActionChainToggleScratchpad[];

// CurrClient (Action Chains)
extern const Action *const NeuroActionChainFocusCurrClient[];
extern const Action *const NeuroActionChainSwapCurrClient[];
extern const Action *const NeuroActionChainSendCurrClient[];
extern const Action *const NeuroActionChainRelativeSendCurrClient[];
extern const Action *const NeuroActionChainSendFollowCurrClient[];
extern const Action *const NeuroActionChainRelativeSendFollowCurrClient[];
extern const Action *const NeuroActionChainKillClient[];
extern const Action *const NeuroActionChainTileCurrClient[];
extern const Action *const NeuroActionChainNormalCurrClient[];
extern const Action *const NeuroActionChainFullscreenCurrClient[];
extern const Action *const NeuroActionChainToggleFullscreenCurrClient[];
extern const Action *const NeuroActionChainMinimizeCurrClient[];
extern const Action *const NeuroActionChainFreeCurrClient[];
extern const Action *const NeuroActionChainToggleFreeCurrClient[];

// PtrClient (Action Chains)
extern const Action *const NeuroActionChainFocusPtrClient[];
extern const Action *const NeuroActionChainFreeMovePtrClient[];
extern const Action *const NeuroActionChainFreeResizePtrClient[];
extern const Action *const NeuroActionChainFloatMovePtrClient[];
extern const Action *const NeuroActionChainFloatResizePtrClient[];
extern const Action *const NeuroActionChainToggleFullscreenPtrClient[];
extern const Action *const NeuroActionChainFreePtrClient[];
extern const Action *const NeuroActionChainToggleFreePtrClient[];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager (Handlers)
void NeuroActionHandlerNothing(GenericArg null_arg);
void NeuroActionHandlerQuit(GenericArg null_arg);
void NeuroActionHandlerReload(GenericArg null_arg);
void NeuroActionHandlerChangeNeurowmName(GenericArg string_arg);
void NeuroActionHandlerSpawn(GenericArg command_arg);
void NeuroActionHandlerSleep(GenericArg int_arg);
void NeuroActionHandlerStartCpuCalc(GenericArg null_arg);
void NeuroActionHandlerStopCpuCalc(GenericArg null_arg);

// Layout (Handlers)
void NeuroActionHandlerChangeLayout(GenericArg int_arg);
void NeuroActionHandlerResetLayout(GenericArg null_arg);
void NeuroActionHandlerToggleLayout(GenericArg int_arg);
void NeuroActionHandlerToggleModLayout(GenericArg uint_arg);
void NeuroActionHandlerIncreaseMasterLayout(GenericArg int_arg);
void NeuroActionHandlerResizeMasterLayout(GenericArg float_arg);

// Workspace (Handlers)
void NeuroActionHandlerChangeWorkspace(GenericArg int_arg);
void NeuroActionHandlerRelativeChangeWorkspace(GenericArg WorkspaceSelectorFn_arg);
void NeuroActionHandlerRestoreLastMinimized(GenericArg null_arg);
void NeuroActionHandlerToggleScratchpad(GenericArg command_arg);

// CurrClient (Handlers)
void NeuroActionHandlerFocusCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerSwapCurrClient(GenericArg clientSelectorFn_arg);
void NeuroActionHandlerSendCurrClient(GenericArg int_arg);
void NeuroActionHandlerRelativeSendCurrClient(GenericArg WorkspaceSelectorFn_arg);
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

// Util
void NeuroActionUtilRunAction(const Action *a, const GenericMaybeArg *arg);
void NeuroActionUtilRunActionChain(const ActionChain *ac);

#endif  // NEURO_ACTION_H_

