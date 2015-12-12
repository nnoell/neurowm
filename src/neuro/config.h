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

// Defines
#define NeuroConfigDefaultInitActionChain CHAIN_NULL(NeuroActionSetNothing)
#define NeuroConfigDefaultStopActionChain CHAIN_NULL(NeuroActionSetNothing)
#define NeuroConfigDefaultNormalBorderColor "#1c1c1c"
#define NeuroConfigDefaultCurrentBorderColor "#b5b3b3"
#define NeuroConfigDefaultOldBorderColor "#444444"
#define NeuroConfigDefaultFreeBorderColor "#f7a16e"
#define NeuroConfigDefaultUrgentBorderColor "#66ff66"
#define NeuroConfigDefaultBorderWidth 1
#define NeuroConfigDefaultBorderGap 0
#define NeuroConfigDefaultRuleSet NULL
#define NeuroConfigDefaultDzenPanelSet NULL


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Commands
extern const char* NeuroConfigDefaultTerminalCommand[];
extern const char* NeuroConfigDefaultLauncherCommand[];
extern const char* NeuroConfigDefaultScratchpadCommand[];

// Configuration fields
extern const LayoutConf* NeuroConfigDefaultLayoutSet[];
extern const LayoutConf* NeuroConfigDefaultToggledLayoutSet[];
extern const Workspace* NeuroConfigDefaultWorkspaceSet[];
extern const Key* NeuroConfigDefaultKeySet[];
extern const Button* NeuroConfigDefaultButtonSet[];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Configuration functions
void NeuroConfigSet(const Configuration *c);
const Configuration *NeuroConfigGet();

#endif  // NEURO_CONFIG_H_

