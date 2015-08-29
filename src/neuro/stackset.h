//----------------------------------------------------------------------------------------------------------------------
// Module      :  stackset
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_STACKSET_H_
#define NEURO_STACKSET_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// StackSet
Bool initSS();
void endSS();
int getCurrStackSS();
int getPrevStackSS();
int getNextStackSS();
int getOldStackSS();
int getNSPStackSS();
int getSizeSS();
int getSizeNSPSS();
ClientPtrPtr getCurrClientCurrStackSS();
ClientPtrPtr getCurrClientNSPStackSS();
void setCurrStackSS(int ws);
void setCurrClientSS(const ClientPtrPtr c);
ClientPtrPtr findClientSS(const ClientTesterFn ctf, const void *data);
ClientPtrPtr findNSPClientSS();
ClientPtrPtr addClientEndSS(const Client *c);
ClientPtrPtr addClientStartSS(const Client *c);
Client *rmvClientSS(ClientPtrPtr c);
Client *pushMinimizedClientSS(Client *c);
Client *popMinimizedClientSS(int ws);
Client *rmvMinimizedClientSS(Window w);

// Stack
Bool isCurrStackSS(int ws);
Bool isNSPStackSS(int ws);
Bool isEmptyStackSS(int ws);
const char *getNameStackSS(int ws);
int getSizeStackSS(int ws);
int getMinimizedNumStackSS(int ws);
int getNumLayoutStackSS(int ws);
int getLayoutIdxStackSS(int ws);
Bool isCurrTogLayoutStackSS(int ws);
void setLayoutStackSS(int ws, int i);
void setTogLayoutStackSS(int ws, int i);
Layout *getLayoutStackSS(int ws, int i);
const LayoutConf *getLayoutConfStackSS(int ws, int i);
Layout *getCurrLayoutStackSS(int ws);
const LayoutConf *getCurrLayoutConfStackSS(int ws);
Rectangle *getRegionStackSS(int ws);
ClientPtrPtr getCurrClientStackSS(int ws);
ClientPtrPtr getPrevClientStackSS(int ws);
ClientPtrPtr getHeadClientStackSS(int ws);
ClientPtrPtr getLastClientStackSS(int ws);
ClientPtrPtr findClientStackSS(int ws, const ClientTesterFn ctf, const void *p);

// ClientPtrPtr
Bool isCurrClientSS(const ClientPtrPtr c);
Bool isPrevClientSS(const ClientPtrPtr c);
Bool isHeadClientSS(const ClientPtrPtr c);
Bool isLastClientSS(const ClientPtrPtr c);
Rectangle *getRegionClientSS(const ClientPtrPtr c);
ClientPtrPtr getNextClientSS(const ClientPtrPtr c);
ClientPtrPtr getPrevClientSS(const ClientPtrPtr c);
ClientPtrPtr swpClientSS(const ClientPtrPtr c1, const ClientPtrPtr c2);


#endif  // NEURO_STACKSET_H_

