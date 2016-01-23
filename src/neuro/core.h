//----------------------------------------------------------------------------------------------------------------------
// Module      :  core
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

// StackSet
bool NeuroCoreInit();
void NeuroCoreStop();
size_t NeuroCoreGetHeadStack();
size_t NeuroCoreGetLastStack();
size_t NeuroCoreGetCurrStack();
size_t NeuroCoreGetPrevStack();
size_t NeuroCoreGetNextStack();
size_t NeuroCoreGetOldStack();
size_t NeuroCoreGetNspStack();
size_t NeuroCoreGetMonitorStack(const Monitor *m);
size_t NeuroCoreGetSize();
size_t NeuroCoreGetNspStackSize();
ClientPtrPtr NeuroCoreGetCurrClientNspStack();
void NeuroCoreSetCurrStack(size_t ws);
void NeuroCoreSetCurrClient(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreFindClient(const ClientTesterFn ctf, const void *data);
ClientPtrPtr NeuroCoreFindNspClient();
ClientPtrPtr NeuroCoreAddClientEnd(const Client *c);
ClientPtrPtr NeuroCoreAddClientStart(const Client *c);
Client *NeuroCoreRemoveClient(ClientPtrPtr c);
Client *NeuroCorePushMinimizedClient(Client *c);
Client *NeuroCorePopMinimizedClient(size_t ws);
Client *NeuroCoreRemoveMinimizedClient(Window w);

// Stack
bool NeuroCoreStackIsCurr(size_t ws);
bool NeuroCoreStackIsNsp(size_t ws);
bool NeuroCoreStackIsEmpty(size_t ws);
const Monitor *NeuroCoreStackGetMonitor(size_t ws);
void NeuroCoreStackSetMonitor(size_t ws, const Monitor *m);
const char *NeuroCoreStackGetName(size_t ws);
size_t NeuroCoreStackGetSize(size_t ws);
size_t NeuroCoreStackGetMinimizedNum(size_t ws);
size_t NeuroCoreStackGetNumLayouts(size_t ws);
size_t NeuroCoreStackGetLayoutIdx(size_t ws);
bool NeuroCoreStackIsCurrToggledLayout(size_t ws);
void NeuroCoreStackSetLayoutIdx(size_t ws, size_t i);
void NeuroCoreStackSetToggledLayout(size_t ws, size_t *i);
Layout *NeuroCoreStackGetLayout(size_t ws, size_t i);
const LayoutConf *NeuroCoreStackGetLayoutConf(size_t ws, size_t i);
Layout *NeuroCoreStackGetCurrLayout(size_t ws);
const LayoutConf *NeuroCoreStackGetCurrLayoutConf(size_t ws);
Rectangle *NeuroCoreStackGetRegion(size_t ws);
const int *NeuroCoreStackGetGaps(size_t ws);
ClientPtrPtr NeuroCoreStackGetCurrClient(size_t ws);
ClientPtrPtr NeuroCoreStackGetPrevClient(size_t ws);
ClientPtrPtr NeuroCoreStackGetHeadClient(size_t ws);
ClientPtrPtr NeuroCoreStackGetLastClient(size_t ws);
ClientPtrPtr NeuroCoreStackFindClient(size_t ws, const ClientTesterFn ctf, const void *p);

// Client
bool NeuroCoreClientIsCurr(const ClientPtrPtr c);
bool NeuroCoreClientIsPrev(const ClientPtrPtr c);
bool NeuroCoreClientIsHead(const ClientPtrPtr c);
bool NeuroCoreClientIsLast(const ClientPtrPtr c);
Rectangle *NeuroCoreClientGetRegion(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreClientGetNext(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreClientGetPrev(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreClientSwap(const ClientPtrPtr c1, const ClientPtrPtr c2);

