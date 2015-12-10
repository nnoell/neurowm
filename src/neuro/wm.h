//----------------------------------------------------------------------------------------------------------------------
// Module      :  wm
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_WM_H_
#define NEURO_WM_H_

// Includes
#include "config.h"
#include "layout.h"
#include "rule.h"
#include "client.h"
#include "workspace.h"
#include "action.h"
#include "dzen.h"
#include "theme.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

int NeuroWmRun(const Configuration *c);
void NeuroWmQuit();
void NeuroWmReload();

#endif  // NEURO_WM_H_

