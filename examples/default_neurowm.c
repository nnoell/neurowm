//----------------------------------------------------------------------------------------------------------------------
// File        :  ~/.neurowm/neurowm.c
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include <neuro/wm.h>


//----------------------------------------------------------------------------------------------------------------------
// CONFIGURATION
//----------------------------------------------------------------------------------------------------------------------

static const NeuroConfiguration configuration_ = {
  NEURO_CONFIG_DEFAULT_INIT_ACTION_CHAIN,
  NEURO_CONFIG_DEFAULT_STOP_ACTION_CHAIN,
  NEURO_CONFIG_DEFAULT_NORMAL_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_CURRENT_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_OLD_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_FREE_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_URGENT_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_BORDER_WIDTH,
  NEURO_CONFIG_DEFAULT_BORDER_GAP,
  NeuroConfigDefaultMonitorList,
  NeuroConfigDefaultWorkspaceList,
  NEURO_CONFIG_DEFAULT_RULE_LIST,
  NeuroConfigDefaultKeyList,
  NeuroConfigDefaultButtonList
};


//----------------------------------------------------------------------------------------------------------------------
// MAIN
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  return NeuroWmRun(&configuration_);
}

