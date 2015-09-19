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
void NeuroWorkspaceChange(int ws);
void NeuroWorkspaceUpdate(int ws);
void NeuroWorkspaceFocus(int ws);
void NeuroWorkspaceHide(int ws, Bool doRules);
void NeuroWorkspaceShow(int ws, Bool doRules);
void NeuroWorkspaceTile(int ws);
void NeuroWorkspaceFree(int ws, const void *freeSetterFn);
void NeuroWorkspaceMinimize(int ws);
void NeuroWorkspaceRestoreLastMinimized(int ws);
void NeuroWorkspaceAddEnterNotifyMask(int ws);
void NeuroWorkspaceRemoveEnterNotifyMask(int ws);

// Find
ClientPtrPtr NeuroWorkspaceClientFindWindow(int ws, Window w);
ClientPtrPtr NeuroWorkspaceClientFindUrgent(int ws);
ClientPtrPtr NeuroWorkspaceClientFindFixed(int ws);

// Client
void NeuroWorkspaceClientFocus(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSwap(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSend(ClientPtrPtr ref, const ClientSelectorFn csf, const void *ws);
void NeuroWorkspaceClientKill(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientMinimize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientTile(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFree(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn);
void NeuroWorkspaceClientToggleFree(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn);
void NeuroWorkspaceClientNormal(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFullscreen(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientToggleFullscreen(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatMove(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatResize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeMove(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeResize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);

// Workspace Selectors
int NeuroWorkspaceSelector0();
int NeuroWorkspaceSelector1();
int NeuroWorkspaceSelector2();
int NeuroWorkspaceSelector3();
int NeuroWorkspaceSelector4();
int NeuroWorkspaceSelector5();
int NeuroWorkspaceSelector6();
int NeuroWorkspaceSelector7();
int NeuroWorkspaceSelector8();
int NeuroWorkspaceSelector9();
int NeuroWorkspaceSelectorCurr();
int NeuroWorkspaceSelectorPrev();
int NeuroWorkspaceSelectorNext();
int NeuroWorkspaceSelectorOld();

#endif  // NEURO_WORKSPACE_H_

