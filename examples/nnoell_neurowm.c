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
#include <neuro/neurowm.h>

// Fonts
#define DZENPANELFONT "-*-terminus-medium-r-normal-*-11-*-*-*-*-*-*-*"

// Sizes
#define myBorderWidth 1
#define myBorderGap 0

// Modmask
#define myModMask Mod4Mask
// #define myModMask Mod1Mask


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

// ACTIONS (ACTION)
static const Action action0[] = { { changeWMNameN,  {.str = "LG3D"} }   };
static const Action action1[] = { { spawnN,         {.cmd = xdefload} } };
static const Action action2[] = { { spawnN,         {.cmd = wallcmd} }  };
static const Action action3[] = { { startCpuCalcDP, NO_ARG }         };
static const Action action4[] = { { endCpuCalcDP,   NO_ARG }         };

// STARTUP
static const Action *myStartUpHook[] = { action0, action1, action2, action3, NULL };

// ENDUP
static const Action *myEndUpHook[] = { action4, NULL };


//----------------------------------------------------------------------------------------------------------------------
// LAYOUTS
//----------------------------------------------------------------------------------------------------------------------

// LAYOUTCONFS (NAME, ARRANGE, BORDERCOLORF, BORDERWIDTHF, BORDERGAPF, REGION (XYWH), MOD, FOLLOWMOUSE, ARGS)
static const LayoutConf tile[] = { {
  "Tile", tallArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), NO_ARG}
} };
static const LayoutConf mirr[] = { {
  "Mirr", tallArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), NO_ARG}
} };
static const LayoutConf grid[] = { {
  "Grid", gridArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL|reflXModL|reflYModL, True, {NO_ARG, NO_ARG, NO_ARG, NO_ARG}
} };
static const LayoutConf full[] = { {
  "Full",  fullArrL,  allBorderColorC, smartBorderWidthC,  alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {NO_ARG, NO_ARG, NO_ARG, NO_ARG}
} };
static const LayoutConf floa[] = { {
  "Float", floatArrL, allBorderColorC, alwaysBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, False, {NO_ARG, NO_ARG, NO_ARG, NO_ARG}
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
  "URxvt", WM_SCRATCHPAD_NAME, "urxvt", False, scratchpadFreeR, notFixedR, 0.0f, currWSR, False
} };
static const Rule rule01[] = { {
  NULL, NULL, "Firefox Preferences", False, centerFreeR, notFixedR, 0.0f, currWSR, False
} };
static const Rule rule02[] = { {
  NULL, NULL, "Buddy List", False, notFreeR, leftFixedR, 0.2f, 4, False
} };
static const Rule rule03[] = { {
  "Pidgin", NULL, NULL, False, notFreeR, notFixedR, 0.0f, 4, False
} };
static const Rule rule04[] = { {
  "Firefox", NULL, NULL, False, notFreeR, notFixedR, 0.0f, 1, False
} };
static const Rule rule05[] = { {
  "Chromium", NULL, NULL, False, notFreeR, notFixedR, 0.0f, 1, False
} };
static const Rule rule06[] = { {
  "MPlayer", NULL, NULL, False, centerFreeR, notFixedR, 0.0f, currWSR, False
} };
static const Rule rule07[] = { {
  "Gmrun", NULL, NULL, False, centerFreeR, notFixedR, 0.0f, currWSR, False
} };
static const Rule rule08[] = { {
  "Xephyr", NULL, NULL, True, bigCenterFreeR, notFixedR, 0.0f, currWSR, False
} };
static const Rule rule09[] = { {
  "Transmission-gtk", NULL, NULL, False, notFreeR, notFixedR, 0.0f, 9, True
} };
static const Rule rule10[] = { {
  "PPSSPPSDL", "PPSSPPSDL", NULL, False, centerFreeR, notFixedR, 0.0f, currWSR, False
} };
static const Rule rule11[] = { {
  "XCalc", NULL, NULL, False, centerFreeR, notFixedR, 0.0f, currWSR, False
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
  0, 0, 1500, 16, NNOELL_colorWhiteAltP, NNOELL_colorBlackP, 'l', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn topLeftLoggers[] = {
  nnoellCurrLayoutLoggerP, nnoellCurrLayoutModLoggerP, nnoellCurrWSLoggerP, nnoellCurrTitleLoggerP, NULL
};

// TOP RIGHT
static const DzenFlags topRightDzenFlags[] = { {
  1500, 0, 420, 16, NNOELL_colorWhiteAltP, NNOELL_colorBlackP, 'r', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn topRightLoggers[] = {
  nnoellUptimeLoggerP, nnoellDateTimeLoggerP, NULL
};

// BOTTOM LEFT
static const DzenFlags botLeftDzenFlags[] = { {
  0, 1064, 920, 16, NNOELL_colorWhiteAltP, NNOELL_colorBlackP, 'l', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn botLeftLoggers[] = {
  nnoellCurrWSListLoggerP, nnoellCurrSizeStackLoggerP, nnoellCurrMinimizedCountLoggerP, NULL
};

// BOTTOM RIGHT
static const DzenFlags botRightDzenFlags[] = { {
  920, 1064, 1000, 16, NNOELL_colorWhiteAltP, NNOELL_colorBlackP, 'r', DZENPANELFONT, "onstart=lower", "-p"
} };
static const LoggerFn botRightLoggers[] = {
  nnoellCPUUsageLoggerP, nnoellMemPercLoggerP, nnoellTempLoggerP, nnoellWifiStrengthLoggerP, NULL
};

// DZENPANELS (DZENFLAGS, LOGGERS, SEP, REFRESH)
static const DzenPanel topLeftPanel[]  = { { topLeftDzenFlags,  topLeftLoggers,  " ",  XEvDP } };
static const DzenPanel topRightPanel[] = { { topRightDzenFlags, topRightLoggers, " ",  1     } };
static const DzenPanel botLeftPanel[]  = { { botLeftDzenFlags,  botLeftLoggers,  " ",  XEvDP } };
static const DzenPanel botRightPanel[] = { { botRightDzenFlags, botRightLoggers, " ",  1     } };

// DZENPANELSET
static const DzenPanel *myDzenPanelSet[] = { topLeftPanel, topRightPanel, botLeftPanel, botRightPanel, NULL };


//----------------------------------------------------------------------------------------------------------------------
// KEYS (GRABBED TO THE ROOT WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// KEY (MOD, KEY, ACTION)
static const Key key00[] = { { myModMask|ShiftMask,  XK_Return,    { spawnN, ARG_CMD(termcmd) }              } };
static const Key key01[] = { { myModMask,            XK_c,         { killCliN, NO_ARG }                      } };
static const Key key02[] = { { myModMask,            XK_j,         { moveFocusN, ARG_SCF(nextC) }            } };
static const Key key03[] = { { myModMask,            XK_k,         { moveFocusN, ARG_SCF(prevC) }            } };
static const Key key04[] = { { myModMask,            XK_Tab,       { moveFocusN, ARG_SCF(oldC) }             } };
static const Key key05[] = { { myModMask,            XK_m,         { moveFocusN, ARG_SCF(headC) }            } };
static const Key key06[] = { { myModMask|ShiftMask,  XK_j,         { swapCliN, ARG_SCF(nextC) }              } };
static const Key key07[] = { { myModMask|ShiftMask,  XK_k,         { swapCliN, ARG_SCF(prevC) }              } };
static const Key key08[] = { { myModMask|ShiftMask,  XK_p,         { swapCliN, ARG_SCF(oldC) }               } };
static const Key key09[] = { { myModMask|ShiftMask,  XK_m,         { swapCliN, ARG_SCF(headC) }              } };
static const Key key10[] = { { myModMask,            XK_Up,        { moveFocusN, ARG_SCF(upC) }              } };
static const Key key11[] = { { myModMask,            XK_Down,      { moveFocusN, ARG_SCF(downC) }            } };
static const Key key12[] = { { myModMask,            XK_Left,      { moveFocusN, ARG_SCF(leftC) }            } };
static const Key key13[] = { { myModMask,            XK_Right,     { moveFocusN, ARG_SCF(rightC) }           } };
static const Key key14[] = { { myModMask|ShiftMask,  XK_Up,        { swapCliN, ARG_SCF(upC) }                } };
static const Key key15[] = { { myModMask|ShiftMask,  XK_Down,      { swapCliN, ARG_SCF(downC) }              } };
static const Key key16[] = { { myModMask|ShiftMask,  XK_Left,      { swapCliN, ARG_SCF(leftC) }              } };
static const Key key17[] = { { myModMask|ShiftMask,  XK_Right,     { swapCliN, ARG_SCF(rightC) }             } };
static const Key key18[] = { { myModMask,            XK_space,     { changeLayoutN, ARG_INT(1) }             } };
static const Key key19[] = { { myModMask,            XK_t,         { toggleFreeCliN, ARG_FLF(defFreeR) }     } };
static const Key key20[] = { { myModMask,            XK_z,         { toggleLayoutModN, ARG_UINT(mirrModL) }  } };
static const Key key21[] = { { myModMask,            XK_x,         { toggleLayoutModN, ARG_UINT(reflXModL) } } };
static const Key key22[] = { { myModMask,            XK_y,         { toggleLayoutModN, ARG_UINT(reflYModL) } } };
static const Key key23[] = { { myModMask,            XK_f,         { toggleLayoutN, ARG_INT(0) }             } };
static const Key key24[] = { { myModMask,            XK_o,         { toggleLayoutN, ARG_INT(1) }             } };
static const Key key25[] = { { myModMask|ShiftMask,  XK_f,         { toggleFullScreenCliN, NO_ARG }          } };
static const Key key26[] = { { myModMask,            XK_comma,     { increaseMasterN, ARG_INT(1) }           } };
static const Key key27[] = { { myModMask,            XK_period,    { increaseMasterN, ARG_INT(-1) }          } };
static const Key key28[] = { { myModMask,            XK_l,         { resizeMasterN, ARG_FLOAT(1.0f) }        } };
static const Key key29[] = { { myModMask,            XK_h,         { resizeMasterN, ARG_FLOAT(-1.0f) }       } };
static const Key key30[] = { { myModMask,            XK_1,         { changeToWorkspaceN, ARG_INT(0) }        } };
static const Key key31[] = { { myModMask,            XK_2,         { changeToWorkspaceN, ARG_INT(1) }        } };
static const Key key32[] = { { myModMask,            XK_3,         { changeToWorkspaceN, ARG_INT(2) }        } };
static const Key key33[] = { { myModMask,            XK_4,         { changeToWorkspaceN, ARG_INT(3) }        } };
static const Key key34[] = { { myModMask,            XK_5,         { changeToWorkspaceN, ARG_INT(4) }        } };
static const Key key35[] = { { myModMask,            XK_6,         { changeToWorkspaceN, ARG_INT(5) }        } };
static const Key key36[] = { { myModMask,            XK_7,         { changeToWorkspaceN, ARG_INT(6) }        } };
static const Key key37[] = { { myModMask,            XK_8,         { changeToWorkspaceN, ARG_INT(7) }        } };
static const Key key38[] = { { myModMask,            XK_9,         { changeToWorkspaceN, ARG_INT(8) }        } };
static const Key key39[] = { { myModMask,            XK_0,         { changeToWorkspaceN, ARG_INT(9) }        } };
static const Key key40[] = { { ControlMask|Mod1Mask, XK_Left,      { changeToPrevWorkspaceN, NO_ARG }        } };
static const Key key41[] = { { ControlMask|Mod1Mask, XK_Right,     { changeToNextWorkspaceN, NO_ARG }        } };
static const Key key42[] = { { myModMask|ShiftMask,  XK_Tab,       { changeToLastWorkspaceN, NO_ARG }        } };
static const Key key43[] = { { myModMask|ShiftMask,  XK_1,         { moveCliToWorkspaceN, ARG_INT(0) }       } };
static const Key key44[] = { { myModMask|ShiftMask,  XK_2,         { moveCliToWorkspaceN, ARG_INT(1) }       } };
static const Key key45[] = { { myModMask|ShiftMask,  XK_3,         { moveCliToWorkspaceN, ARG_INT(2) }       } };
static const Key key46[] = { { myModMask|ShiftMask,  XK_4,         { moveCliToWorkspaceN, ARG_INT(3) }       } };
static const Key key47[] = { { myModMask|ShiftMask,  XK_5,         { moveCliToWorkspaceN, ARG_INT(4) }       } };
static const Key key48[] = { { myModMask|ShiftMask,  XK_6,         { moveCliToWorkspaceN, ARG_INT(5) }       } };
static const Key key49[] = { { myModMask|ShiftMask,  XK_7,         { moveCliToWorkspaceN, ARG_INT(6) }       } };
static const Key key50[] = { { myModMask|ShiftMask,  XK_8,         { moveCliToWorkspaceN, ARG_INT(7) }       } };
static const Key key51[] = { { myModMask|ShiftMask,  XK_9,         { moveCliToWorkspaceN, ARG_INT(8) }       } };
static const Key key52[] = { { myModMask|ShiftMask,  XK_0,         { moveCliToWorkspaceN, ARG_INT(9) }       } };
static const Key key53[] = { { myModMask|ShiftMask,  XK_q,         { quitN, NO_ARG }                         } };
static const Key key54[] = { { myModMask,            XK_q,         { reloadN, NO_ARG }                       } };
static const Key key55[] = { { Mod1Mask,             XK_F2,        { spawnN, ARG_CMD(lchrcmd) }              } };
static const Key key56[] = { { myModMask|ShiftMask,  XK_space,     { resetLayoutN, NO_ARG }                  } };
static const Key key57[] = { { myModMask,            XK_n,         { minimizeCliN, NO_ARG }                  } };
static const Key key58[] = { { myModMask|ShiftMask,  XK_n,         { restoreCliN,  NO_ARG }                  } };
static const Key key59[] = { { myModMask|ShiftMask,  XK_t,         { freeCliN, ARG_FLF(bigCenterFreeR) }     } };
static const Key key60[] = { { myModMask,            XK_grave,     { toggleNSPN, ARG_CMD(nspcmd) }           } };
static const Key key61[] = { { myModMask,            XK_masculine, { toggleNSPN, ARG_CMD(nspcmd) }           } };
static const Key key62[] = { { Mod1Mask,             XK_Up,        { spawnN, ARG_CMD(volupcmd) }             } };
static const Key key63[] = { { Mod1Mask,             XK_Down,      { spawnN, ARG_CMD(voldocmd) }             } };
static const Key key64[] = { { Mod1Mask,             XK_Right,     { spawnN, ARG_CMD(songncmd) }             } };
static const Key key65[] = { { Mod1Mask,             XK_Left,      { spawnN, ARG_CMD(songpcmd) }             } };

// KEYS
static const Key *myKeys[] = {
  key00, key01, key02, key03, key04, key05, key06, key07, key08, key09,
  key10, key11, key12, key13, key14, key15, key16, key17, key18, key19,
  key20, key21, key22, key23, key24, key25, key26, key27, key28, key29,
  key30, key31, key32, key33, key34, key35, key36, key37, key38, key39,
  key40, key41, key42, key43, key44, key45, key46, key47, key48, key49,
  key50, key51, key52, key53, key54, key55, key56, key57, key58, key59,
  key60, key61, key62, key63, key64, key65, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// BUTTONS (GRABBED TO EACH WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// BUTTON (MOD, BUTTON, ACTION, UNWRAPONFOCUS)
static const Button button00[] = { { noModMask,           Button1, { moveFocusN, ARG_SCF(pointerC) },        True  } };
static const Button button01[] = { { myModMask,           Button1, { freeMovePointerCliN, NO_ARG },          False } };
static const Button button02[] = { { myModMask,           Button2, { toggleFreePtrCliN, ARG_FLF(defFreeR) }, False } };
static const Button button03[] = { { myModMask,           Button3, { freeResizePointerCliN, NO_ARG },        False } };
static const Button button04[] = { { myModMask|ShiftMask, Button1, { movePointerCliN, NO_ARG },              False } };
static const Button button05[] = { { myModMask|ShiftMask, Button2, { toggleFullScreenPtrCliN, NO_ARG },      False } };
static const Button button06[] = { { myModMask|ShiftMask, Button3, { resizePointerCliN, NO_ARG },            False } };

// BUTTONS
static const Button *myButtons[] = { button00, button01, button02, button03, button04, button05, button06, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static const WMConfig myWMConfig = {
  defNormBorderColor,
  defCurrBorderColor,
  defPrevBorderColor,
  defFreeBorderColor,
  defUrgtBorderColor,
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
  return neurowm(&myWMConfig);
  // return neurowm(&defWMConfig);
}

