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
#define NeuroConfigNormalBorderColor  "#1c1c1c"
#define NeuroConfigCurrentBorderColor "#b5b3b3"
#define NeuroConfigOldBorderColor     "#444444"
#define NeuroConfigFreeBorderColor    "#f7a16e"
#define NeuroConfigUrgentBorderColor  "#66ff66"

// Borders
#define NeuroConfigBorderWidth 2
#define NeuroConfigBorderGap 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Default Commands
extern const char* NeuroConfigTerminalCommand[];
extern const char* NeuroConfigLauncherCommand[];
extern const char* NeuroConfigScratchpadCommand[];

// Default Layout Sets
extern const LayoutConf* NeuroConfigLayoutSet[];
extern const LayoutConf* NeuroConfigToggledLayoutSet[];

// Default Configuration Fields
extern const ActionChain NeuroConfigStartupHook[];
extern const ActionChain NeuroConfigEndupHook[];
extern const Button* NeuroConfigButtons[];
extern const Key* NeuroConfigKeys[];
extern const DzenPanel* NeuroConfigDzenPanelSet[];
extern const Rule* NeuroConfigRuleSet[];
extern const Workspace* NeuroConfigWorkspaceSet[];

// Default Configuration
extern const Configuration NeuroConfigConfiguration;

#endif  // NEURO_CONFIG_H_

