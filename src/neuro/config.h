//----------------------------------------------------------------------------------------------------------------------
// Module      :  config
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_CONFIG_H_
#define NEURO_CONFIG_H_

// Includes
#include "type.h"

// Colors
#define defNormBorderColor "#1c1c1c"
#define defCurrBorderColor "#b5b3b3"
#define defPrevBorderColor "#444444"
#define defFreeBorderColor "#f7a16e"
#define defUrgtBorderColor "#66ff66"

// Borders
#define defBorderWidth 2
#define defBorderGap   0

// Arrange Settings
#define defTallMasterNum  {.i = 1}
#define defTallMasterSize {.f = 0.5f}
#define defTallStepSize   {.f = 0.03f}

// Modmask
#define noModMask 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Default layoutSets
extern const LayoutConf *defLayoutSet[];
extern const LayoutConf *defTogLayoutSet[];

// Default WMConfig fields fields
extern const Action *defStartUpHook[];
extern const Action *defEndUpHook[];
extern const Button *defButtons[];
extern const Key *defKeys[];
extern const DzenPanel *defDzenPanelSet[];
extern const Rule *defRuleSet[];
extern const Workspace *defWorkspaceSet[];

// DefaultWMConfig
extern const WMConfig defWMConfig;


#endif  // NEURO_CONFIG_H_

