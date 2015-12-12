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

// Includes
#include "wm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Layout
static const LayoutConf tile_ = {
  "Tile", NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, LayoutModNull, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
};
static const LayoutConf mirror_ = {
  "Mirr", NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, LayoutModMirror, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
};
static const LayoutConf grid_ = {
  "Grid", NeuroLayoutArrangerGrid,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, LayoutModNull, True, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
};
static const LayoutConf full_ = {
  "Full", NeuroLayoutArrangerFull,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, LayoutModNull, True, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
};
static const LayoutConf float_ = {
  "Float", NeuroLayoutArrangerFloat,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterAlways, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, LayoutModNull, False, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
};

// Workspace
static const Workspace ws0_ = {
    "ws1", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws1_ = {
    "ws2", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws2_ = {
    "ws3", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws3_ = {
    "ws4", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws4_ = {
    "ws5", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws5_ = {
    "ws6", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws6_ = {
    "ws7", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws7_ = {
    "ws8", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws8_ = {
    "ws9", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };
static const Workspace ws9_ = {
    "ws0", {0, 0, 0, 0},  NeuroConfigDefaultLayoutSet, NeuroConfigDefaultToggledLayoutSet };

// Key
static const Key key00_ = {
    Mod1Mask|ShiftMask,   XK_Return, CHAIN(NeuroActionSetSpawn, ARG_CMD(NeuroConfigDefaultTerminalCommand))      };
static const Key key01_ = {
    Mod1Mask,             XK_c,      CHAIN_NULL(NeuroActionSetKillClient)                                        };
static const Key key02_ = {
    Mod1Mask,             XK_j,      CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorNext))      };
static const Key key03_ = {
    Mod1Mask,             XK_k,      CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorPrev))      };
static const Key key04_ = {
    Mod1Mask,             XK_Tab,    CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorOld))       };
static const Key key05_ = {
    Mod1Mask,             XK_m,      CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorHead))      };
static const Key key06_ = {
    Mod1Mask|ShiftMask,   XK_j,      CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorNext))       };
static const Key key07_ = {
    Mod1Mask|ShiftMask,   XK_k,      CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorPrev))       };
static const Key key08_ = {
    Mod1Mask|ShiftMask,   XK_p,      CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorOld))        };
static const Key key09_ = {
    Mod1Mask|ShiftMask,   XK_m,      CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorHead))       };
static const Key key10_ = {
    Mod1Mask,             XK_Up,     CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorUp))        };
static const Key key11_ = {
    Mod1Mask,             XK_Down,   CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorDown))      };
static const Key key12_ = {
    Mod1Mask,             XK_Left,   CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorLeft))      };
static const Key key13_ = {
    Mod1Mask,             XK_Right,  CHAIN(NeuroActionSetFocusCurrClient, ARG_CSF(NeuroClientSelectorRight))     };
static const Key key14_ = {
    Mod1Mask|ShiftMask,   XK_Up,     CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorUp))         };
static const Key key15_ = {
    Mod1Mask|ShiftMask,   XK_Down,   CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorDown))       };
static const Key key16_ = {
    Mod1Mask|ShiftMask,   XK_Left,   CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorLeft))       };
static const Key key17_ = {
    Mod1Mask|ShiftMask,   XK_Right,  CHAIN(NeuroActionSetSwapCurrClient, ARG_CSF(NeuroClientSelectorRight))      };
static const Key key18_ = {
    Mod1Mask,             XK_space,  CHAIN_NULL(NeuroActionSetChangeLayout)                                      };
static const Key key19_ = {
    Mod1Mask,             XK_t,      CHAIN_NULL(NeuroActionSetToggleFreeCurrClient)                              };
static const Key key20_ = {
    Mod1Mask,             XK_z,      CHAIN(NeuroActionSetToggleModLayout, ARG_LMOD(LayoutModMirror))             };
static const Key key21_ = {
    Mod1Mask,             XK_x,      CHAIN(NeuroActionSetToggleModLayout, ARG_LMOD(LayoutModReflectX))           };
static const Key key22_ = {
    Mod1Mask,             XK_y,      CHAIN(NeuroActionSetToggleModLayout, ARG_LMOD(LayoutModReflectY))           };
static const Key key23_ = {
    Mod1Mask,             XK_f,      CHAIN(NeuroActionSetToggleLayout, ARG_INT(0))                               };
static const Key key24_ = {
    Mod1Mask,             XK_o,      CHAIN(NeuroActionSetToggleLayout, ARG_INT(1))                               };
static const Key key25_ = {
    Mod1Mask|ShiftMask,   XK_f,      CHAIN_NULL(NeuroActionSetToggleFullscreenCurrClient)                        };
static const Key key26_ = {
    Mod1Mask,             XK_comma,  CHAIN_NULL(NeuroActionSetIncreaseMasterLayout)                              };
static const Key key27_ = {
    Mod1Mask,             XK_period, CHAIN(NeuroActionSetIncreaseMasterLayout, ARG_INT(-1))                      };
static const Key key28_ = {
    Mod1Mask,             XK_l,      CHAIN_NULL(NeuroActionSetResizeMasterLayout)                                };
static const Key key29_ = {
    Mod1Mask,             XK_h,      CHAIN(NeuroActionSetResizeMasterLayout, ARG_FLOAT(-1.0f))                   };
static const Key key30_ = {
    Mod1Mask,             XK_1,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector0))      };
static const Key key31_ = {
    Mod1Mask,             XK_2,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector1))      };
static const Key key32_ = {
    Mod1Mask,             XK_3,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector2))      };
static const Key key33_ = {
    Mod1Mask,             XK_4,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector3))      };
static const Key key34_ = {
    Mod1Mask,             XK_5,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector4))      };
static const Key key35_ = {
    Mod1Mask,             XK_6,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector5))      };
static const Key key36_ = {
    Mod1Mask,             XK_7,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector6))      };
static const Key key37_ = {
    Mod1Mask,             XK_8,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector7))      };
static const Key key38_ = {
    Mod1Mask,             XK_9,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector8))      };
static const Key key39_ = {
    Mod1Mask,             XK_0,      CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector9))      };
static const Key key40_ = {
    ControlMask|Mod1Mask, XK_Left,   CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorPrev))   };
static const Key key41_ = {
    ControlMask|Mod1Mask, XK_Right,  CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorNext))   };
static const Key key42_ = {
    Mod1Mask|ShiftMask,   XK_Tab,    CHAIN(NeuroActionSetChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorOld))    };
static const Key key43_ = {
    Mod1Mask|ShiftMask,   XK_1,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector0))       };
static const Key key44_ = {
    Mod1Mask|ShiftMask,   XK_2,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector1))       };
static const Key key45_ = {
    Mod1Mask|ShiftMask,   XK_3,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector2))       };
static const Key key46_ = {
    Mod1Mask|ShiftMask,   XK_4,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector3))       };
static const Key key47_ = {
    Mod1Mask|ShiftMask,   XK_5,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector4))       };
static const Key key48_ = {
    Mod1Mask|ShiftMask,   XK_6,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector5))       };
static const Key key49_ = {
    Mod1Mask|ShiftMask,   XK_7,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector6))       };
static const Key key50_ = {
    Mod1Mask|ShiftMask,   XK_8,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector7))       };
static const Key key51_ = {
    Mod1Mask|ShiftMask,   XK_9,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector8))       };
static const Key key52_ = {
    Mod1Mask|ShiftMask,   XK_0,      CHAIN(NeuroActionSetSendCurrClient, ARG_WSF(NeuroWorkspaceSelector9))       };
static const Key key53_ = {
    Mod1Mask|ControlMask, XK_1,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector0)) };
static const Key key54_ = {
    Mod1Mask|ControlMask, XK_2,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector1)) };
static const Key key55_ = {
    Mod1Mask|ControlMask, XK_3,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector2)) };
static const Key key56_ = {
    Mod1Mask|ControlMask, XK_4,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector3)) };
static const Key key57_ = {
    Mod1Mask|ControlMask, XK_5,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector4)) };
static const Key key58_ = {
    Mod1Mask|ControlMask, XK_6,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector5)) };
static const Key key59_ = {
    Mod1Mask|ControlMask, XK_7,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector6)) };
static const Key key60_ = {
    Mod1Mask|ControlMask, XK_8,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector7)) };
static const Key key61_ = {
    Mod1Mask|ControlMask, XK_9,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector8)) };
static const Key key62_ = {
    Mod1Mask|ControlMask, XK_0,      CHAIN(NeuroActionSetSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector9)) };
static const Key key63_ = {
    Mod1Mask|ShiftMask,   XK_q,      CHAIN_NULL(NeuroActionSetQuit)                                              };
static const Key key64_ = {
    Mod1Mask,             XK_q,      CHAIN_NULL(NeuroActionSetReload)                                            };
static const Key key65_ = {
    Mod1Mask,             XK_F2,     CHAIN(NeuroActionSetSpawn, ARG_CMD(NeuroConfigDefaultLauncherCommand))      };
static const Key key66_ = {
    Mod1Mask|ShiftMask,   XK_space,  CHAIN_NULL(NeuroActionSetResetLayout)                                       };
static const Key key67_ = {
    Mod1Mask,             XK_n,      CHAIN_NULL(NeuroActionSetMinimizeCurrClient)                                };
static const Key key68_ = {
    Mod1Mask|ShiftMask,   XK_n,      CHAIN_NULL(NeuroActionSetRestoreLastMinimized)                              };

// Button
static const Button button0_ = {
    NULL_MASK,          Button1, CHAIN_NULL(NeuroActionSetFocusPtrClient),            True  };
static const Button button1_ = {
    Mod1Mask,           Button1, CHAIN_NULL(NeuroActionSetFreeMovePtrClient),         False };
static const Button button2_ = {
    Mod1Mask,           Button2, CHAIN_NULL(NeuroActionSetToggleFreePtrClient),       False };
static const Button button3_ = {
    Mod1Mask,           Button3, CHAIN_NULL(NeuroActionSetFreeResizePtrClient),       False };
static const Button button4_ = {
    Mod1Mask|ShiftMask, Button1, CHAIN_NULL(NeuroActionSetFloatMovePtrClient),        False };
static const Button button5_ = {
    Mod1Mask|ShiftMask, Button2, CHAIN_NULL(NeuroActionSetToggleFullscreenPtrClient), False };
static const Button button6_ = {
    Mod1Mask|ShiftMask, Button3, CHAIN_NULL(NeuroActionSetFloatResizePtrClient),      False };

// Default configuration
static const Configuration default_ = {
  NeuroConfigDefaultInitActionChain,
  NeuroConfigDefaultStopActionChain,
  NeuroConfigDefaultNormalBorderColor,
  NeuroConfigDefaultCurrentBorderColor,
  NeuroConfigDefaultOldBorderColor,
  NeuroConfigDefaultFreeBorderColor,
  NeuroConfigDefaultUrgentBorderColor,
  NeuroConfigDefaultBorderWidth,
  NeuroConfigDefaultBorderGap,
  NeuroConfigDefaultWorkspaceSet,
  NeuroConfigDefaultRuleSet,
  NeuroConfigDefaultDzenPanelSet,
  NeuroConfigDefaultKeySet,
  NeuroConfigDefaultButtonSet
};

// Main configuration
static const Configuration *configuration_ = &default_;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Command
const char* NeuroConfigDefaultTerminalCommand[] = { "/usr/bin/xterm", NULL };
const char* NeuroConfigDefaultLauncherCommand[] = { "/usr/bin/gmrun", NULL };
const char* NeuroConfigDefaultScratchpadCommand[] = { "/usr/bin/xterm", "-name", RULE_SCRATCHPAD_NAME, NULL };

// LayoutSet
const LayoutConf* NeuroConfigDefaultLayoutSet[] = { &tile_, &mirror_, &grid_, NULL };
const LayoutConf* NeuroConfigDefaultToggledLayoutSet[] = { &full_, &float_, NULL };

// WorkspaceSet
const Workspace* NeuroConfigDefaultWorkspaceSet[] = {
    &ws0_, &ws1_, &ws2_, &ws3_, &ws4_, &ws5_, &ws6_, &ws7_, &ws8_, &ws9_, NULL };

// KeySet
const Key* NeuroConfigDefaultKeySet[] = {
  &key00_, &key01_, &key02_, &key03_, &key04_, &key05_, &key06_, &key07_, &key08_, &key09_,
  &key10_, &key11_, &key12_, &key13_, &key14_, &key15_, &key16_, &key17_, &key18_, &key19_,
  &key20_, &key21_, &key22_, &key23_, &key24_, &key25_, &key26_, &key27_, &key28_, &key29_,
  &key30_, &key31_, &key32_, &key33_, &key34_, &key35_, &key36_, &key37_, &key38_, &key39_,
  &key40_, &key41_, &key42_, &key43_, &key44_, &key45_, &key46_, &key47_, &key48_, &key49_,
  &key50_, &key51_, &key52_, &key53_, &key54_, &key55_, &key56_, &key57_, &key58_, &key59_,
  &key60_, &key61_, &key62_, &key63_, &key64_, &key65_, &key66_, &key67_, &key68_, NULL
};

// ButtonSet
const Button* NeuroConfigDefaultButtonSet[] = {
    &button0_, &button1_, &button2_, &button3_, &button4_, &button5_, &button5_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroConfigSet(const Configuration *c) {
  if (c)
    configuration_ = c;
}

const Configuration *NeuroConfigGet() {
  return configuration_;
}

