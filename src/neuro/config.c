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

// NeuroLayout
static const NeuroLayoutConf tile_ = {
  "Tile", NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, NEURO_LAYOUT_MOD_NULL, true,
  {NEURO_ARG_IDX(1U), NEURO_ARG_FLOAT(0.5f), NEURO_ARG_FLOAT(0.03f), NEURO_ARG_NULL}
};
static const NeuroLayoutConf mirror_ = {
  "Mirr", NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, NEURO_LAYOUT_MOD_MIRROR, true,
  {NEURO_ARG_IDX(1U), NEURO_ARG_FLOAT(0.5f), NEURO_ARG_FLOAT(0.03f), NEURO_ARG_NULL}
};
static const NeuroLayoutConf grid_ = {
  "Grid", NeuroLayoutArrangerGrid,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, NEURO_LAYOUT_MOD_NULL, true,
  {NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL}
};
static const NeuroLayoutConf full_ = {
  "Full", NeuroLayoutArrangerFull,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, NEURO_LAYOUT_MOD_NULL, true,
  {NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL}
};
static const NeuroLayoutConf float_ = {
  "Float", NeuroLayoutArrangerFloat,
  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterAlways, NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f}, NEURO_LAYOUT_MOD_NULL, false,
  {NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL}
};

// Monitors
static const NeuroMonitorConf m0_ = { "Main", 0U, {0, 0, 0, 0}, NULL };

// NeuroWorkspace
static const NeuroWorkspace ws0_ = { "ws1", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws1_ = { "ws2", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws2_ = { "ws3", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws3_ = { "ws4", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws4_ = { "ws5", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws5_ = { "ws6", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws6_ = { "ws7", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws7_ = { "ws8", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws8_ = { "ws9", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };
static const NeuroWorkspace ws9_ = { "ws0", NeuroConfigDefaultLayoutList, NeuroConfigDefaultToggledLayoutList };

// NeuroKey
static const NeuroKey key00_ = {
    Mod1Mask|ShiftMask, XK_Return,
    NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(NeuroConfigDefaultTerminalCommand))      };
static const NeuroKey key01_ = {
    Mod1Mask, XK_c,
    NEURO_CHAIN_NULL(NeuroActionListKillClient)                                              };
static const NeuroKey key02_ = {
    Mod1Mask, XK_j,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext))      };
static const NeuroKey key03_ = {
    Mod1Mask, XK_k,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorPrev))      };
static const NeuroKey key04_ = {
    Mod1Mask, XK_Tab,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorOld))       };
static const NeuroKey key05_ = {
    Mod1Mask, XK_m,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorHead))      };
static const NeuroKey key06_ = {
    Mod1Mask|ShiftMask, XK_j,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext))       };
static const NeuroKey key07_ = {
    Mod1Mask|ShiftMask, XK_k,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorPrev))       };
static const NeuroKey key08_ = {
    Mod1Mask|ShiftMask, XK_p,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorOld))        };
static const NeuroKey key09_ = {
    Mod1Mask|ShiftMask, XK_m,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorHead))       };
static const NeuroKey key10_ = {
    Mod1Mask, XK_Up,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorUp))        };
static const NeuroKey key11_ = {
    Mod1Mask, XK_Down,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorDown))      };
static const NeuroKey key12_ = {
    Mod1Mask, XK_Left,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLeft))      };
static const NeuroKey key13_ = {
    Mod1Mask, XK_Right,
    NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorRight))     };
static const NeuroKey key14_ = {
    Mod1Mask|ShiftMask, XK_Up,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorUp))         };
static const NeuroKey key15_ = {
    Mod1Mask|ShiftMask, XK_Down,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorDown))       };
static const NeuroKey key16_ = {
    Mod1Mask|ShiftMask, XK_Left,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLeft))       };
static const NeuroKey key17_ = {
    Mod1Mask|ShiftMask, XK_Right,
    NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorRight))      };
static const NeuroKey key18_ = {
    Mod1Mask, XK_space,
    NEURO_CHAIN_NULL(NeuroActionListChangeLayout)                                            };
static const NeuroKey key19_ = {
    Mod1Mask, XK_t,
    NEURO_CHAIN_NULL(NeuroActionListToggleFreeCurrClient)                                    };
static const NeuroKey key20_ = {
    Mod1Mask, XK_z,
    NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_MIRROR))     };
static const NeuroKey key21_ = {
    Mod1Mask, XK_x,
    NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_REFLECTX))   };
static const NeuroKey key22_ = {
    Mod1Mask, XK_y,
    NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MODE_REFLECTY))  };
static const NeuroKey key23_ = {
    Mod1Mask, XK_f,
    NEURO_CHAIN(NeuroActionListToggleLayout, NEURO_ARG_IDX(0U))                              };
static const NeuroKey key24_ = {
    Mod1Mask, XK_o,
    NEURO_CHAIN(NeuroActionListToggleLayout, NEURO_ARG_IDX(1U))                              };
static const NeuroKey key25_ = {
    Mod1Mask|ShiftMask, XK_f,
    NEURO_CHAIN_NULL(NeuroActionListToggleFullscreenCurrClient)                              };
static const NeuroKey key26_ = {
    Mod1Mask, XK_comma,
    NEURO_CHAIN_NULL(NeuroActionListIncreaseMasterLayout)                                    };
static const NeuroKey key27_ = {
    Mod1Mask, XK_period,
    NEURO_CHAIN(NeuroActionListIncreaseMasterLayout, NEURO_ARG_INT(-1))                      };
static const NeuroKey key28_ = {
    Mod1Mask, XK_l,
    NEURO_CHAIN_NULL(NeuroActionListResizeMasterLayout)                                      };
static const NeuroKey key29_ = {
    Mod1Mask, XK_h,
    NEURO_CHAIN(NeuroActionListResizeMasterLayout, NEURO_ARG_FLOAT(-1.0f))                   };
static const NeuroKey key30_ = {
    Mod1Mask, XK_1,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector0))      };
static const NeuroKey key31_ = {
    Mod1Mask, XK_2,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector1))      };
static const NeuroKey key32_ = {
    Mod1Mask, XK_3,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector2))      };
static const NeuroKey key33_ = {
    Mod1Mask, XK_4,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector3))      };
static const NeuroKey key34_ = {
    Mod1Mask, XK_5,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector4))      };
static const NeuroKey key35_ = {
    Mod1Mask, XK_6,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector5))      };
static const NeuroKey key36_ = {
    Mod1Mask, XK_7,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector6))      };
static const NeuroKey key37_ = {
    Mod1Mask, XK_8,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector7))      };
static const NeuroKey key38_ = {
    Mod1Mask, XK_9,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector8))      };
static const NeuroKey key39_ = {
    Mod1Mask, XK_0,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector9))      };
static const NeuroKey key40_ = {
    ControlMask|Mod1Mask, XK_Left,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorPrev))   };
static const NeuroKey key41_ = {
    ControlMask|Mod1Mask, XK_Right,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorNext))   };
static const NeuroKey key42_ = {
    Mod1Mask|ShiftMask, XK_Tab,
    NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorOld))    };
static const NeuroKey key43_ = {
    Mod1Mask|ShiftMask, XK_1,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector0))       };
static const NeuroKey key44_ = {
    Mod1Mask|ShiftMask, XK_2,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector1))       };
static const NeuroKey key45_ = {
    Mod1Mask|ShiftMask, XK_3,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector2))       };
static const NeuroKey key46_ = {
    Mod1Mask|ShiftMask, XK_4,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector3))       };
static const NeuroKey key47_ = {
    Mod1Mask|ShiftMask, XK_5,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector4))       };
static const NeuroKey key48_ = {
    Mod1Mask|ShiftMask, XK_6,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector5))       };
static const NeuroKey key49_ = {
    Mod1Mask|ShiftMask, XK_7,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector6))       };
static const NeuroKey key50_ = {
    Mod1Mask|ShiftMask, XK_8,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector7))       };
static const NeuroKey key51_ = {
    Mod1Mask|ShiftMask, XK_9,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector8))       };
static const NeuroKey key52_ = {
    Mod1Mask|ShiftMask, XK_0,
    NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector9))       };
static const NeuroKey key53_ = {
    Mod1Mask|ControlMask, XK_1,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector0)) };
static const NeuroKey key54_ = {
    Mod1Mask|ControlMask, XK_2,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector1)) };
static const NeuroKey key55_ = {
    Mod1Mask|ControlMask, XK_3,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector2)) };
static const NeuroKey key56_ = {
    Mod1Mask|ControlMask, XK_4,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector3)) };
static const NeuroKey key57_ = {
    Mod1Mask|ControlMask, XK_5,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector4)) };
static const NeuroKey key58_ = {
    Mod1Mask|ControlMask, XK_6,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector5)) };
static const NeuroKey key59_ = {
    Mod1Mask|ControlMask, XK_7,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector6)) };
static const NeuroKey key60_ = {
    Mod1Mask|ControlMask, XK_8,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector7)) };
static const NeuroKey key61_ = {
    Mod1Mask|ControlMask, XK_9,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector8)) };
static const NeuroKey key62_ = {
    Mod1Mask|ControlMask, XK_0,
    NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector9)) };
static const NeuroKey key63_ = {
    Mod1Mask|ShiftMask, XK_q,
    NEURO_CHAIN_NULL(NeuroActionListQuit)                                                    };
static const NeuroKey key64_ = {
    Mod1Mask, XK_q,
    NEURO_CHAIN_NULL(NeuroActionListReload)                                                  };
static const NeuroKey key65_ = {
    Mod1Mask, XK_F2,
    NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(NeuroConfigDefaultLauncherCommand))      };
static const NeuroKey key66_ = {
    Mod1Mask|ShiftMask, XK_space,
    NEURO_CHAIN_NULL(NeuroActionListResetLayout)                                             };
static const NeuroKey key67_ = {
    Mod1Mask, XK_n,
    NEURO_CHAIN_NULL(NeuroActionListMinimizeCurrClient)                                      };
static const NeuroKey key68_ = {
    Mod1Mask|ShiftMask, XK_n,
    NEURO_CHAIN_NULL(NeuroActionListRestoreLastMinimized)                                    };

// NeuroButton
static const NeuroButton button0_ = {
    NEURO_NULL_MASK,    Button1, NEURO_CHAIN_NULL(NeuroActionListFocusPtrClient),            true  };
static const NeuroButton button1_ = {
    Mod1Mask,           Button1, NEURO_CHAIN_NULL(NeuroActionListFreeMovePtrClient),         false };
static const NeuroButton button2_ = {
    Mod1Mask,           Button2, NEURO_CHAIN_NULL(NeuroActionListToggleFreePtrClient),       false };
static const NeuroButton button3_ = {
    Mod1Mask,           Button3, NEURO_CHAIN_NULL(NeuroActionListFreeResizePtrClient),       false };
static const NeuroButton button4_ = {
    Mod1Mask|ShiftMask, Button1, NEURO_CHAIN_NULL(NeuroActionListFloatMovePtrClient),        false };
static const NeuroButton button5_ = {
    Mod1Mask|ShiftMask, Button2, NEURO_CHAIN_NULL(NeuroActionListToggleFullscreenPtrClient), false };
static const NeuroButton button6_ = {
    Mod1Mask|ShiftMask, Button3, NEURO_CHAIN_NULL(NeuroActionListFloatResizePtrClient),      false };

// Default configuration
static const NeuroConfiguration default_ = {
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

// Main configuration
static const NeuroConfiguration *configuration_ = &default_;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Command
const char* NeuroConfigDefaultTerminalCommand[] = { "/usr/bin/xterm", NULL };
const char* NeuroConfigDefaultLauncherCommand[] = { "/usr/bin/gmrun", NULL };
const char* NeuroConfigDefaultScratchpadCommand[] = { "/usr/bin/xterm", "-name", RULE_SCRATCHPAD_NAME, NULL };

// NeuroMonitor List
const NeuroMonitorConf* NeuroConfigDefaultMonitorList[] = { &m0_, NULL };

// NeuroLayout List
const NeuroLayoutConf* NeuroConfigDefaultLayoutList[] = { &tile_, &mirror_, &grid_, NULL };
const NeuroLayoutConf* NeuroConfigDefaultToggledLayoutList[] = { &full_, &float_, NULL };

// NeuroWorkspace List
const NeuroWorkspace* NeuroConfigDefaultWorkspaceList[] = {
    &ws0_, &ws1_, &ws2_, &ws3_, &ws4_, &ws5_, &ws6_, &ws7_, &ws8_, &ws9_, NULL };

// NeuroKey List
const NeuroKey* NeuroConfigDefaultKeyList[] = {
  &key00_, &key01_, &key02_, &key03_, &key04_, &key05_, &key06_, &key07_, &key08_, &key09_,
  &key10_, &key11_, &key12_, &key13_, &key14_, &key15_, &key16_, &key17_, &key18_, &key19_,
  &key20_, &key21_, &key22_, &key23_, &key24_, &key25_, &key26_, &key27_, &key28_, &key29_,
  &key30_, &key31_, &key32_, &key33_, &key34_, &key35_, &key36_, &key37_, &key38_, &key39_,
  &key40_, &key41_, &key42_, &key43_, &key44_, &key45_, &key46_, &key47_, &key48_, &key49_,
  &key50_, &key51_, &key52_, &key53_, &key54_, &key55_, &key56_, &key57_, &key58_, &key59_,
  &key60_, &key61_, &key62_, &key63_, &key64_, &key65_, &key66_, &key67_, &key68_, NULL
};

// NeuroButton List
const NeuroButton* NeuroConfigDefaultButtonList[] = {
    &button0_, &button1_, &button2_, &button3_, &button4_, &button5_, &button5_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroConfigSet(const NeuroConfiguration *c) {
  if (c)
    configuration_ = c;
}

const NeuroConfiguration *NeuroConfigGet() {
  return configuration_;
}

