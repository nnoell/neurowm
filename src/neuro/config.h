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

#pragma once

// Includes
#include "type.h"

// Defines
#define NeuroConfigDefaultInitActionChain CHAIN_NULL(NeuroActionListNothing)
#define NeuroConfigDefaultStopActionChain CHAIN_NULL(NeuroActionListNothing)
#define NeuroConfigDefaultNormalBorderColor "#1c1c1c"
#define NeuroConfigDefaultCurrentBorderColor "#b5b3b3"
#define NeuroConfigDefaultOldBorderColor "#444444"
#define NeuroConfigDefaultFreeBorderColor "#f7a16e"
#define NeuroConfigDefaultUrgentBorderColor "#66ff66"
#define NeuroConfigDefaultBorderWidth 1
#define NeuroConfigDefaultBorderGap 0
#define NeuroConfigDefaultRuleList NULL


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Commands
extern const char* NeuroConfigDefaultTerminalCommand[];
extern const char* NeuroConfigDefaultLauncherCommand[];
extern const char* NeuroConfigDefaultScratchpadCommand[];

// NeuroConfiguration fields
extern const NeuroMonitorConf* NeuroConfigDefaultMonitorList[];
extern const NeuroLayoutConf* NeuroConfigDefaultLayoutList[];
extern const NeuroLayoutConf* NeuroConfigDefaultToggledLayoutList[];
extern const NeuroWorkspace* NeuroConfigDefaultWorkspaceList[];
extern const NeuroKey* NeuroConfigDefaultKeyList[];
extern const NeuroButton* NeuroConfigDefaultButtonList[];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// NeuroConfiguration functions
void NeuroConfigSet(const NeuroConfiguration *c);
const NeuroConfiguration *NeuroConfigGet();

