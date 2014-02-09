//----------------------------------------------------------------------------------------------------------------------
// Module      :  layout
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_LAYOUT_H_
#define NEURO_LAYOUT_H_

// Includes
#include "general.h"

// Defines
#define notModL      0
#define mirrModL     (1<<0)
#define reflXModL    (1<<1)
#define reflYModL    (1<<2)

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void runLayoutL(int ws, int i);
void runCurrLayoutL(int ws);
void togModLayoutL(int ws, int i, unsigned int mod);
void togModCurrLayoutL(int ws, unsigned int mod);
void togLayoutL(int ws, int l);
void changeLayoutL(int ws, int s);
void resetLayoutL(int ws);

void increaseMasterL(int ws, int s);
void resizeMasterL(int ws, int r);

// Layouts
Arrange *tallArrL(Arrange *a);
Arrange *gridArrL(Arrange *a);
Arrange *fullArrL(Arrange *a);
Arrange *floatArrL(Arrange *a);

#endif  // NEURO_LAYOUT_H_

