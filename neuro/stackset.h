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
#include "general.h"


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
CliPtr getCurrCliCurrStackSS();
CliPtr getCurrCliNSPStackSS();
void setCurrStackSS(int ws);
void setCurrCliSS(const CliPtr c);
CliPtr findCliSS(const TestCliPtrFunc tcfn, const void *p);
CliPtr findNSPCliSS();
CliPtr addCliEndSS(const Client *c);
CliPtr addCliStartSS(const Client *c);
Client *rmvCliSS(CliPtr c);
Client *pushMinimizedCliSS(Client *c);
Client *popMinimizedCliSS(int ws);
Client *rmvMinimizedCliSS(Window w);

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
Area *getRegionStackSS(int ws);
CliPtr getCurrCliStackSS(int ws);
CliPtr getPrevCliStackSS(int ws);
CliPtr getHeadCliStackSS(int ws);
CliPtr getLastCliStackSS(int ws);
CliPtr findCliStackSS(int ws, const TestCliPtrFunc tcfn, const void *p);

// CliPtr
Bool isCurrCliSS(const CliPtr c);
Bool isPrevCliSS(const CliPtr c);
Bool isHeadCliSS(const CliPtr c);
Bool isLastCliSS(const CliPtr c);
Area *getRegionCliSS(const CliPtr c);
CliPtr getNextCliSS(const CliPtr c);
CliPtr getPrevCliSS(const CliPtr c);
CliPtr swpCliSS(const CliPtr c1, const CliPtr c2);


#endif  // NEURO_STACKSET_H_

