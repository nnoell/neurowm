//----------------------------------------------------------------------------------------------------------------------
// Module      :  workspace
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_WORKSPACE_H_
#define NEURO_WORKSPACE_H_

// Includes
#include "general.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void updateW(int ws);
void updateFocusW(int ws);
void moveFocusW(const CliPtr c, const SelectCliF sf);
void swapClientsW(const CliPtr c, const SelectCliF sf);
void hideW(int ws, Bool doRules);
void showW(int ws, Bool doRules);
void tileW(int ws);
void freeW(int ws, const FreeLocF ff);
void changeToWorkspaceW(int ws);
void moveCliToWorkspaceW(CliPtr c, int ws);
void moveCliToWorkspaceAndFollowW(CliPtr c, int ws);
void minimizeW(int ws);
void restoreLastMinimizedW(int ws);
void addEnterNotifyMaskW(int ws);
void rmvEnterNotifyMaskW(int ws);

// Find functions
CliPtr findWindowClientAllW(Window w);
CliPtr findWindowClientW(int ws, Window w);
CliPtr findUrgentClientW(int ws);
CliPtr findFixedClientW(int ws);

#endif  // NEURO_WORKSPACE_H_

