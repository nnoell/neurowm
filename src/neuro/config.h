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
#define NEURO_CONFIG_DEFAULT_INIT_ACTION_CHAIN CHAIN_NULL(NeuroActionListNothing)
#define NEURO_CONFIG_DEFAULT_STOP_ACTION_CHAIN CHAIN_NULL(NeuroActionListNothing)
#define NEURO_CONFIG_DEFAULT_NORMAL_BORDER_COLOR "#1c1c1c"
#define NEURO_CONFIG_DEFAULT_CURRENT_BORDER_COLOR "#b5b3b3"
#define NEURO_CONFIG_DEFAULT_OLD_BORDER_COLOR "#444444"
#define NEURO_CONFIG_DEFAULT_FREE_BORDER_COLOR "#f7a16e"
#define NEURO_CONFIG_DEFAULT_URGENT_BORDER_COLOR "#66ff66"
#define NEURO_CONFIG_DEFAULT_BORDER_WIDTH 1
#define NEURO_CONFIG_DEFAULT_BORDER_GAP 0
#define NEURO_CONFIG_DEFAULT_RULE_LIST NULL


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

