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

#pragma once

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void NeuroLayoutRun(NeuroIndex ws, NeuroIndex i);
void NeuroLayoutRunCurr(NeuroIndex ws);
void NeuroLayoutToggleMod(NeuroIndex ws, NeuroIndex i, NeuroLayoutMod mod);
void NeuroLayoutToggleModCurr(NeuroIndex ws, NeuroLayoutMod mod);
void NeuroLayoutToggle(NeuroIndex ws, NeuroIndex i);
void NeuroLayoutChange(NeuroIndex ws, int step);
void NeuroLayoutReset(NeuroIndex ws);
void NeuroLayoutIncreaseMaster(NeuroIndex ws, int step);
void NeuroLayoutResizeMaster(NeuroIndex ws, float factor);

// NeuroLayout Arrangers
NeuroArrange *NeuroLayoutArrangerTall(NeuroArrange *a);
NeuroArrange *NeuroLayoutArrangerGrid(NeuroArrange *a);
NeuroArrange *NeuroLayoutArrangerFull(NeuroArrange *a);
NeuroArrange *NeuroLayoutArrangerFloat(NeuroArrange *a);

