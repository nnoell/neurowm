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
int getLastStackSS();
int getNSPStackSS();
int getSizeSS();
int getSizeNSPSS();
CliPtr getCurrClientCurrStackSS();
CliPtr getCurrClientNSPStackSS();
void setCurrStackSS(int ws);
void setCurrClientSS(const CliPtr c);
CliPtr findClientSS(const TestCliPtrFn tcfn, const void *data);
CliPtr findNSPClientSS();
CliPtr addClientEndSS(const Client *c);
CliPtr addClientStartSS(const Client *c);
Client *rmvClientSS(CliPtr c);
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
CliPtr getCurrClientStackSS(int ws);
CliPtr getPrevClientStackSS(int ws);
CliPtr getHeadClientStackSS(int ws);
CliPtr getLastClientStackSS(int ws);
CliPtr findClientStackSS(int ws, const TestCliPtrFn tcfn, const void *p);

// CliPtr
Bool isCurrClientSS(const CliPtr c);
Bool isPrevClientSS(const CliPtr c);
Bool isHeadClientSS(const CliPtr c);
Bool isLastClientSS(const CliPtr c);
Rectangle *getRegionClientSS(const CliPtr c);
CliPtr getNextClientSS(const CliPtr c);
CliPtr getPrevClientSS(const CliPtr c);
CliPtr swpClientSS(const CliPtr c1, const CliPtr c2);


#endif  // NEURO_STACKSET_H_

