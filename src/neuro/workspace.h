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
NeuroIndex NeuroWorkspaceSelector0(void);
NeuroIndex NeuroWorkspaceSelector1(void);
NeuroIndex NeuroWorkspaceSelector2(void);
NeuroIndex NeuroWorkspaceSelector3(void);
NeuroIndex NeuroWorkspaceSelector4(void);
NeuroIndex NeuroWorkspaceSelector5(void);
NeuroIndex NeuroWorkspaceSelector6(void);
NeuroIndex NeuroWorkspaceSelector7(void);
NeuroIndex NeuroWorkspaceSelector8(void);
NeuroIndex NeuroWorkspaceSelector9(void);
NeuroIndex NeuroWorkspaceSelectorCurr(void);
NeuroIndex NeuroWorkspaceSelectorPrev(void);
NeuroIndex NeuroWorkspaceSelectorNext(void);
NeuroIndex NeuroWorkspaceSelectorOld(void);

