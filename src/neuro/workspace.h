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
void NeuroWorkspaceChange(NeuroIndex ws);
void NeuroWorkspaceUpdate(NeuroIndex ws);
void NeuroWorkspaceFocus(NeuroIndex ws);
void NeuroWorkspaceUnfocus(NeuroIndex ws);
void NeuroWorkspaceTile(NeuroIndex ws);
void NeuroWorkspaceFree(NeuroIndex ws, const void *free_setter_fn);
void NeuroWorkspaceMinimize(NeuroIndex ws);
void NeuroWorkspaceRestoreLastMinimized(NeuroIndex ws);
void NeuroWorkspaceAddEnterNotifyMask(NeuroIndex ws);
void NeuroWorkspaceRemoveEnterNotifyMask(NeuroIndex ws);

// Find
NeuroClientPtrPtr NeuroWorkspaceClientFindWindow(NeuroIndex ws, Window w);
NeuroClientPtrPtr NeuroWorkspaceClientFindUrgent(NeuroIndex ws);
NeuroClientPtrPtr NeuroWorkspaceClientFindFixed(NeuroIndex ws);
NeuroClientPtrPtr NeuroWorkspaceClientFindPointed(NeuroIndex ws, const NeuroPoint *p);

// Client
void NeuroWorkspaceClientFocus(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSwap(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientSend(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *ws);
void NeuroWorkspaceClientKill(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientMinimize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientTile(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFree(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *free_setter_fn);
void NeuroWorkspaceClientToggleFree(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *free_setter_fn);
void NeuroWorkspaceClientNormal(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFullscreen(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientToggleFullscreen(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatMove(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFloatResize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeMove(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);
void NeuroWorkspaceClientFreeResize(NeuroClientPtrPtr ref, const NeuroClientSelectorFn csf, const void *data);

// NeuroWorkspace Selectors
NeuroIndex NeuroWorkspaceSelector0();
NeuroIndex NeuroWorkspaceSelector1();
NeuroIndex NeuroWorkspaceSelector2();
NeuroIndex NeuroWorkspaceSelector3();
NeuroIndex NeuroWorkspaceSelector4();
NeuroIndex NeuroWorkspaceSelector5();
NeuroIndex NeuroWorkspaceSelector6();
NeuroIndex NeuroWorkspaceSelector7();
NeuroIndex NeuroWorkspaceSelector8();
NeuroIndex NeuroWorkspaceSelector9();
NeuroIndex NeuroWorkspaceSelectorCurr();
NeuroIndex NeuroWorkspaceSelectorPrev();
NeuroIndex NeuroWorkspaceSelectorNext();
NeuroIndex NeuroWorkspaceSelectorOld();

