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
void freeW(int ws, const void *freeSetterFn);
void changeToWorkspaceW(int ws);
void minimizeW(int ws);
void restoreLastMinimizedW(int ws);
void addEnterNotifyMaskW(int ws);
void rmvEnterNotifyMaskW(int ws);

// Client
void focusClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void swapClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void sendClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *ws);
void killClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void minimizeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void tileClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void freeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *freeSetterFn);
void toggleFreeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *freeSetterFn);
void normalClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void fullScreenClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void toggleFullScreenClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void floatMoveClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void floatResizeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void freeMoveClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);
void freeResizeClientW(ClientPtrPtr c, const ClientSelectorFn csf, const void *data);

// Workspace Selectors
int prevW();
int nextW();
int oldW();

// Get functions
ClientPtrPtr getPtrClientW(int *x, int *y);

// Find functions
ClientPtrPtr findWindowClientAllW(Window w);
ClientPtrPtr findWindowClientW(int ws, Window w);
ClientPtrPtr findUrgentClientW(int ws);
ClientPtrPtr findFixedClientW(int ws);

#endif  // NEURO_WORKSPACE_H_

