//----------------------------------------------------------------------------------------------------------------------
// Module      :  neurowm
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_NEUROWM_H_
#define NEURO_NEUROWM_H_

// Includes
#include "system.h"
#include "config.h"
#include "layout.h"
#include "client.h"
#include "workspace.h"
#include "rule.h"
#include "dzenpanel.h"
#include "personal.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void changeWMNameN(ActionAr string_arg);
void spawnN(ActionAr command_arg);
void quitN(ActionAr no_arg);
void reloadN(ActionAr no_arg);

// Layout
void changeLayoutN(ActionAr int_arg);
void resetLayoutN(ActionAr no_arg);
void toggleLayoutModN(ActionAr int_arg);
void toggleLayoutN(ActionAr int_arg);
void increaseMasterLayoutN(ActionAr int_arg);
void resizeMasterLayoutN(ActionAr float_arg);

// Workspace
void changeToWorkspaceN(ActionAr int_arg);
void changeToPrevWorkspaceN(ActionAr no_arg);
void changeToNextWorkspaceN(ActionAr no_arg);
void changeToLastWorkspaceN(ActionAr no_arg);
void moveClientToWorkspaceN(ActionAr int_arg);
void moveClientToWorkspaceAndFollowN(ActionAr int_arg);
void restoreLastMinimizedN(ActionAr no_arg);
void toggleNSPN(ActionAr command_arg);

// Curr Client
void moveFocusCurrClientN(ActionAr selectCliFn_arg);
void swapCurrClientN(ActionAr selectCliFn_arg);
void killCurrClientN(ActionAr selectCliFn_arg);
void tileCurrClientN(ActionAr selectCliFn_arg);
void normalCurrClientN(ActionAr selectCliFn_arg);
void fullScreenCurrClientN(ActionAr selectCliFn_arg);
void toggleFullScreenCurrClientN(ActionAr selectCliFn_arg);
void minimizeCurrClientN(ActionAr selectCliFn_arg);
void freeCurrClientN(ActionAr freeLocFn_arg);
void toggleFreeCurrClientN(ActionAr freeLocFn_arg);

// Ptr Client
void moveFocusPtrClientN(ActionAr selectCliFn_arg);
void freeMovePtrClientN(ActionAr selectCliFn_arg);
void freeResizePtrClientN(ActionAr selectCliFn_arg);
void movePtrClientN(ActionAr selectCliFn_arg);
void resizePtrClientN(ActionAr selectCliFn_arg);
void toggleFullScreenPtrClientN(ActionAr selectCliFn_arg);
void freePtrClientN(ActionAr freeLocFn_arg);
void toggleFreePtrClientN(ActionAr freeLocFn_arg);

// Main function
int neurowm(const WMConfig *c);

#endif  // NEURO_NEUROWM_H_

