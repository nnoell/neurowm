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
void moveFocusN(ActionAr arg);
void swapCliN(ActionAr arg);
void killCliN(ActionAr arg);
void changeLayoutN(ActionAr arg);
void resetLayoutN(ActionAr arg);
void increaseMasterN(ActionAr arg);
void resizeMasterN(ActionAr arg);
void changeToWorkspaceN(ActionAr arg);
void changeToPrevWorkspaceN(ActionAr arg);
void changeToNextWorkspaceN(ActionAr arg);
void changeToLastWorkspaceN(ActionAr arg);
void tileCliN(ActionAr arg);
void freeCliN(ActionAr arg);
void toggleFreeCliN(ActionAr arg);
void normalCliN(ActionAr arg);
void fullScreenCliN(ActionAr arg);
void toggleFullScreenCliN(ActionAr arg);
void toggleFreePtrCliN(ActionAr arg);
void toggleFullScreenPtrCliN(ActionAr arg);
void freeMovePointerCliN(ActionAr arg);
void freeResizePointerCliN(ActionAr arg);
void movePointerCliN(ActionAr arg);
void resizePointerCliN(ActionAr arg);
void toggleLayoutModN(ActionAr arg);
void toggleLayoutN(ActionAr arg);
void moveCliToWorkspaceN(ActionAr arg);
void moveCliToWorkspaceAndFollowN(ActionAr arg);
void toggleNSPN(ActionAr arg);
void minimizeCliN(ActionAr arg);
void restoreCliN(ActionAr arg);

// Main function
int neurowm(const WMConfig *c);

#endif  // NEURO_NEUROWM_H_

