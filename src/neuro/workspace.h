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
void changeW(int ws);
void updateW(int ws);
void updateFocusW(int ws);
void hideW(int ws, Bool doRules);
void showW(int ws, Bool doRules);
void tileW(int ws);
void freeW(int ws, const void *freeSetterFn);
void minimizeW(int ws);
void restoreLastMinimizedW(int ws);
void addEnterNotifyMaskW(int ws);
void rmvEnterNotifyMaskW(int ws);

// Client
void focusClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void swapClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void sendClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *ws);
void killClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void minimizeClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void tileClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void freeClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn);
void toggleFreeClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn);
void normalClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void fullScreenClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void toggleFullScreenClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void floatMoveClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void floatResizeClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void freeMoveClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void freeResizeClientW(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);

// Workspace Selectors
int idx0W();
int idx1W();
int idx2W();
int idx3W();
int idx4W();
int idx5W();
int idx6W();
int idx7W();
int idx8W();
int idx9W();
int currW();
int prevW();
int nextW();
int oldW();

// Find functions
ClientPtrPtr findWindowClientAllW(Window w);
ClientPtrPtr findWindowClientW(int ws, Window w);
ClientPtrPtr findUrgentClientW(int ws);
ClientPtrPtr findFixedClientW(int ws);

#endif  // NEURO_WORKSPACE_H_

