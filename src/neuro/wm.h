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

#pragma once

// Includes
#include "config.h"
#include "layout.h"
#include "rule.h"
#include "client.h"
#include "workspace.h"
#include "action.h"
#include "dzen.h"
#include "theme.h"
#include "monitor.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

int NeuroWmRun(const Configuration *c);
void NeuroWmQuit();
void NeuroWmReload();

