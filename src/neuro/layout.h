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
#include "type.h"

// Defines
#define notModL      0
#define mirrModL     (1<<0)
#define reflXModL    (1<<1)
#define reflYModL    (1<<2)

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void runL(int ws, int i);
void runCurrL(int ws);
void toggleModL(int ws, int i, unsigned int mod);
void toggleModCurrL(int ws, unsigned int mod);
void toggleL(int ws, int l);
void changeL(int ws, int s);
void resetL(int ws);

void increaseMasterL(int ws, int size);
void resizeMasterL(int ws, float factor);

// Layout Arrangers
Arrange *tallArrangerL(Arrange *a);
Arrange *gridArrangerL(Arrange *a);
Arrange *fullArrangerL(Arrange *a);
Arrange *floatArrangerL(Arrange *a);

#endif  // NEURO_LAYOUT_H_

