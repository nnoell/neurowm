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

#pragma once

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Workspace
void NeuroWorkspaceChange(size_t ws);
void NeuroWorkspaceUpdate(size_t ws);
void NeuroWorkspaceFocus(size_t ws);
void NeuroWorkspaceHide(size_t ws, bool doRules);
void NeuroWorkspaceShow(size_t ws, bool doRules);
void NeuroWorkspaceTile(size_t ws);
void NeuroWorkspaceFree(size_t ws, const void *free_setter_fn);
void NeuroWorkspaceMinimize(size_t ws);
void NeuroWorkspaceRestoreLastMinimized(size_t ws);
void NeuroWorkspaceAddEnterNotifyMask(size_t ws);
void NeuroWorkspaceRemoveEnterNotifyMask(size_t ws);

// Find
ClientPtrPtr NeuroWorkspaceClientFindWindow(size_t ws, Window w);
ClientPtrPtr NeuroWorkspaceClientFindUrgent(size_t ws);
ClientPtrPtr NeuroWorkspaceClientFindFixed(size_t ws);

// Client
void NeuroWorkspaceClientFocus(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSwap(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSend(ClientPtrPtr ref, const ClientSelectorFn csf, const void *ws);
void NeuroWorkspaceClientKill(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientMinimize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientTile(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFree(ClientPtrPtr ref, const ClientSelectorFn csf, const void *freeSetterFn);
void NeuroWorkspaceClientToggleFree(ClientPtrPtr ref, const ClientSelectorFn csf, const void *free_setter_fn);
void NeuroWorkspaceClientNormal(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFullscreen(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientToggleFullscreen(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatMove(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatResize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeMove(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeResize(ClientPtrPtr ref, const ClientSelectorFn csf, const void *data);

// Workspace Selectors
size_t NeuroWorkspaceSelector0();
size_t NeuroWorkspaceSelector1();
size_t NeuroWorkspaceSelector2();
size_t NeuroWorkspaceSelector3();
size_t NeuroWorkspaceSelector4();
size_t NeuroWorkspaceSelector5();
size_t NeuroWorkspaceSelector6();
size_t NeuroWorkspaceSelector7();
size_t NeuroWorkspaceSelector8();
size_t NeuroWorkspaceSelector9();
size_t NeuroWorkspaceSelectorCurr();
size_t NeuroWorkspaceSelectorPrev();
size_t NeuroWorkspaceSelectorNext();
size_t NeuroWorkspaceSelectorOld();

