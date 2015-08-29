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
extern const Action const nothingActionA[];
extern const Action const quitNeurowmActionA[];
extern const Action const reloadNeurowmActionA[];
extern const Action const spawnActionA[];
extern const Action const sleepActionA[];
extern const Action const startCpuCalcActionA[];
extern const Action const endCpuCalcActionA[];

// Layout (Actions)
extern const Action const changeLayoutActionA[];
extern const Action const resetLayoutActionA[];
extern const Action const toggleLayoutActionA[];
extern const Action const toggleLayoutModActionA[];
extern const Action const increaseMasterLayoutActionA[];
extern const Action const resizeMasterLayoutActionA[];

// Workspace (Actions)
extern const Action const changeToWorkspaceActionA[];
extern const Action const changeToRelWorkspaceActionA[];
extern const Action const moveClientToWorkspaceActionA[];
extern const Action const moveClientToRelWorkspaceActionA[];
extern const Action const restoreLastMinimizedActionA[];
extern const Action const toggleNSPActionA[];

// CurrClient (Actions)
extern const Action const moveFocusCurrClientActionA[];
extern const Action const swapCurrClientActionA[];
extern const Action const killCurrClientActionA[];
extern const Action const tileCurrClientActionA[];
extern const Action const normalCurrClientActionA[];
extern const Action const fullScreenCurrClientActionA[];
extern const Action const toggleFullScreenCurrClientActionA[];
extern const Action const minimizeCurrClientActionA[];
extern const Action const freeCurrClientActionA[];
extern const Action const toggleFreeCurrClientActionA[];

// PtrClient (Actions)
extern const Action const moveFocusPtrClientActionA[];
extern const Action const freeMovePtrClientActionA[];
extern const Action const freeResizePtrClientActionA[];
extern const Action const movePtrClientActionA[];
extern const Action const resizePtrClientActionA[];
extern const Action const toggleFullScreenPtrClientActionA[];
extern const Action const freePtrClientActionA[];
extern const Action const toggleFreePtrClientActionA[];

// Window Manager (Action Chains)
extern const Action *const nothingA[];
extern const Action *const quitNeurowmA[];
extern const Action *const reloadNeurowmA[];
extern const Action *const spawnA[];
extern const Action *const sleepA[];
extern const Action *const startCpuCalcA[];
extern const Action *const endCpuCalcA[];

// Layout (Action Chains)
extern const Action *const changeLayoutA[];
extern const Action *const resetLayoutA[];
extern const Action *const toggleLayoutA[];
extern const Action *const toggleLayoutModA[];
extern const Action *const increaseMasterLayoutA[];
extern const Action *const resizeMasterLayoutA[];

// Workspace (Action Chains)
extern const Action *const changeToWorkspaceA[];
extern const Action *const changeToRelWorkspaceA[];
extern const Action *const moveClientToWorkspaceA[];
extern const Action *const moveClientToRelWorkspaceA[];
extern const Action *const moveClientToWorkspaceFollowA[];
extern const Action *const moveClientToRelWorkspaceFollowA[];
extern const Action *const restoreLastMinimizedA[];
extern const Action *const toggleNSPA[];

// CurrClient (Action Chains)
extern const Action *const moveFocusCurrClientA[];
extern const Action *const swapCurrClientA[];
extern const Action *const killCurrClientA[];
extern const Action *const tileCurrClientA[];
extern const Action *const normalCurrClientA[];
extern const Action *const fullScreenCurrClientA[];
extern const Action *const toggleFullScreenCurrClientA[];
extern const Action *const minimizeCurrClientA[];
extern const Action *const freeCurrClientA[];
extern const Action *const toggleFreeCurrClientA[];

// PtrClient (Action Chains)
extern const Action *const moveFocusPtrClientA[];
extern const Action *const freeMovePtrClientA[];
extern const Action *const freeResizePtrClientA[];
extern const Action *const movePtrClientA[];
extern const Action *const resizePtrClientA[];
extern const Action *const toggleFullScreenPtrClientA[];
extern const Action *const freePtrClientA[];
extern const Action *const toggleFreePtrClientA[];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager (Handlers)
void nothingHandlerA(GenericArg null_arg);
void quitNeurowmHandlerA(GenericArg null_arg);
void reloadNeurowmHandlerA(GenericArg null_arg);
void changeNeurowmNameHandlerA(GenericArg string_arg);
void spawnHandlerA(GenericArg command_arg);
void sleepHandlerA(GenericArg int_arg);
void startCpuCalcHandlerA(GenericArg null_arg);
void endCpuCalcHandlerA(GenericArg null_arg);

// Layout (Handlers)
void changeLayoutHandlerA(GenericArg int_arg);
void resetLayoutHandlerA(GenericArg null_arg);
void toggleLayoutHandlerA(GenericArg int_arg);
void toggleLayoutModHandlerA(GenericArg uint_arg);
void increaseMasterLayoutHandlerA(GenericArg int_arg);
void resizeMasterLayoutHandlerA(GenericArg float_arg);

// Workspace (Handlers)
void changeToWorkspaceHandlerA(GenericArg int_arg);
void changeToRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg);
void moveClientToWorkspaceHandlerA(GenericArg int_arg);
void moveClientToRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg);
void restoreLastMinimizedHandlerA(GenericArg null_arg);
void toggleNSPHandlerA(GenericArg command_arg);

// CurrClient (Handlers)
void moveFocusCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void swapCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void killCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void tileCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void normalCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void fullScreenCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void toggleFullScreenCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void minimizeCurrClientHandlerA(GenericArg clientSelectorFn_arg);
void freeCurrClientHandlerA(GenericArg freeSetterFn_arg);
void toggleFreeCurrClientHandlerA(GenericArg freeSetterFn_arg);

// PtrClient (Handlers)
void moveFocusPtrClientHandlerA(GenericArg clientSelectorFn_arg);
void freeMovePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void freeResizePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void movePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void resizePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void toggleFullScreenPtrClientHandlerA(GenericArg clientSelectorFn_arg);
void freePtrClientHandlerA(GenericArg freeSetterFn_arg);
void toggleFreePtrClientHandlerA(GenericArg freeSetterFn_arg);

// Main
void runActionA(const Action *a, const ActionChainArg *arg);
void runActionChainA(const ActionChain *ac);


#endif  // NEURO_ACTION_H_

