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

#ifndef NEURO_CORE_H_
#define NEURO_CORE_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// StackSet
Bool NeuroCoreInit();
void NeuroCoreStop();
int NeuroCoreGetCurrStack();
int NeuroCoreGetPrevStack();
int NeuroCoreGetNextStack();
int NeuroCoreGetOldStack();
int NeuroCoreGetNspStack();
int NeuroCoreGetSize();
int NeuroCoreGetNspStackSize();
ClientPtrPtr NeuroCoreGetCurrClientNspStack();
void NeuroCoreSetCurrStack(int ws);
void NeuroCoreSetCurrClient(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreFindClient(const ClientTesterFn ctf, const void *data);
ClientPtrPtr NeuroCoreFindNspClient();
ClientPtrPtr NeuroCoreAddClientEnd(const Client *c);
ClientPtrPtr NeuroCoreAddClientStart(const Client *c);
Client *NeuroCoreRemoveClient(ClientPtrPtr c);
Client *NeuroCorePushMinimizedClient(Client *c);
Client *NeuroCorePopMinimizedClient(int ws);
Client *NeuroCoreRemoveMinimizedClient(Window w);

// Stack
Bool NeuroCoreStackIsCurr(int ws);
Bool NeuroCoreStackIsNsp(int ws);
Bool NeuroCoreStackIsEmpty(int ws);
const char *NeuroCoreStackGetName(int ws);
int NeuroCoreStackGetSize(int ws);
int NeuroCoreStackGetMinimizedNum(int ws);
int NeuroCoreStackGetNumLayouts(int ws);
int NeuroCoreStackGetLayoutIdx(int ws);
Bool NeuroCoreStackIsCurrToggledLayout(int ws);
void NeuroCoreStackSetLayoutIdx(int ws, int i);
void NeuroCoreStackSetToggledLayout(int ws, int i);
Layout *NeuroCoreStackGetLayout(int ws, int i);
const LayoutConf *NeuroCoreStackGetLayoutConf(int ws, int i);
Layout *NeuroCoreStackGetCurrLayout(int ws);
const LayoutConf *NeuroCoreStackGetCurrLayoutConf(int ws);
Rectangle *NeuroCoreStackGetRegion(int ws);
ClientPtrPtr NeuroCoreStackGetCurrClient(int ws);
ClientPtrPtr NeuroCoreStackGetPrevClient(int ws);
ClientPtrPtr NeuroCoreStackGetHeadClient(int ws);
ClientPtrPtr NeuroCoreStackGetLastClient(int ws);
ClientPtrPtr NeuroCoreStackFindClient(int ws, const ClientTesterFn ctf, const void *p);

// Client
Bool NeuroCoreClientIsCurr(const ClientPtrPtr c);
Bool NeuroCoreClientIsPrev(const ClientPtrPtr c);
Bool NeuroCoreClientIsHead(const ClientPtrPtr c);
Bool NeuroCoreClientIsLast(const ClientPtrPtr c);
Rectangle *NeuroCoreClientGetRegion(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreClientGetNext(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreClientGetPrev(const ClientPtrPtr c);
ClientPtrPtr NeuroCoreClientSwap(const ClientPtrPtr c1, const ClientPtrPtr c2);


#endif  // NEURO_CORE_H_

