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
#include "../neuro/neurowm.h"

// Fonts
#define DZENPANELFONT "-*-terminus-medium-r-normal-*-11-*-*-*-*-*-*-*"

// Sizes
#define myBorderWidth 1
#define myBorderGap 0

// Modmask
// #define myModMask Mod4Mask
#define myModMask Mod1Mask


//----------------------------------------------------------------------------------------------------------------------
// COMMANDS
//----------------------------------------------------------------------------------------------------------------------

static const char* termcmd[]  = { "/usr/bin/urxvt", NULL };
static const char* nspcmd[]   = { "/usr/bin/urxvt", "-name", WM_SCRATCHPAD_NAME, NULL };
static const char* xdefload[] = { "/usr/bin/xrdb", "-load", "/home/julian/.config/xorg/Xdefaults", NULL };
static const char* wallcmd[]  = { "/usr/bin/feh", "--bg-scale", "/home/julian/Pictures/wallpapers/neurowm.png", NULL };
static const char* lchrcmd[]  = { "/usr/bin/gmrun", NULL };
static const char* volupcmd[] = { "/home/julian/bin/voldzen.sh", "+", "-d", NULL };
static const char* voldocmd[] = { "/home/julian/bin/voldzen.sh", "-", "-d", NULL };
static const char* songncmd[] = { "/usr/bin/mpc", "next", NULL };
static const char* songpcmd[] = { "/usr/bin/mpc", "prev", NULL };


//----------------------------------------------------------------------------------------------------------------------
// START/END UP HOOK
//----------------------------------------------------------------------------------------------------------------------

// STARTUP
static const Action action0[] = { { NeuroActionHandlerChangeNeurowmName, ARG_STR("LG3D") } };
static const Action action1[] = { { NeuroActionHandlerSpawn, ARG_CMD(xdefload) } };
static const Action action2[] = { { NeuroActionHandlerSpawn, ARG_CMD(wallcmd) } };

static const Action *startUpHookActions[] = { action0, action1, action2, NeuroActionStartCpuCalc, NULL };

static const ActionChain myStartUpHook[] = { CHAIN_NULL(startUpHookActions) };

// ENDUP
static const ActionChain myEndUpHook[] = { CHAIN_NULL(NeuroActionChainStopCpuCalc) };


//----------------------------------------------------------------------------------------------------------------------
// LAYOUTS
//----------------------------------------------------------------------------------------------------------------------

// LAYOUTCONFS (NAME, ARRANGERF, BORDERCOLORF, BORDERWIDTHF, BORDERGAPF, REGION (XYWH), MOD, FOLLOWMOUSE, ARGS)
static const LayoutConf tile[] = { {
  "Tile", NeuroLayoutArrangerTall, NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways, {0.0f, 0.0f, 1.0f, 1.0f},
  nullModL, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
} };
static const LayoutConf mirr[] = { {
  "Mirr", NeuroLayoutArrangerTall, NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
} };
static const LayoutConf grid[] = { {
  "Grid", NeuroLayoutArrangerGrid, NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart, NeuroClientBorderGapSetterAlways, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL|reflXModL|reflYModL, True, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
} };
static const LayoutConf full[] = { {
  "Full",  NeuroLayoutArrangerFull,  NeuroClientColorSetterAll, NeuroClientBorderWidthSetterSmart,  NeuroClientBorderGapSetterAlways, {0.0f, 0.0f, 1.0f, 1.0f},
  nullModL, True, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
} };
static const LayoutConf floa[] = { {
  "Float", NeuroLayoutArrangerFloat, NeuroClientColorSetterAll, NeuroClientBorderWidthSetterAlways, NeuroClientBorderGapSetterAlways, {0.0f, 0.0f, 1.0f, 1.0f},
  nullModL, False, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
} };

// LAYOUTSET
static const LayoutConf *myLayoutSet[] = { tile, mirr, grid, NULL };

// TOGLAYOUTSET
static const LayoutConf *myTogLayoutSet[] = { full, floa, NULL };


//----------------------------------------------------------------------------------------------------------------------
// WORKSPACES
//----------------------------------------------------------------------------------------------------------------------

// WORKSPACES (NAME, GAPS (UDLR), LAYOUTS, TOGLAYOUTS)
static const Workspace ws0[] = { { "Terminal",    {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws1[] = { { "Network",     {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws2[] = { { "Development", {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws3[] = { { "Graphics",    {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws4[] = { { "Chatting",    {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws5[] = { { "Video",       {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws6[] = { { "Alternate1",  {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws7[] = { { "Alternate2",  {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws8[] = { { "Alternate3",  {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };
static const Workspace ws9[] = { { "Alternate4",  {16, 16, 0, 0},  myLayoutSet, myTogLayoutSet } };

// WORKSPACESET
static const Workspace *myWorkspaceSet[] = { ws0, ws1, ws2, ws3, ws4, ws5, ws6, ws7, ws8, ws9, NULL };


//----------------------------------------------------------------------------------------------------------------------
// RULES
//----------------------------------------------------------------------------------------------------------------------

// RULES (ClASS, NAME, TITLE, FULLSCREEN, FREE, FIXED, FIXSIZE, WS, FOLLOW)
static const Rule rule00[] = { {
  "URxvt", WM_SCRATCHPAD_NAME, "urxvt", False, NeuroRuleFreeSetterScratchpad, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };
static const Rule rule01[] = { {
  NULL, NULL, "Firefox Preferences", False, NeuroRuleFreeSetterCenter, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };
static const Rule rule02[] = { {
  NULL, NULL, "Buddy List", False, NeuroRuleFreeSetterNull, NeuroRuleFixedPositionLeft, 0.2f, idx4W, False
} };
static const Rule rule03[] = { {
  "Pidgin", NULL, NULL, False, NeuroRuleFreeSetterNull, NeuroRuleFixedPositionNull, 0.0f, idx4W, False
} };
static const Rule rule04[] = { {
  "Firefox", NULL, NULL, False, NeuroRuleFreeSetterNull, NeuroRuleFixedPositionNull, 0.0f, idx1W, False
} };
static const Rule rule05[] = { {
  "Chromium", NULL, NULL, False, NeuroRuleFreeSetterNull, NeuroRuleFixedPositionNull, 0.0f, idx1W, False
} };
static const Rule rule06[] = { {
  "MPlayer", NULL, NULL, False, NeuroRuleFreeSetterCenter, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };
static const Rule rule07[] = { {
  "Gmrun", NULL, NULL, False, NeuroRuleFreeSetterCenter, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };
static const Rule rule08[] = { {
  "Xephyr", NULL, NULL, True, NeuroRuleFreeSetterBigCenter, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };
static const Rule rule09[] = { {
  "Transmission-gtk", NULL, NULL, False, NeuroRuleFreeSetterNull, NeuroRuleFixedPositionNull, 0.0f, idx9W, True
} };
static const Rule rule10[] = { {
  "PPSSPPSDL", "PPSSPPSDL", NULL, False, NeuroRuleFreeSetterCenter, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };
static const Rule rule11[] = { {
  "XCalc", NULL, NULL, False, NeuroRuleFreeSetterCenter, NeuroRuleFixedPositionNull, 0.0f, currW, False
} };

// RULESET
static const Rule *myRuleSet[] = {
  rule00, rule01, rule02, rule03, rule04, rule05, rule06, rule07, rule08, rule09,
  rule10, rule11, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// DZEN PANELS
//----------------------------------------------------------------------------------------------------------------------

// TOP LEFT
static const DzenFlags topLeftDzenFlags[] = { {
  0, 0, 1500, 16, NeuroThemeNnoellColorWhiteAlt, NeuroThemeNnoellColorBlack, 'l', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn topLeftLoggers[] = {
  NeuroThemeNnoellLoggerCurrLayout, NeuroThemeNnoellLoggerLayoutMod, NeuroThemeNnoellLoggerCurrWorkspace, NeuroThemeNnoellLoggerCurrTitle, NULL
};

// TOP RIGHT
static const DzenFlags topRightDzenFlags[] = { {
  1500, 0, 420, 16, NeuroThemeNnoellColorWhiteAlt, NeuroThemeNnoellColorBlack, 'r', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn topRightLoggers[] = {
  NeuroThemeNnoellLoggerUptime, NeuroThemeNnoellLoggerDateTime, NULL
};

// BOTTOM LEFT
static const DzenFlags botLeftDzenFlags[] = { {
  0, 1064, 920, 16, NeuroThemeNnoellColorWhiteAlt, NeuroThemeNnoellColorBlack, 'l', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn botLeftLoggers[] = {
  NeuroThemeNnoellLoggerWorkspaceList, NeuroThemeNnoellLoggerStackSize, NeuroThemeNnoellLoggerMinimizedCount, NULL
};

// BOTTOM RIGHT
static const DzenFlags botRightDzenFlags[] = { {
  920, 1064, 1000, 16, NeuroThemeNnoellColorWhiteAlt, NeuroThemeNnoellColorBlack, 'r', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn botRightLoggers[] = {
  NeuroThemeNnoellLoggerCpu, NeuroThemeNnoellLoggerRam, NeuroThemeNnoellLoggerTemperature, NeuroThemeNnoellLoggerWifiStrength, NULL
};

// DZENPANELS (DZENFLAGS, LOGGERS, SEP, REFRESH)
static const DzenPanel topLeftPanel[]  = { { topLeftDzenFlags,  topLeftLoggers,  " ",  WM_EVENT } };
static const DzenPanel topRightPanel[] = { { topRightDzenFlags, topRightLoggers, " ",  1        } };
static const DzenPanel botLeftPanel[]  = { { botLeftDzenFlags,  botLeftLoggers,  " ",  WM_EVENT } };
static const DzenPanel botRightPanel[] = { { botRightDzenFlags, botRightLoggers, " ",  1        } };

// DZENPANELSET
static const DzenPanel *myDzenPanelSet[] = { topLeftPanel, topRightPanel, botLeftPanel, botRightPanel, NULL };


//----------------------------------------------------------------------------------------------------------------------
// KEYS (GRABBED TO THE ROOT WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// KEY (MOD, KEY, ACTIONCHAIN)
static const Key key00[] = { { myModMask|ShiftMask,   XK_Return,    CHAIN(NeuroActionChainSpawn, ARG_CMD(termcmd))                 } };
static const Key key01[] = { { myModMask,             XK_c,         CHAIN_NULL(NeuroActionChainKillClient)                         } };
static const Key key02[] = { { myModMask,             XK_j,         CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorNext))         } };
static const Key key03[] = { { myModMask,             XK_k,         CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorPrev))         } };
static const Key key04[] = { { myModMask,             XK_Tab,       CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorOld))          } };
static const Key key05[] = { { myModMask,             XK_m,         CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorHead))         } };
static const Key key06[] = { { myModMask|ShiftMask,   XK_j,         CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorNext))          } };
static const Key key07[] = { { myModMask|ShiftMask,   XK_k,         CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorPrev))          } };
static const Key key08[] = { { myModMask|ShiftMask,   XK_p,         CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorOld))           } };
static const Key key09[] = { { myModMask|ShiftMask,   XK_m,         CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorHead))          } };
static const Key key10[] = { { myModMask,             XK_Up,        CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorUp))           } };
static const Key key11[] = { { myModMask,             XK_Down,      CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorDown))         } };
static const Key key12[] = { { myModMask,             XK_Left,      CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorLeft))         } };
static const Key key13[] = { { myModMask,             XK_Right,     CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(NeuroClientSelectorRight))        } };
static const Key key14[] = { { myModMask|ShiftMask,   XK_Up,        CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorUp))            } };
static const Key key15[] = { { myModMask|ShiftMask,   XK_Down,      CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorDown))          } };
static const Key key16[] = { { myModMask|ShiftMask,   XK_Left,      CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorLeft))          } };
static const Key key17[] = { { myModMask|ShiftMask,   XK_Right,     CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(NeuroClientSelectorRight))         } };
static const Key key18[] = { { myModMask,             XK_space,     CHAIN_NULL(NeuroActionChainChangeLayout)                       } };
static const Key key19[] = { { myModMask,             XK_t,         CHAIN_NULL(NeuroActionChainToggleFreeCurrClient)               } };
static const Key key20[] = { { myModMask,             XK_z,         CHAIN(NeuroActionChainToggleModLayout, ARG_UINT(mirrModL))     } };
static const Key key21[] = { { myModMask,             XK_x,         CHAIN(NeuroActionChainToggleModLayout, ARG_UINT(reflXModL))    } };
static const Key key22[] = { { myModMask,             XK_y,         CHAIN(NeuroActionChainToggleModLayout, ARG_UINT(reflYModL))    } };
static const Key key23[] = { { myModMask,             XK_f,         CHAIN(NeuroActionChainToggleLayout, ARG_INT(0))                } };
static const Key key24[] = { { myModMask,             XK_o,         CHAIN(NeuroActionChainToggleLayout, ARG_INT(1))                } };
static const Key key25[] = { { myModMask|ShiftMask,   XK_f,         CHAIN_NULL(NeuroActionChainToggleFullscreenCurrClient)         } };
static const Key key26[] = { { myModMask,             XK_comma,     CHAIN_NULL(NeuroActionChainIncreaseMasterLayout)               } };
static const Key key27[] = { { myModMask,             XK_period,    CHAIN(NeuroActionChainIncreaseMasterLayout, ARG_INT(-1))       } };
static const Key key28[] = { { myModMask,             XK_l,         CHAIN_NULL(NeuroActionChainResizeMasterLayout)                 } };
static const Key key29[] = { { myModMask,             XK_h,         CHAIN(NeuroActionChainResizeMasterLayout, ARG_FLOAT(-1.0f))    } };
static const Key key30[] = { { myModMask,             XK_1,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx0W)) } };
static const Key key31[] = { { myModMask,             XK_2,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx1W)) } };
static const Key key32[] = { { myModMask,             XK_3,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx2W)) } };
static const Key key33[] = { { myModMask,             XK_4,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx3W)) } };
static const Key key34[] = { { myModMask,             XK_5,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx4W)) } };
static const Key key35[] = { { myModMask,             XK_6,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx5W)) } };
static const Key key36[] = { { myModMask,             XK_7,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx6W)) } };
static const Key key37[] = { { myModMask,             XK_8,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx7W)) } };
static const Key key38[] = { { myModMask,             XK_9,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx8W)) } };
static const Key key39[] = { { myModMask,             XK_0,         CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(idx9W)) } };
static const Key key40[] = { { ControlMask|Mod1Mask,  XK_Left,      CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(prevW)) } };
static const Key key41[] = { { ControlMask|Mod1Mask,  XK_Right,     CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(nextW)) } };
static const Key key42[] = { { myModMask|ShiftMask,   XK_Tab,       CHAIN(NeuroActionChainChangeWorkspace, ARG_WSF(oldW))  } };
static const Key key43[] = { { myModMask|ShiftMask,   XK_1,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx0W))  } };
static const Key key44[] = { { myModMask|ShiftMask,   XK_2,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx1W))  } };
static const Key key45[] = { { myModMask|ShiftMask,   XK_3,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx2W))  } };
static const Key key46[] = { { myModMask|ShiftMask,   XK_4,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx3W))  } };
static const Key key47[] = { { myModMask|ShiftMask,   XK_5,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx4W))  } };
static const Key key48[] = { { myModMask|ShiftMask,   XK_6,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx5W))  } };
static const Key key49[] = { { myModMask|ShiftMask,   XK_7,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx6W))  } };
static const Key key50[] = { { myModMask|ShiftMask,   XK_8,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx7W))  } };
static const Key key51[] = { { myModMask|ShiftMask,   XK_9,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx8W))  } };
static const Key key52[] = { { myModMask|ShiftMask,   XK_0,         CHAIN(NeuroActionChainSendCurrClient, ARG_WSF(idx9W))  } };
static const Key key53[] = { { myModMask|ControlMask, XK_1,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx0W))        } };
static const Key key54[] = { { myModMask|ControlMask, XK_2,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx1W))        } };
static const Key key55[] = { { myModMask|ControlMask, XK_3,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx2W))        } };
static const Key key56[] = { { myModMask|ControlMask, XK_4,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx3W))        } };
static const Key key57[] = { { myModMask|ControlMask, XK_5,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx4W))        } };
static const Key key58[] = { { myModMask|ControlMask, XK_6,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx5W))        } };
static const Key key59[] = { { myModMask|ControlMask, XK_7,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx6W))        } };
static const Key key60[] = { { myModMask|ControlMask, XK_8,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx7W))        } };
static const Key key61[] = { { myModMask|ControlMask, XK_9,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx8W))        } };
static const Key key62[] = { { myModMask|ControlMask, XK_0,         CHAIN(NeuroActionChainSendFollowCurrClient, ARG_WSF(idx9W))        } };
static const Key key63[] = { { myModMask|ShiftMask,   XK_q,         CHAIN_NULL(NeuroActionChainQuit)                               } };
static const Key key64[] = { { myModMask,             XK_q,         CHAIN_NULL(NeuroActionChainReload)                             } };
static const Key key65[] = { { Mod1Mask,              XK_F2,        CHAIN(NeuroActionChainSpawn, ARG_CMD(lchrcmd))                 } };
static const Key key66[] = { { myModMask|ShiftMask,   XK_space,     CHAIN_NULL(NeuroActionChainResetLayout)                        } };
static const Key key67[] = { { myModMask,             XK_n,         CHAIN_NULL(NeuroActionChainMinimizeCurrClient)                 } };
static const Key key68[] = { { myModMask|ShiftMask,   XK_n,         CHAIN_NULL(NeuroActionChainRestoreLastMinimized)               } };
static const Key key69[] = { { myModMask|ShiftMask,   XK_t,         CHAIN(NeuroActionChainFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterBigCenter)) } };
static const Key key70[] = { { myModMask,             XK_grave,     CHAIN(NeuroActionChainToggleScratchpad, ARG_CMD(nspcmd))       } };
static const Key key71[] = { { myModMask,             XK_masculine, CHAIN(NeuroActionChainToggleScratchpad, ARG_CMD(nspcmd))       } };
static const Key key72[] = { { Mod1Mask,              XK_Up,        CHAIN(NeuroActionChainSpawn, ARG_CMD(volupcmd))                } };
static const Key key73[] = { { Mod1Mask,              XK_Down,      CHAIN(NeuroActionChainSpawn, ARG_CMD(voldocmd))                } };
static const Key key74[] = { { Mod1Mask,              XK_Right,     CHAIN(NeuroActionChainSpawn, ARG_CMD(songncmd))                } };
static const Key key75[] = { { Mod1Mask,              XK_Left,      CHAIN(NeuroActionChainSpawn, ARG_CMD(songpcmd))                } };

// KEYS
static const Key *myKeys[] = {
  key00, key01, key02, key03, key04, key05, key06, key07, key08, key09,
  key10, key11, key12, key13, key14, key15, key16, key17, key18, key19,
  key20, key21, key22, key23, key24, key25, key26, key27, key28, key29,
  key30, key31, key32, key33, key34, key35, key36, key37, key38, key39,
  key40, key41, key42, key43, key44, key45, key46, key47, key48, key49,
  key50, key51, key52, key53, key54, key55, key56, key57, key58, key59,
  key60, key61, key62, key63, key64, key65, key66, key67, key68, key69,
  key70, key71, key72, key73, key74, key75, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// BUTTONS (GRABBED TO EACH WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// BUTTON (MOD, BUTTON, ACTIONCHAIN, UNWRAPONFOCUS)
static const Button button00[] = { { NULL_MASK,           Button1, CHAIN_NULL(NeuroActionChainFocusPtrClient),            True  } };
static const Button button01[] = { { myModMask,           Button1, CHAIN_NULL(NeuroActionChainFreeMovePtrClient),         False } };
static const Button button02[] = { { myModMask,           Button2, CHAIN_NULL(NeuroActionChainToggleFreePtrClient),       False } };
static const Button button03[] = { { myModMask,           Button3, CHAIN_NULL(NeuroActionChainFreeResizePtrClient),       False } };
static const Button button04[] = { { myModMask|ShiftMask, Button1, CHAIN_NULL(NeuroActionChainFloatMovePtrClient),        False } };
static const Button button05[] = { { myModMask|ShiftMask, Button2, CHAIN_NULL(NeuroActionChainToggleFullscreenPtrClient), False } };
static const Button button06[] = { { myModMask|ShiftMask, Button3, CHAIN_NULL(NeuroActionChainFloatResizePtrClient),      False } };

// BUTTONS
static const Button *myButtons[] = { button00, button01, button02, button03, button04, button05, button06, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static const Configuration myConfiguration = {
  NeuroConfigNormBorderColor,
  NeuroConfigCurrBorderColor,
  NeuroConfigPrevBorderColor,
  NeuroConfigFreeBorderColor,
  NeuroConfigUrgtBorderColor,
  myBorderWidth,
  myBorderGap,
  myWorkspaceSet,
  myRuleSet,
  myDzenPanelSet,  // Dzen must be installed
  myKeys,
  myButtons,
  myStartUpHook,
  myEndUpHook
};


//----------------------------------------------------------------------------------------------------------------------
// MAIN
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  return NeuroNeurowmRun(&myConfiguration);
  // return NeuroNeurowmRun(&NeuroConfigConfiguration);
}

