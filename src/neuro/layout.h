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
void NeuroLayoutRun(size_t ws, size_t i);
void NeuroLayoutRunCurr(size_t ws);
void NeuroLayoutToggleMod(size_t ws, size_t i, NeuroLayoutMod mod);
void NeuroLayoutToggleModCurr(size_t ws, NeuroLayoutMod mod);
void NeuroLayoutToggle(size_t ws, size_t i);
void NeuroLayoutChange(size_t ws, int step);
void NeuroLayoutReset(size_t ws);
void NeuroLayoutIncreaseMaster(size_t ws, int step);
void NeuroLayoutResizeMaster(size_t ws, float factor);

// NeuroLayout Arrangers
NeuroArrange *NeuroLayoutArrangerTall(NeuroArrange *a);
NeuroArrange *NeuroLayoutArrangerGrid(NeuroArrange *a);
NeuroArrange *NeuroLayoutArrangerFull(NeuroArrange *a);
NeuroArrange *NeuroLayoutArrangerFloat(NeuroArrange *a);

