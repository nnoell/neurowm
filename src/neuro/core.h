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
size_t NeuroCoreGetMonitorStack(const NeuroMonitor *m);
size_t NeuroCoreGetSize();
size_t NeuroCoreGetNspStackSize();
NeuroClientPtrPtr NeuroCoreGetCurrClientNspStack();
void NeuroCoreSetCurrStack(size_t ws);
void NeuroCoreSetCurrClient(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreFindClient(const NeuroClientTesterFn ctf, const void *data);
NeuroClientPtrPtr NeuroCoreFindNspClient();
NeuroClientPtrPtr NeuroCoreAddClientEnd(const NeuroClient *c);
NeuroClientPtrPtr NeuroCoreAddClientStart(const NeuroClient *c);
NeuroClient *NeuroCoreRemoveClient(NeuroClientPtrPtr c);
NeuroClient *NeuroCorePushMinimizedClient(NeuroClient *c);
NeuroClient *NeuroCorePopMinimizedClient(size_t ws);
NeuroClient *NeuroCoreRemoveMinimizedClient(Window w);

// Stack
bool NeuroCoreStackIsCurr(size_t ws);
bool NeuroCoreStackIsNsp(size_t ws);
bool NeuroCoreStackIsEmpty(size_t ws);
const NeuroMonitor *NeuroCoreStackGetMonitor(size_t ws);
void NeuroCoreStackSetMonitor(size_t ws, const NeuroMonitor *m);
const char *NeuroCoreStackGetName(size_t ws);
size_t NeuroCoreStackGetSize(size_t ws);
size_t NeuroCoreStackGetMinimizedNum(size_t ws);
size_t NeuroCoreStackGetNumLayouts(size_t ws);
size_t NeuroCoreStackGetLayoutIdx(size_t ws);
bool NeuroCoreStackIsCurrToggledLayout(size_t ws);
void NeuroCoreStackSetLayoutIdx(size_t ws, size_t i);
void NeuroCoreStackSetToggledLayout(size_t ws, size_t *i);
NeuroLayout *NeuroCoreStackGetLayout(size_t ws, size_t i);
const NeuroLayoutConf *NeuroCoreStackGetLayoutConf(size_t ws, size_t i);
NeuroLayout *NeuroCoreStackGetCurrLayout(size_t ws);
const NeuroLayoutConf *NeuroCoreStackGetCurrLayoutConf(size_t ws);
NeuroRectangle *NeuroCoreStackGetRegion(size_t ws);
const int *NeuroCoreStackGetGaps(size_t ws);
NeuroClientPtrPtr NeuroCoreStackGetCurrClient(size_t ws);
NeuroClientPtrPtr NeuroCoreStackGetPrevClient(size_t ws);
NeuroClientPtrPtr NeuroCoreStackGetHeadClient(size_t ws);
NeuroClientPtrPtr NeuroCoreStackGetLastClient(size_t ws);
NeuroClientPtrPtr NeuroCoreStackFindClient(size_t ws, const NeuroClientTesterFn ctf, const void *p);

// NeuroClient
bool NeuroCoreClientIsCurr(const NeuroClientPtrPtr c);
bool NeuroCoreClientIsPrev(const NeuroClientPtrPtr c);
bool NeuroCoreClientIsHead(const NeuroClientPtrPtr c);
bool NeuroCoreClientIsLast(const NeuroClientPtrPtr c);
NeuroRectangle *NeuroCoreClientGetRegion(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreClientGetNext(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreClientGetPrev(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreClientSwap(const NeuroClientPtrPtr c1, const NeuroClientPtrPtr c2);

