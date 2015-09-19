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

// Modmask
#define noModMask 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Default commands
extern const char* defTerminalCommand[];
extern const char* defLauncherCommand[];
extern const char* defNSPCommand[];

// Default layoutSets
extern const LayoutConf *defLayoutSet[];
extern const LayoutConf *defTogLayoutSet[];

// Default Configuration fields
extern const ActionChain defStartUpHook[];
extern const ActionChain defEndUpHook[];
extern const Button *defButtons[];
extern const Key *defKeys[];
extern const DzenPanel *defDzenPanelSet[];
extern const Rule *defRuleSet[];
extern const Workspace *defWorkspaceSet[];

// Default Configuration
extern const Configuration defConfiguration;

#endif  // NEURO_CONFIG_H_

