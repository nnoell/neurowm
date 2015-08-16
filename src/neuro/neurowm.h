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

// Common binding functions
void changeWMNameN(ActionAr arg);
void spawnN(ActionAr arg);
void quitN(ActionAr arg);
void reloadN(ActionAr arg);
void moveFocusClientN(ActionAr arg);
void swapClientN(ActionAr arg);
void killClientN(ActionAr arg);
void changeLayoutN(ActionAr arg);
void resetLayoutN(ActionAr arg);
void increaseMasterN(ActionAr arg);
void resizeMasterN(ActionAr arg);
void changeToWorkspaceN(ActionAr arg);
void changeToPrevWorkspaceN(ActionAr arg);
void changeToNextWorkspaceN(ActionAr arg);
void changeToLastWorkspaceN(ActionAr arg);
void tileClientN(ActionAr arg);
void freeClientN(ActionAr arg);
void toggleFreeCurrClientN(ActionAr arg);
void normalClientN(ActionAr arg);
void fullScreenClientN(ActionAr arg);
void toggleFullScreenClientN(ActionAr arg);
void toggleFreePtrClientN(ActionAr arg);
void toggleFullScreenPtrClientN(ActionAr arg);
void freeMovePtrClientN(ActionAr arg);
void freeResizePtrClientN(ActionAr arg);
void movePtrClientN(ActionAr arg);
void resizePtrClientN(ActionAr arg);
void toggleLayoutModN(ActionAr arg);
void toggleLayoutN(ActionAr arg);
void moveClientToWorkspaceN(ActionAr arg);
void moveClientToWorkspaceAndFollowN(ActionAr arg);
void toggleNSPN(ActionAr arg);
void minimizeClientN(ActionAr arg);
void restoreLastMinimizedClientN(ActionAr arg);

// Main function
int neurowm(const WMConfig *c);

#endif  // NEURO_NEUROWM_H_

