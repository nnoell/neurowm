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
void changeWMNameN(GenericAr arg);
void spawnN(GenericAr arg);
void quitN(GenericAr arg);
void reloadN(GenericAr arg);
void moveFocusN(GenericAr arg);
void swapCliN(GenericAr arg);
void killCliN(GenericAr arg);
void changeLayoutN(GenericAr arg);
void resetLayoutN(GenericAr arg);
void increaseMasterN(GenericAr arg);
void resizeMasterN(GenericAr arg);
void changeToWorkspaceN(GenericAr arg);
void changeToPrevWorkspaceN(GenericAr arg);
void changeToNextWorkspaceN(GenericAr arg);
void changeToLastWorkspaceN(GenericAr arg);
void tileCliN(GenericAr arg);
void freeCliN(GenericAr arg);
void toggleFreeCliN(GenericAr arg);
void normalCliN(GenericAr arg);
void fullScreenCliN(GenericAr arg);
void toggleFullScreenCliN(GenericAr arg);
void toggleFreePtrCliN(GenericAr arg);
void toggleFullScreenPtrCliN(GenericAr arg);
void freeMovePointerCliN(GenericAr arg);
void freeResizePointerCliN(GenericAr arg);
void movePointerCliN(GenericAr arg);
void resizePointerCliN(GenericAr arg);
void toggleLayoutModN(GenericAr arg);
void toggleLayoutN(GenericAr arg);
void moveCliToWorkspaceN(GenericAr arg);
void moveCliToWorkspaceAndFollowN(GenericAr arg);
void toggleNSPN(GenericAr arg);
void minimizeCliN(GenericAr arg);
void restoreCliN(GenericAr arg);

// Main function
int neurowm(const WMConfig *c);

#endif  // NEURO_NEUROWM_H_

