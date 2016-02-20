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

// NeuroWorkspace
void NeuroWorkspaceChange(size_t ws);
void NeuroWorkspaceUpdate(size_t ws);
void NeuroWorkspaceFocus(size_t ws);
void NeuroWorkspaceUnfocus(size_t ws);
void NeuroWorkspaceTile(size_t ws);
void NeuroWorkspaceFree(size_t ws, const void *free_setter_fn);
void NeuroWorkspaceMinimize(size_t ws);
void NeuroWorkspaceRestoreLastMinimized(size_t ws);
void NeuroWorkspaceAddEnterNotifyMask(size_t ws);
void NeuroWorkspaceRemoveEnterNotifyMask(size_t ws);

// Find
NeuroClientPtrPtr NeuroWorkspaceClientFindWindow(size_t ws, Window w);
NeuroClientPtrPtr NeuroWorkspaceClientFindUrgent(size_t ws);
NeuroClientPtrPtr NeuroWorkspaceClientFindFixed(size_t ws);

// NeuroClient
void NeuroWorkspaceClientFocus(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSwap(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSend(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *ws);
void NeuroWorkspaceClientKill(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientMinimize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientTile(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFree(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *freeSetterFn);
void NeuroWorkspaceClientToggleFree(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *free_setter_fn);
void NeuroWorkspaceClientNormal(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFullscreen(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientToggleFullscreen(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatMove(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatResize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeMove(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeResize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);

// NeuroWorkspace Selectors
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

