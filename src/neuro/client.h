//----------------------------------------------------------------------------------------------------------------------
// Module      :  client
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

// Basic Functions
void NeuroClientUpdate(NeuroClientPtrPtr c, const void *data);
void NeuroClientUpdateClassAndName(NeuroClientPtrPtr c, const void *data);
void NeuroClientUpdateTitle(NeuroClientPtrPtr c, const void *data);
void NeuroClientSetUrgent(NeuroClientPtrPtr c, const void *data);
void NeuroClientUnsetUrgent(NeuroClientPtrPtr c, const void *data);
void NeuroClientKill(NeuroClientPtrPtr c, const void *data);
void NeuroClientMinimize(NeuroClientPtrPtr c, const void *data);
void NeuroClientTile(NeuroClientPtrPtr c, const void *data);
void NeuroClientFree(NeuroClientPtrPtr c, const void *freeSetterFn);
void NeuroClientToggleFree(NeuroClientPtrPtr c, const void *free_setter_fn);
void NeuroClientNormal(NeuroClientPtrPtr c, const void *data);
void NeuroClientFullscreen(NeuroClientPtrPtr c, const void *data);
void NeuroClientToggleFullscreen(NeuroClientPtrPtr c, const void *data);
void NeuroClientFloatMove(NeuroClientPtrPtr c, const void *data);
void NeuroClientFloatResize(NeuroClientPtrPtr c, const void *data);
void NeuroClientFreeMove(NeuroClientPtrPtr c, const void *free_setter_fn);
void NeuroClientFreeResize(NeuroClientPtrPtr c, const void *free_setter_fn);

// Find Client
NeuroClientPtrPtr NeuroClientFindWindow(Window w);
NeuroClientPtrPtr NeuroClientFindUrgent(void);
NeuroClientPtrPtr NeuroClientFindFixed(void);
NeuroClientPtrPtr NeuroClientFindPointed(const NeuroPoint *p);

// Client Getters
NeuroClientPtrPtr NeuroClientGetFocused(void);
NeuroClientPtrPtr NeuroClientGetPointedByPointer(void);

// Client Testers
bool NeuroClientTesterWindow(const NeuroClientPtrPtr c, const void *w);
bool NeuroClientTesterUrgent(const NeuroClientPtrPtr c, const void *data);
bool NeuroClientTesterFixed(const NeuroClientPtrPtr c, const void *data);
bool NeuroClientTesterPointed(const NeuroClientPtrPtr c, const void *p);
bool NeuroClientTesterHidden(const NeuroClientPtrPtr c, const void *data);

// Client Selectors
NeuroClientPtrPtr NeuroClientSelectorSelf(const NeuroClientPtrPtr c);   // Client c itself
NeuroClientPtrPtr NeuroClientSelectorNext(const NeuroClientPtrPtr c);   // The next client of c
NeuroClientPtrPtr NeuroClientSelectorPrev(const NeuroClientPtrPtr c);   // The previous client of c
NeuroClientPtrPtr NeuroClientSelectorOld(const NeuroClientPtrPtr c);    // The previous selected client
NeuroClientPtrPtr NeuroClientSelectorHead(const NeuroClientPtrPtr c);   // The first client of the stack
NeuroClientPtrPtr NeuroClientSelectorLast(const NeuroClientPtrPtr c);   // The last client of the stack
NeuroClientPtrPtr NeuroClientSelectorUpper(const NeuroClientPtrPtr c);  // The upper client (layout position) of c
NeuroClientPtrPtr NeuroClientSelectorLower(const NeuroClientPtrPtr c);  // The lower client (layout position) of c
NeuroClientPtrPtr NeuroClientSelectorLeft(const NeuroClientPtrPtr c);   // The left client (layout position) of c
NeuroClientPtrPtr NeuroClientSelectorRight(const NeuroClientPtrPtr c);  // The right client (layout position) of c

// Color Setters
NeuroColor NeuroClientColorSetterCurr(const NeuroClientPtrPtr c);
NeuroColor NeuroClientColorSetterAll(const NeuroClientPtrPtr c);
NeuroColor NeuroClientColorSetterNone(const NeuroClientPtrPtr c);

// Border Width Setters
int NeuroClientBorderWidthSetterAlways(const NeuroClientPtrPtr c);
int NeuroClientBorderWidthSetterNever(const NeuroClientPtrPtr c);
int NeuroClientBorderWidthSetterSmart(const NeuroClientPtrPtr c);
int NeuroClientBorderWidthSetterCurr(const NeuroClientPtrPtr c);

// Border Gap Setters
int NeuroClientBorderGapSetterAlways(const NeuroClientPtrPtr c);
int NeuroClientBorderGapSetterNever(const NeuroClientPtrPtr c);
int NeuroClientBorderGapSetterSmart(const NeuroClientPtrPtr c);
int NeuroClientBorderGapSetterCurr(const NeuroClientPtrPtr c);

