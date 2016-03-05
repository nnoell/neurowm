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
bool NeuroCoreInit(void);
void NeuroCoreStop(void);
NeuroIndex NeuroCoreGetHeadStack(void);
NeuroIndex NeuroCoreGetLastStack(void);
NeuroIndex NeuroCoreGetCurrStack(void);
NeuroIndex NeuroCoreGetPrevStack(void);
NeuroIndex NeuroCoreGetNextStack(void);
NeuroIndex NeuroCoreGetOldStack(void);
NeuroIndex NeuroCoreGetNspStack(void);
NeuroIndex NeuroCoreGetMonitorStack(const NeuroMonitor *m);
NeuroIndex NeuroCoreGetSize(void);
NeuroIndex NeuroCoreGetNspStackSize(void);
NeuroClientPtrPtr NeuroCoreGetCurrClientNspStack(void);
void NeuroCoreSetCurrStack(NeuroIndex ws);
void NeuroCoreSetCurrClient(NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreFindClient(const NeuroClientTesterFn ctf, const void *data);
NeuroClientPtrPtr NeuroCoreFindNspClient(void);
NeuroClientPtrPtr NeuroCoreAddClientEnd(NeuroClient *c);
NeuroClientPtrPtr NeuroCoreAddClientStart(NeuroClient *c);
NeuroClient *NeuroCoreRemoveClient(NeuroClientPtrPtr c);
NeuroClient *NeuroCorePushMinimizedClient(NeuroClient *c);
NeuroClient *NeuroCorePopMinimizedClient(NeuroIndex ws);
NeuroClient *NeuroCoreRemoveMinimizedClient(Window w);

// Stack
bool NeuroCoreStackIsCurr(NeuroIndex ws);
bool NeuroCoreStackIsNsp(NeuroIndex ws);
bool NeuroCoreStackIsEmpty(NeuroIndex ws);
const NeuroMonitor *NeuroCoreStackGetMonitor(NeuroIndex ws);
void NeuroCoreStackSetMonitor(NeuroIndex ws, const NeuroMonitor *m);
const char *NeuroCoreStackGetName(NeuroIndex ws);
NeuroIndex NeuroCoreStackGetSize(NeuroIndex ws);
NeuroIndex NeuroCoreStackGetMinimizedNum(NeuroIndex ws);
NeuroIndex NeuroCoreStackGetNumLayouts(NeuroIndex ws);
NeuroIndex NeuroCoreStackGetLayoutIdx(NeuroIndex ws);
bool NeuroCoreStackIsCurrToggledLayout(NeuroIndex ws);
void NeuroCoreStackSetLayoutIdx(NeuroIndex ws, NeuroIndex i);
void NeuroCoreStackSetToggledLayout(NeuroIndex ws, NeuroIndex *i);
NeuroLayout *NeuroCoreStackGetLayout(NeuroIndex ws, NeuroIndex i);
const NeuroLayoutConf *NeuroCoreStackGetLayoutConf(NeuroIndex ws, NeuroIndex i);
NeuroLayout *NeuroCoreStackGetCurrLayout(NeuroIndex ws);
const NeuroLayoutConf *NeuroCoreStackGetCurrLayoutConf(NeuroIndex ws);
NeuroRectangle *NeuroCoreStackGetRegion(NeuroIndex ws);
const int *NeuroCoreStackGetGaps(NeuroIndex ws);
NeuroClientPtrPtr NeuroCoreStackGetCurrClient(NeuroIndex ws);
NeuroClientPtrPtr NeuroCoreStackGetPrevClient(NeuroIndex ws);
NeuroClientPtrPtr NeuroCoreStackGetHeadClient(NeuroIndex ws);
NeuroClientPtrPtr NeuroCoreStackGetLastClient(NeuroIndex ws);
NeuroClientPtrPtr NeuroCoreStackFindClient(NeuroIndex ws, const NeuroClientTesterFn ctf, const void *p);

// Client
bool NeuroCoreClientIsCurr(const NeuroClientPtrPtr c);
bool NeuroCoreClientIsPrev(const NeuroClientPtrPtr c);
bool NeuroCoreClientIsHead(const NeuroClientPtrPtr c);
bool NeuroCoreClientIsLast(const NeuroClientPtrPtr c);
NeuroRectangle *NeuroCoreClientGetRegion(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreClientGetNext(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreClientGetPrev(const NeuroClientPtrPtr c);
NeuroClientPtrPtr NeuroCoreClientSwap(const NeuroClientPtrPtr c1, const NeuroClientPtrPtr c2);

