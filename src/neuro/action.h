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
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void quitNeurowmA(ActionAr no_arg);
void reloadNeurowmA(ActionAr no_arg);
void changeNeurowmNameA(ActionAr string_arg);
void spawnA(ActionAr command_arg);
void sleepA(ActionAr int_arg);
void startCpuCalcA(ActionAr no_arg);
void endCpuCalcA(ActionAr no_arg);

// Layout
void changeLayoutA(ActionAr int_arg);
void resetLayoutA(ActionAr no_arg);
void toggleLayoutModA(ActionAr int_arg);
void toggleLayoutA(ActionAr int_arg);
void increaseMasterLayoutA(ActionAr int_arg);
void resizeMasterLayoutA(ActionAr float_arg);

// Workspace
void changeToWorkspaceA(ActionAr int_arg);
void changeToPrevWorkspaceA(ActionAr no_arg);
void changeToNextWorkspaceA(ActionAr no_arg);
void changeToLastWorkspaceA(ActionAr no_arg);
void moveClientToWorkspaceA(ActionAr int_arg);
void moveClientToWorkspaceAndFollowA(ActionAr int_arg);
void restoreLastMinimizedA(ActionAr no_arg);
void toggleNSPA(ActionAr command_arg);

// CurrClient
void moveFocusCurrClientA(ActionAr selectCliFn_arg);
void swapCurrClientA(ActionAr selectCliFn_arg);
void killCurrClientA(ActionAr selectCliFn_arg);
void tileCurrClientA(ActionAr selectCliFn_arg);
void normalCurrClientA(ActionAr selectCliFn_arg);
void fullScreenCurrClientA(ActionAr selectCliFn_arg);
void toggleFullScreenCurrClientA(ActionAr selectCliFn_arg);
void minimizeCurrClientA(ActionAr selectCliFn_arg);
void freeCurrClientA(ActionAr freeLocFn_arg);
void toggleFreeCurrClientA(ActionAr freeLocFn_arg);

// PtrClient
void moveFocusPtrClientA(ActionAr selectCliFn_arg);
void freeMovePtrClientA(ActionAr selectCliFn_arg);
void freeResizePtrClientA(ActionAr selectCliFn_arg);
void movePtrClientA(ActionAr selectCliFn_arg);
void resizePtrClientA(ActionAr selectCliFn_arg);
void toggleFullScreenPtrClientA(ActionAr selectCliFn_arg);
void freePtrClientA(ActionAr freeLocFn_arg);
void toggleFreePtrClientA(ActionAr freeLocFn_arg);


#endif  // NEURO_ACTION_H_

