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

static const Configuration configuration_ = {
  NeuroConfigDefaultInitActionChain,
  NeuroConfigDefaultStopActionChain,
  NeuroConfigDefaultNormalBorderColor,
  NeuroConfigDefaultCurrentBorderColor,
  NeuroConfigDefaultOldBorderColor,
  NeuroConfigDefaultFreeBorderColor,
  NeuroConfigDefaultUrgentBorderColor,
  NeuroConfigDefaultBorderWidth,
  NeuroConfigDefaultBorderGap,
  NeuroConfigDefaultWorkspaceList,
  NeuroConfigDefaultRuleList,
  NeuroConfigDefaultDzenPanelList,
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

