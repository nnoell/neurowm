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

#pragma once

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager (Actions)
extern const NeuroAction NeuroActionNothing;
extern const NeuroAction NeuroActionQuit;
extern const NeuroAction NeuroActionReload;
extern const NeuroAction NeuroActionSpawn;
extern const NeuroAction NeuroActionSleep;
extern const NeuroAction NeuroActionInitCpuCalc;
extern const NeuroAction NeuroActionStopCpuCalc;

// NeuroLayout (Actions)
extern const NeuroAction NeuroActionChangeLayout;
extern const NeuroAction NeuroActionResetLayout;
extern const NeuroAction NeuroActionToggleLayout;
extern const NeuroAction NeuroActionToggleModLayout;
extern const NeuroAction NeuroActionIncreaseMasterLayout;
extern const NeuroAction NeuroActionResizeMasterLayout;

// NeuroWorkspace (Actions)
extern const NeuroAction NeuroActionChangeWorkspace;
extern const NeuroAction NeuroActionSelectMonitor;
extern const NeuroAction NeuroActionRestoreLastMinimized;
extern const NeuroAction NeuroActionToggleScatchpad;

// CurrClient (Actions)
extern const NeuroAction NeuroActionFocusCurrClient;
extern const NeuroAction NeuroActionSwapCurrClient;
extern const NeuroAction NeuroActionSendCurrClient;
extern const NeuroAction NeuroActionKillCurrClient;
extern const NeuroAction NeuroActionTileCurrClient;
extern const NeuroAction NeuroActionNormalCurrClient;
extern const NeuroAction NeuroActionFullscreenCurrClient;
extern const NeuroAction NeuroActionToggleFullscreenCurrClient;
extern const NeuroAction NeuroActionMinimizeCurrClient;
extern const NeuroAction NeuroActionFreeCurrClient;
extern const NeuroAction NeuroActionToggleFreeCurrClient;

// PtrClient (Actions)
extern const NeuroAction NeuroActionFocusPtrClient;
extern const NeuroAction NeuroActionFreeMovePtrClient;
extern const NeuroAction NeuroActionFreeResizePtrClient;
extern const NeuroAction NeuroActionFloatMovePtrClient;
extern const NeuroAction NeuroActionFloatResizePtrClient;
extern const NeuroAction NeuroActionToggleFullscreenPtrClient;
extern const NeuroAction NeuroActionFreePtrClient;
extern const NeuroAction NeuroActionToggleFreePtrClient;

// Window Manager (NeuroAction Lists)
extern const NeuroAction* NeuroActionListNothing[];
extern const NeuroAction* NeuroActionListQuit[];
extern const NeuroAction* NeuroActionListReload[];
extern const NeuroAction* NeuroActionListSpawn[];
extern const NeuroAction* NeuroActionListSleep[];
extern const NeuroAction* NeuroActionListInitCpuCalc[];
extern const NeuroAction* NeuroActionListStopCpuCalc[];

// NeuroLayout (NeuroAction Lists)
extern const NeuroAction* NeuroActionListChangeLayout[];
extern const NeuroAction* NeuroActionListResetLayout[];
extern const NeuroAction* NeuroActionListToggleLayout[];
extern const NeuroAction* NeuroActionListToggleModLayout[];
extern const NeuroAction* NeuroActionListIncreaseMasterLayout[];
extern const NeuroAction* NeuroActionListResizeMasterLayout[];

// NeuroWorkspace (NeuroAction Lists)
extern const NeuroAction* NeuroActionListChangeWorkspace[];
extern const NeuroAction* NeuroActionListSelectMonitor[];
extern const NeuroAction* NeuroActionListRestoreLastMinimized[];
extern const NeuroAction* NeuroActionListToggleScratchpad[];

// CurrClient (NeuroAction Lists)
extern const NeuroAction* NeuroActionListFocusCurrClient[];
extern const NeuroAction* NeuroActionListSwapCurrClient[];
extern const NeuroAction* NeuroActionListSendCurrClient[];
extern const NeuroAction* NeuroActionListSendFollowCurrClient[];
extern const NeuroAction* NeuroActionListKillClient[];
extern const NeuroAction* NeuroActionListTileCurrClient[];
extern const NeuroAction* NeuroActionListNormalCurrClient[];
extern const NeuroAction* NeuroActionListFullscreenCurrClient[];
extern const NeuroAction* NeuroActionListToggleFullscreenCurrClient[];
extern const NeuroAction* NeuroActionListMinimizeCurrClient[];
extern const NeuroAction* NeuroActionListFreeCurrClient[];
extern const NeuroAction* NeuroActionListToggleFreeCurrClient[];

// PtrClient (NeuroAction Lists)
extern const NeuroAction* NeuroActionListFocusPtrClient[];
extern const NeuroAction* NeuroActionListFreeMovePtrClient[];
extern const NeuroAction* NeuroActionListFreeResizePtrClient[];
extern const NeuroAction* NeuroActionListFloatMovePtrClient[];
extern const NeuroAction* NeuroActionListFloatResizePtrClient[];
extern const NeuroAction* NeuroActionListToggleFullscreenPtrClient[];
extern const NeuroAction* NeuroActionListFreePtrClient[];
extern const NeuroAction* NeuroActionListToggleFreePtrClient[];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager (Handlers)
void NeuroActionHandlerNothing(NeuroArg null_arg);
void NeuroActionHandlerQuit(NeuroArg null_arg);
void NeuroActionHandlerReload(NeuroArg null_arg);
void NeuroActionHandlerChangeWmName(NeuroArg string_arg);
void NeuroActionHandlerSpawn(NeuroArg command_arg);
void NeuroActionHandlerSleep(NeuroArg uint_arg);
void NeuroActionHandlerInitCpuCalc(NeuroArg null_arg);
void NeuroActionHandlerStopCpuCalc(NeuroArg null_arg);

// NeuroLayout (Handlers)
void NeuroActionHandlerChangeLayout(NeuroArg int_arg);
void NeuroActionHandlerResetLayout(NeuroArg null_arg);
void NeuroActionHandlerToggleLayout(NeuroArg idx_arg);
void NeuroActionHandlerToggleModLayout(NeuroArg lmod_arg);
void NeuroActionHandlerIncreaseMasterLayout(NeuroArg int_arg);
void NeuroActionHandlerResizeMasterLayout(NeuroArg float_arg);

// NeuroWorkspace (Handlers)
void NeuroActionHandlerChangeWorkspace(NeuroArg WorkspaceSelectorFn_arg);
void NeuroActionHandlerSelectMonitor(NeuroArg MonitorSelectorFn_arg);
void NeuroActionHandlerRestoreLastMinimized(NeuroArg null_arg);
void NeuroActionHandlerToggleScratchpad(NeuroArg command_arg);

// CurrClient (Handlers)
void NeuroActionHandlerFocusCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerSwapCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerSendCurrClient(NeuroArg WorkspaceSelectorFn_arg);
void NeuroActionHandlerKillCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerTileCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerNormalCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFullscreenCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerToggleFullscreenCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerMinimizeCurrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFreeCurrClient(NeuroArg freeSetterFn_arg);
void NeuroActionHandlerToggleFreeCurrClient(NeuroArg freeSetterFn_arg);

// PtrClient (Handlers)
void NeuroActionHandlerFocusPtrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFreeMovePtrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFreeResizePtrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFloatMovePtrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFloatResizePtrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerToggleFullscreenPtrClient(NeuroArg clientSelectorFn_arg);
void NeuroActionHandlerFreePtrClient(NeuroArg freeSetterFn_arg);
void NeuroActionHandlerToggleFreePtrClient(NeuroArg freeSetterFn_arg);

// Run
void NeuroActionRunAction(const NeuroAction *a, const NeuroMaybeArg *arg);
void NeuroActionRunActionChain(const NeuroActionChain *ac);

