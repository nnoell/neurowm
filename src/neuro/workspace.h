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
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Workspace
void updateW(int ws);
void updateFocusW(int ws);
void hideW(int ws, Bool doRules);
void showW(int ws, Bool doRules);
void tileW(int ws);
void freeW(int ws, const void *freeLocFn);
void changeToWorkspaceW(int ws);
void moveClientToWorkspaceW(CliPtr c, int ws);
void moveClientToWorkspaceAndFollowW(CliPtr c, int ws);
void minimizeW(int ws);
void restoreLastMinimizedW(int ws);
void addEnterNotifyMaskW(int ws);
void rmvEnterNotifyMaskW(int ws);

// Client
void moveFocusClientW(const CliPtr c, const SelectCliFn scf);
void swapClientW(const CliPtr c, const SelectCliFn scf);
void updateClientW(const CliPtr c, const SelectCliFn scf);
void updateClassAndNameClientW(CliPtr c, const SelectCliFn scf);
void updateTitleClientW(CliPtr c, const SelectCliFn scf);
void hideClientW(CliPtr c, const SelectCliFn scf, const void *doRules);
void showClientW(CliPtr c, const SelectCliFn scf, const void *doRules);
void setUrgentClientW(CliPtr c, const SelectCliFn scf);
void unsetUrgentClientW(CliPtr c, const SelectCliFn scf);
void killClientW(const CliPtr c, const SelectCliFn scf);
void minimizeClientW(const CliPtr c, const SelectCliFn scf);
void tileClientW(CliPtr c, const SelectCliFn scf);
void freeClientW(CliPtr c, const SelectCliFn scf, const void *freeLocFn);
void toggleFreeClientW(CliPtr c, const SelectCliFn scf, const void *freeLocFn);
void normalClientW(CliPtr c, const SelectCliFn scf);
void fullScreenClientW(CliPtr c, const SelectCliFn scf);
void toggleFullScreenClientW(const CliPtr c, const SelectCliFn scf);

// Find functions
CliPtr findWindowClientAllW(Window w);
CliPtr findWindowClientW(int ws, Window w);
CliPtr findUrgentClientW(int ws);
CliPtr findFixedClientW(int ws);

#endif  // NEURO_WORKSPACE_H_

