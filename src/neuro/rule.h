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

#ifndef NEURO_RULE_H_
#define NEURO_RULE_H_

// Includes
#include "type.h"

// Defines
#define NeuroRuleFreeSetterNull NULL
#define NeuroRuleFixedPositionNull  0
#define NeuroRuleFixedPositionUp    1
#define NeuroRuleFixedPositionDown  2
#define NeuroRuleFixedPositionLeft  3
#define NeuroRuleFixedPositionRigth 4


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
Client *NeuroRuleAllocClient(Window w, const XWindowAttributes *wa);
void NeuroRuleApply(const ClientPtrPtr c);
void NeuroRuleUnapply(const ClientPtrPtr c);

// Free Setters
void NeuroRuleFreeSetterDefault(Rectangle *a, const Rectangle *r);
void NeuroRuleFreeSetterCenter(Rectangle *a, const Rectangle *r);
void NeuroRuleFreeSetterBigCenter(Rectangle *a, const Rectangle *r);
void NeuroRuleFreeSetterScratchpad(Rectangle *a, const Rectangle *r);


#endif  // NEURO_RULE_H_

