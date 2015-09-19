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
#define nullModL  0
#define mirrModL  (1<<0)
#define reflXModL (1<<1)
#define reflYModL (1<<2)

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void NeuroLayoutRun(int ws, int i);
void NeuroLayoutRunCurr(int ws);
void NeuroLayoutToggleMod(int ws, int i, unsigned int mod);
void NeuroLayoutToggleModCurr(int ws, unsigned int mod);
void NeuroLayoutToggle(int ws, int l);
void NeuroLayoutChange(int ws, int s);
void NeuroLayoutReset(int ws);
void NeuroLayoutIncreaseMaster(int ws, int size);
void NeuroLayoutResizeMaster(int ws, float factor);

// Layout Arrangers
Arrange *NeuroLayoutArrangerTall(Arrange *a);
Arrange *NeuroLayoutArrangerGrid(Arrange *a);
Arrange *NeuroLayoutArrangerFull(Arrange *a);
Arrange *NeuroLayoutArrangerFloat(Arrange *a);

#endif  // NEURO_LAYOUT_H_

