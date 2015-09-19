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

#ifndef NEURO_CLIENT_H_
#define NEURO_CLIENT_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
void NeuroClientUpdate(ClientPtrPtr c, const void *data);
void NeuroClientUpdateClassAndName(ClientPtrPtr c, const void *data);
void NeuroClientUpdateTitle(ClientPtrPtr c, const void *data);
void NeuroClientHide(ClientPtrPtr c, const void *doRules);
void NeuroClientShow(ClientPtrPtr c, const void *doRules);
void NeuroClientSetUrgent(ClientPtrPtr c, const void *data);
void NeuroClientUnsetUrgent(ClientPtrPtr c, const void *data);
void NeuroClientKill(ClientPtrPtr c, const void *data);
void NeuroClientMinimize(ClientPtrPtr c, const void *data);
void NeuroClientTile(ClientPtrPtr c, const void *data);
void NeuroClientFree(ClientPtrPtr c, const void *freeSetterFn);
void NeuroClientToggleFree(ClientPtrPtr c, const void *freeSetterFn);
void NeuroClientNormal(ClientPtrPtr c, const void *data);
void NeuroClientFullscreen(ClientPtrPtr c, const void *data);
void NeuroClientToggleFullscreen(ClientPtrPtr c, const void *data);
void NeuroClientFloatMove(ClientPtrPtr c, const void *data);
void NeuroClientFloatResize(ClientPtrPtr c, const void *data);
void NeuroClientFreeMove(ClientPtrPtr c, const void *freeSetterFn);
void NeuroClientFreeResize(ClientPtrPtr c, const void *freeSetterFn);

// Client getters
ClientPtrPtr NeuroClientGetFocused();
ClientPtrPtr NeuroClientGetPointed(int *x, int *y);

// Client Selectors
ClientPtrPtr NeuroClientSelectorSelf(const ClientPtrPtr c);   // Client c itself
ClientPtrPtr NeuroClientSelectorNext(const ClientPtrPtr c);   // The next client of c
ClientPtrPtr NeuroClientSelectorPrev(const ClientPtrPtr c);   // The previous client of c
ClientPtrPtr NeuroClientSelectorOld(const ClientPtrPtr c);    // The previous selected client
ClientPtrPtr NeuroClientSelectorHead(const ClientPtrPtr c);   // The first client of the stack
ClientPtrPtr NeuroClientSelectorLast(const ClientPtrPtr c);   // The last client of the stack
ClientPtrPtr NeuroClientSelectorUp(const ClientPtrPtr c);     // The upper client (layout position) of c
ClientPtrPtr NeuroClientSelectorDown(const ClientPtrPtr c);   // The lower client (layout position) of c
ClientPtrPtr NeuroClientSelectorLeft(const ClientPtrPtr c);   // The left client (layout position) of c
ClientPtrPtr NeuroClientSelectorRight(const ClientPtrPtr c);  // The right client (layout position) of c

// Client Testers
Bool NeuroClientTesterWindow(const ClientPtrPtr c, const void *w);
Bool NeuroClientTesterUrgent(const ClientPtrPtr c, const void *data);
Bool NeuroClientTesterFixed(const ClientPtrPtr c, const void *data);

// Color Setters
Color NeuroClientColorSetterCurr(const ClientPtrPtr c);
Color NeuroClientColorSetterAll(const ClientPtrPtr c);
Color NeuroClientColorSetterNone(const ClientPtrPtr c);

// Border Width Setters
int NeuroClientBorderWidthSetterAlways(const ClientPtrPtr c);
int NeuroClientBorderWidthSetterNever(const ClientPtrPtr c);
int NeuroClientBorderWidthSetterSmart(const ClientPtrPtr c);
int NeuroClientBorderWidthSetterCurr(const ClientPtrPtr c);

// Border Gap Setters
int NeuroClientBorderGapSetterAlways(const ClientPtrPtr c);
int NeuroClientBorderGapSetterNever(const ClientPtrPtr c);
int NeuroClientBorderGapSetterSmart(const ClientPtrPtr c);
int NeuroClientBorderGapSetterCurr(const ClientPtrPtr c);

#endif  // NEURO_CLIENT_H_

