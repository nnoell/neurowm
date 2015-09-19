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
extern const Action const stopCpuCalcActionA[];

// Layout (Actions)
extern const Action const changeLayoutActionA[];
extern const Action const resetLayoutActionA[];
extern const Action const toggleLayoutActionA[];
extern const Action const toggleModLayoutActionA[];
extern const Action const increaseMasterLayoutActionA[];
extern const Action const resizeMasterLayoutActionA[];

// Workspace (Actions)
extern const Action const changeWorkspaceActionA[];
extern const Action const changeRelWorkspaceActionA[];
extern const Action const restoreLastMinimizedActionA[];
extern const Action const toggleNSPActionA[];

// CurrClient (Actions)
extern const Action const focusCurrClientActionA[];
extern const Action const swapCurrClientActionA[];
extern const Action const sendCurrClientActionA[];
extern const Action const sendCurrClientRelativeActionA[];
extern const Action const killCurrClientActionA[];
extern const Action const tileCurrClientActionA[];
extern const Action const normalCurrClientActionA[];
extern const Action const fullScreenCurrClientActionA[];
extern const Action const toggleFullScreenCurrClientActionA[];
extern const Action const minimizeCurrClientActionA[];
extern const Action const freeCurrClientActionA[];
extern const Action const toggleFreeCurrClientActionA[];

// PtrClient (Actions)
extern const Action const focusPtrClientActionA[];
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
extern const Action *const stopCpuCalcA[];

// Layout (Action Chains)
extern const Action *const changeLayoutA[];
extern const Action *const resetLayoutA[];
extern const Action *const toggleLayoutA[];
extern const Action *const toggleModLayoutA[];
extern const Action *const increaseMasterLayoutA[];
extern const Action *const resizeMasterLayoutA[];

// Workspace (Action Chains)
extern const Action *const changeWorkspaceA[];
extern const Action *const changeRelWorkspaceA[];
extern const Action *const restoreLastMinimizedA[];
extern const Action *const toggleNSPA[];

// CurrClient (Action Chains)
extern const Action *const focusCurrClientA[];
extern const Action *const swapCurrClientA[];
extern const Action *const sendCurrClientA[];
extern const Action *const sendCurrClientRelativeA[];
extern const Action *const sendCurrClientFollowA[];
extern const Action *const sendCurrClientFollowRelativeA[];
extern const Action *const killCurrClientA[];
extern const Action *const tileCurrClientA[];
extern const Action *const normalCurrClientA[];
extern const Action *const fullScreenCurrClientA[];
extern const Action *const toggleFullScreenCurrClientA[];
extern const Action *const minimizeCurrClientA[];
extern const Action *const freeCurrClientA[];
extern const Action *const toggleFreeCurrClientA[];

// PtrClient (Action Chains)
extern const Action *const focusPtrClientA[];
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
void stopCpuCalcHandlerA(GenericArg null_arg);

// Layout (Handlers)
void changeLayoutHandlerA(GenericArg int_arg);
void resetLayoutHandlerA(GenericArg null_arg);
void toggleLayoutHandlerA(GenericArg int_arg);
void toggleModLayoutHandlerA(GenericArg uint_arg);
void increaseMasterLayoutHandlerA(GenericArg int_arg);
void resizeMasterLayoutHandlerA(GenericArg float_arg);

// Workspace (Handlers)
void changeWorkspaceHandlerA(GenericArg int_arg);
void changeRelWorkspaceHandlerA(GenericArg WorkspaceSelectorFn_arg);
void sendCurrClientHandlerA(GenericArg int_arg);
void sendCurrClientRelativeHandlerA(GenericArg WorkspaceSelectorFn_arg);
void restoreLastMinimizedHandlerA(GenericArg null_arg);
void toggleNSPHandlerA(GenericArg command_arg);

// CurrClient (Handlers)
void focusCurrClientHandlerA(GenericArg clientSelectorFn_arg);
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
void focusPtrClientHandlerA(GenericArg clientSelectorFn_arg);
void freeMovePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void freeResizePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void movePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void resizePtrClientHandlerA(GenericArg clientSelectorFn_arg);
void toggleFullScreenPtrClientHandlerA(GenericArg clientSelectorFn_arg);
void freePtrClientHandlerA(GenericArg freeSetterFn_arg);
void toggleFreePtrClientHandlerA(GenericArg freeSetterFn_arg);

// Main
void runActionA(const Action *a, const GenericMaybeArg *arg);
void runActionChainA(const ActionChain *ac);


#endif  // NEURO_ACTION_H_

