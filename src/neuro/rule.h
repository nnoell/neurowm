//----------------------------------------------------------------------------------------------------------------------
// Module      :  rule
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

// Defines
#define NEURO_RULE_SCRATCHPAD_NAME "neurowm_scratchpad"
#define NeuroRuleFreeSetterNull NULL


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
NeuroClient *NeuroRuleNewClient(Window w, const XWindowAttributes *wa);
void NeuroRuleSetLayoutRegion(NeuroRectangle *r, const NeuroClientPtrPtr c);
void NeuroRuleSetClientRegion(NeuroRectangle *r, const NeuroClientPtrPtr c);

// Free Setters
void NeuroRuleFreeSetterDefault(NeuroRectangle *a, const NeuroRectangle *r);
void NeuroRuleFreeSetterCenter(NeuroRectangle *a, const NeuroRectangle *r);
void NeuroRuleFreeSetterBigCenter(NeuroRectangle *a, const NeuroRectangle *r);
void NeuroRuleFreeSetterScratchpad(NeuroRectangle *a, const NeuroRectangle *r);

