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
#include "base.h"
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
void spawnN(Arg arg);
void quitN(Arg arg);
void reloadN(Arg arg);
void moveFocusN(Arg arg);
void swapCliN(Arg arg);
void killCliN(Arg arg);
void changeLayoutN(Arg arg);
void resetLayoutN(Arg arg);
void increaseMasterN(Arg arg);
void resizeMasterN(Arg arg);
void changeToWorkspaceN(Arg arg);
void changeToPrevWorkspaceN(Arg arg);
void changeToNextWorkspaceN(Arg arg);
void changeToLastWorkspaceN(Arg arg);
void tileCliN(Arg arg);
void freeCliN(Arg arg);
void toggleFreeCliN(Arg arg);
void normalCliN(Arg arg);
void fullScreenCliN(Arg arg);
void toggleFullScreenCliN(Arg arg);
void toggleFreePtrCliN(Arg arg);
void toggleFullScreenPtrCliN(Arg arg);
void freeMovePointerCliN(Arg arg);
void freeResizePointerCliN(Arg arg);
void movePointerCliN(Arg arg);
void resizePointerCliN(Arg arg);
void toggleLayoutModN(Arg arg);
void toggleLayoutN(Arg arg);
void moveCliToWorkspaceN(Arg arg);
void moveCliToWorkspaceAndFollowN(Arg arg);
void toggleNSPN(Arg arg);
void minimizeCliN(Arg arg);
void restoreCliN(Arg arg);

// Main function
int neurowm(const WMConfig *c);

#endif  // NEURO_NEUROWM_H_

