//----------------------------------------------------------------------------------------------------------------------
// Module      :  neurowm
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_NEUROWM_H_
#define NEURO_NEUROWM_H_

// Includes
#include "config.h"
#include "layout.h"
#include "rule.h"
#include "client.h"
#include "workspace.h"
#include "action.h"
#include "personal.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

int runNeurowmN(const WMConfig *c);
void quitNeurowmN();
void reloadNeurowmN();

#endif  // NEURO_NEUROWM_H_

