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
#include "../neuro/wm.h"

// Defines
// #define mod_mask_ Mod4Mask
#define mod_mask_ Mod1Mask


//----------------------------------------------------------------------------------------------------------------------
// COMMANDS
//----------------------------------------------------------------------------------------------------------------------

// COMMAND
static const char* termcmd_[]  = { "/usr/bin/urxvt", NULL };
static const char* nspcmd_[]   = { "/usr/bin/urxvt", "-name", RULE_SCRATCHPAD_NAME, NULL };
static const char* xdefload_[] = { "/usr/bin/xrdb", "-load", "/home/julian/.config/xorg/Xdefaults", NULL };
static const char* wallcmd_[]  = { "/usr/bin/feh", "--bg-scale", "/home/julian/Pictures/wallpapers/neurowm.png", NULL };
static const char* volupcmd_[] = { "/home/julian/bin/voldzen.sh", "+", "-d", NULL };
static const char* voldocmd_[] = { "/home/julian/bin/voldzen.sh", "-", "-d", NULL };
static const char* songncmd_[] = { "/usr/bin/mpc", "next", NULL };
static const char* songpcmd_[] = { "/usr/bin/mpc", "prev", NULL };


//----------------------------------------------------------------------------------------------------------------------
// ACTIONS
//----------------------------------------------------------------------------------------------------------------------

// ACTION (HANDLER, ARG)
static const Action action0_ = { NeuroActionHandlerChangeWmName, ARG_STR("LG3D") };
static const Action action1_ = { NeuroActionHandlerSpawn, ARG_CMD(xdefload_) };
static const Action action2_ = { NeuroActionHandlerSpawn, ARG_CMD(wallcmd_) };

// ACTIONSET
static const Action* init_action_list_[] = { &action0_, &action1_, &action2_, &NeuroActionInitCpuCalc, NULL };


//----------------------------------------------------------------------------------------------------------------------
// LAYOUTS
//----------------------------------------------------------------------------------------------------------------------

// LAYOUTCONF (NAME, ARRANGERF, BORDERCOLORF, BORDERWIDTHF, BORDERGAPF, REGION (XYWH), MOD, FOLLOWMOUSE, ARGS)
static const LayoutConf tile_ = {
  "Tile",
  NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  LayoutModNull,
  true,
  {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
};
static const LayoutConf mirror_ = {
  "Mirr",
  NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  LayoutModMirror,
  true,
  {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
};
static const LayoutConf grid_ = {
  "Grid",
  NeuroLayoutArrangerGrid,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  LayoutModMirror|LayoutModReflectX|LayoutModReflectY,
  true,
  {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
};
static const LayoutConf full_ = {
  "Full",
  NeuroLayoutArrangerFull,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  LayoutModNull,
  true,
  {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
};
static const LayoutConf float_ = {
  "Float",
  NeuroLayoutArrangerFloat,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterAlways,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  LayoutModNull,
  false,
  {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
};

// LAYOUTSET
static const LayoutConf* layout_list_[] = { &tile_, &mirror_, &grid_, NULL };
static const LayoutConf* toggled_layout_list_[] = { &full_, &float_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// WORKSPACES
//----------------------------------------------------------------------------------------------------------------------

// WORKSPACE (NAME, GAPS (UDLR), LAYOUTS, TOGLAYOUTS)
static const Workspace ws0_ = { "Terminal",    {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws1_ = { "Network",     {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws2_ = { "Development", {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws3_ = { "Graphics",    {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws4_ = { "Chatting",    {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws5_ = { "Video",       {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws6_ = { "Alternate1",  {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws7_ = { "Alternate2",  {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws8_ = { "Alternate3",  {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };
static const Workspace ws9_ = { "Alternate4",  {16, 16, 0, 0},  layout_list_, toggled_layout_list_ };

// WORKSPACESET
static const Workspace* workspace_list_[] = {
    &ws0_, &ws1_, &ws2_, &ws3_, &ws4_, &ws5_, &ws6_, &ws7_, &ws8_, &ws9_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// RULES
//----------------------------------------------------------------------------------------------------------------------

// RULE (ClASS, NAME, TITLE, FULLSCREEN, FREE, FIXED, FIXSIZE, WS, FOLLOW)
static const Rule rule00_ = {
  "URxvt", RULE_SCRATCHPAD_NAME, "urxvt",
  false, NeuroRuleFreeSetterScratchpad, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const Rule rule01_ = {
  NULL, NULL, "Firefox Preferences",
  false, NeuroRuleFreeSetterCenter, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const Rule rule02_ = {
  NULL, NULL, "Buddy List",
  false, NeuroRuleFreeSetterNull, RuleFixedPositionLeft, 0.2f, NeuroWorkspaceSelector4, false
};
static const Rule rule03_ = {
  "Pidgin", NULL, NULL,
  false, NeuroRuleFreeSetterNull, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelector4, false
};
static const Rule rule04_ = {
  "Firefox", NULL, NULL,
  false, NeuroRuleFreeSetterNull, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelector1, false
};
static const Rule rule05_ = {
  "chromium", NULL, NULL,
  false, NeuroRuleFreeSetterNull, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelector1, false
};
static const Rule rule06_ = {
  "MPlayer", NULL, NULL,
  false, NeuroRuleFreeSetterCenter, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const Rule rule07_ = {
  "Gmrun", NULL, NULL,
  false, NeuroRuleFreeSetterCenter, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const Rule rule08_ = {
  "Xephyr", NULL, NULL,
  true, NeuroRuleFreeSetterBigCenter, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const Rule rule09_ = {
  "Transmission-gtk", NULL, NULL,
  false, NeuroRuleFreeSetterNull, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelector9, true
};
static const Rule rule10_ = {
  "PPSSPPSDL", "PPSSPPSDL", NULL,
  false, NeuroRuleFreeSetterCenter, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const Rule rule11_ = {
  "XCalc", NULL, NULL,
  false, NeuroRuleFreeSetterCenter, RuleFixedPositionNull, 0.0f, NeuroWorkspaceSelectorCurr, false
};

// RULESET
static const Rule* rule_list_[] = {
  &rule00_, &rule01_, &rule02_, &rule03_, &rule04_, &rule05_, &rule06_, &rule07_, &rule08_, &rule09_,
  &rule10_, &rule11_, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// DZEN PANELS
//----------------------------------------------------------------------------------------------------------------------

// TOP LEFT
static const DzenFlags top_left_dzen_flags_ = {
  0, 0, 1500, 16,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellColorBlack,
  'l', NeuroThemeNnoellFontDzenPanel, "onstart=lower", "-p"
};
static const LoggerFn top_left_loggers_[] = {
  NeuroThemeNnoellLoggerCurrLayout,
  NeuroThemeNnoellLoggerLayoutMod,
  NeuroThemeNnoellLoggerCurrWorkspace,
  NeuroThemeNnoellLoggerCurrTitle,
  NULL
};

// TOP RIGHT
static const DzenFlags top_right_dzen_flags_ = {
  1500, 0, 420, 16,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellColorBlack,
  'r', NeuroThemeNnoellFontDzenPanel, "onstart=lower", "-p"
};
static const LoggerFn top_right_loggers_[] = {
  NeuroThemeNnoellLoggerUptime,
  NeuroThemeNnoellLoggerDateTime,
  NULL
};

// BOTTOM LEFT
static const DzenFlags bottom_left_dzen_flags_ = {
  0, 1064, 920, 16,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellColorBlack,
  'l', NeuroThemeNnoellFontDzenPanel, "onstart=lower", "-p"
};
static const LoggerFn bottom_left_loggers_[] = {
  NeuroThemeNnoellLoggerWorkspaceList,
  NeuroThemeNnoellLoggerStackSize,
  NeuroThemeNnoellLoggerMinimizedCount,
  NULL
};

// BOTTOM RIGHT
static const DzenFlags bottom_right_dzen_flags_ = {
  920, 1064, 1000, 16,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellColorBlack,
  'r', NeuroThemeNnoellFontDzenPanel, "onstart=lower", "-p"
};
static const LoggerFn bottom_right_loggers_[] = {
  NeuroThemeNnoellLoggerCpu,
  NeuroThemeNnoellLoggerRam,
  NeuroThemeNnoellLoggerTemperature,
  NeuroThemeNnoellLoggerWifiStrength,
  NULL
};

// DZENPANEL (DZENFLAGS, LOGGERS, SEP, REFRESH)
static const DzenPanel top_left_panel_     = { &top_left_dzen_flags_,     top_left_loggers_,     " ",  DZEN_ON_EVENT };
static const DzenPanel top_right_panel_    = { &top_right_dzen_flags_,    top_right_loggers_,    " ",  1             };
static const DzenPanel bottom_left_panel_  = { &bottom_left_dzen_flags_,  bottom_left_loggers_,  " ",  DZEN_ON_EVENT };
static const DzenPanel bottom_right_panel_ = { &bottom_right_dzen_flags_, bottom_right_loggers_, " ",  1             };

// DZENPANELSET
static const DzenPanel* dzen_panel_list_[] = {
    &top_left_panel_, &top_right_panel_, &bottom_left_panel_, &bottom_right_panel_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// KEYS (GRABBED TO THE ROOT WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// KEY (MOD, KEY, ACTIONCHAIN)
static const Key key00_ = {  // Launches the terminal (urxvtc)
  mod_mask_|ShiftMask, XK_Return,
  CHAIN(NeuroActionListSpawn, ARG_CMD(termcmd_))
};
static const Key key01_ = {  // Closes the current client
  mod_mask_, XK_c,
  CHAIN_NULL(NeuroActionListKillClient)
};
static const Key key02_ = {  // Selects the next client
  mod_mask_, XK_j,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorNext))
};
static const Key key03_ = {  // Selects the previous client
  mod_mask_, XK_k,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorPrev))
};
static const Key key04_ = {  // Selects the previous selected client
  mod_mask_, XK_Tab,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorOld))
};
static const Key key05_ = {  // Selects the master client
  mod_mask_, XK_m,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorHead))
};
static const Key key06_ = {  // Swaps the current client with the next one
  mod_mask_|ShiftMask, XK_j,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorNext))
};
static const Key key07_ = {  // Swaps the current client with the previous one
  mod_mask_|ShiftMask, XK_k,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorPrev))
};
static const Key key08_ = {  // Swaps the current client with the previous selected one
  mod_mask_|ShiftMask, XK_p,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorOld))
};
static const Key key09_ = {  // Swaps the current client with the master one
  mod_mask_|ShiftMask, XK_m,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorHead))
};
static const Key key10_ = {  // Selects the upper client by position
  mod_mask_, XK_Up,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorUp))
};
static const Key key11_ = {  // Selects the lower client by position
  mod_mask_, XK_Down,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorDown))
};
static const Key key12_ = {  // Selects the left client by position
  mod_mask_, XK_Left,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorLeft))
};
static const Key key13_ = {  // Selects the right client by position
  mod_mask_, XK_Right,
  CHAIN(NeuroActionListFocusCurrClient, ARG_CSF(NeuroClientSelectorRight))
};
static const Key key14_ = {  // Swaps the current client with the upper one by position
  mod_mask_|ShiftMask, XK_Up,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorUp))
};
static const Key key15_ = {  // Swaps the current client with the lower one by position
  mod_mask_|ShiftMask, XK_Down,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorDown))
};
static const Key key16_ = {  // Swaps the current client with the left one by position
  mod_mask_|ShiftMask, XK_Left,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorLeft))
};
static const Key key17_ = {  // Swaps the current client with the right one by position
  mod_mask_|ShiftMask, XK_Right,
  CHAIN(NeuroActionListSwapCurrClient, ARG_CSF(NeuroClientSelectorRight))
};
static const Key key18_ = {  // Cicles through all the layouts
  mod_mask_, XK_space,
  CHAIN_NULL(NeuroActionListChangeLayout)
};
static const Key key19_ = {  // Toggles the selected client into tile/free mode
  mod_mask_, XK_t,
  CHAIN_NULL(NeuroActionListToggleFreeCurrClient)
};
static const Key key20_ = {  // Toggles mirror mod to the current layout
  mod_mask_, XK_z,
  CHAIN(NeuroActionListToggleModLayout, ARG_LMOD(LayoutModMirror))
};
static const Key key21_ = {  // Toggles reflect X mod to the current layout
  mod_mask_, XK_x,
  CHAIN(NeuroActionListToggleModLayout, ARG_LMOD(LayoutModReflectX))
};
static const Key key22_ = {  // Toggles reflect Y mod to the current layout
  mod_mask_, XK_y,
  CHAIN(NeuroActionListToggleModLayout, ARG_LMOD(LayoutModReflectY))
};
static const Key key23_ = {  // Toggles the full layout
  mod_mask_, XK_f,
  CHAIN(NeuroActionListToggleLayout, ARG_INT(0))
};
static const Key key24_ = {  // Toggles the float layout
  mod_mask_, XK_o,
  CHAIN(NeuroActionListToggleLayout, ARG_INT(1))
};
static const Key key25_ = {  // Toggles the selected client into normal/fullscreen mode
  mod_mask_|ShiftMask, XK_f,
  CHAIN_NULL(NeuroActionListToggleFullscreenCurrClient)
};
static const Key key26_ = {  // Increases the number of clients in the master area
  mod_mask_, XK_comma,
  CHAIN_NULL(NeuroActionListIncreaseMasterLayout)
};
static const Key key27_ = {  // Reduces the number of clients in the master area
  mod_mask_, XK_period,
  CHAIN(NeuroActionListIncreaseMasterLayout, ARG_INT(-1))
};
static const Key key28_ = {  // Increases layout's master size
  mod_mask_, XK_l,
  CHAIN_NULL(NeuroActionListResizeMasterLayout)
};
static const Key key29_ = {  // Reduces layout's master size
  mod_mask_, XK_h,
  CHAIN(NeuroActionListResizeMasterLayout, ARG_FLOAT(-1.0f))
};
static const Key key30_ = {  // Changes to workspace 0
  mod_mask_, XK_1,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector0))
};
static const Key key31_ = {  // Changes to workspace 1
  mod_mask_, XK_2,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector1))
};
static const Key key32_ = {  // Changes to workspace 2
  mod_mask_, XK_3,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector2))
};
static const Key key33_ = {  // Changes to workspace 3
  mod_mask_, XK_4,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector3))
};
static const Key key34_ = {  // Changes to workspace 4
  mod_mask_, XK_5,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector4))
};
static const Key key35_ = {  // Changes to workspace 5
  mod_mask_, XK_6,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector5))
};
static const Key key36_ = {  // Changes to workspace 6
  mod_mask_, XK_7,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector6))
};
static const Key key37_ = {  // Changes to workspace 7
  mod_mask_, XK_8,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector7))
};
static const Key key38_ = {  // Changes to workspace 8
  mod_mask_, XK_9,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector8))
};
static const Key key39_ = {  // Changes to workspace 9
  mod_mask_, XK_0,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelector9))
};
static const Key key40_ = {  // Changes to the previous workspace
  ControlMask|Mod1Mask, XK_Left,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorPrev))
};
static const Key key41_ = {  // Changes to the next workspace
  ControlMask|Mod1Mask, XK_Right,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorNext))
};
static const Key key42_ = {  // Changes to the previous selected workspace
  mod_mask_|ShiftMask, XK_Tab,
  CHAIN(NeuroActionListChangeWorkspace, ARG_WSF(NeuroWorkspaceSelectorOld))
};
static const Key key43_ = {  // Sends the selected client to workspace 0
  mod_mask_|ShiftMask, XK_1,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector0))
};
static const Key key44_ = {  // Sends the selected client to workspace 1
  mod_mask_|ShiftMask, XK_2,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector1))
};
static const Key key45_ = {  // Sends the selected client to workspace 2
  mod_mask_|ShiftMask, XK_3,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector2))
};
static const Key key46_ = {  // Sends the selected client to workspace 3
  mod_mask_|ShiftMask, XK_4,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector3))
};
static const Key key47_ = {  // Sends the selected client to workspace 4
  mod_mask_|ShiftMask, XK_5,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector4))
};
static const Key key48_ = {  // Sends the selected client to workspace 5
  mod_mask_|ShiftMask, XK_6,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector5))
};
static const Key key49_ = {  // Sends the selected client to workspace 6
  mod_mask_|ShiftMask, XK_7,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector6))
};
static const Key key50_ = {  // Sends the selected client to workspace 7
  mod_mask_|ShiftMask, XK_8,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector7))
};
static const Key key51_ = {  // Sends the selected client to workspace 8
  mod_mask_|ShiftMask, XK_9,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector8))
};
static const Key key52_ = {  // Sends the selected client to workspace 9
  mod_mask_|ShiftMask, XK_0,
  CHAIN(NeuroActionListSendCurrClient, ARG_WSF(NeuroWorkspaceSelector9))
};
static const Key key53_ = {  // Sends the selected client to workspace 0 and follows it
  mod_mask_|ControlMask, XK_1,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector0))
};
static const Key key54_ = {  // Sends the selected client to workspace 1 and follows it
  mod_mask_|ControlMask, XK_2,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector1))
};
static const Key key55_ = {  // Sends the selected client to workspace 2 and follows it
  mod_mask_|ControlMask, XK_3,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector2))
};
static const Key key56_ = {  // Sends the selected client to workspace 3 and follows it
  mod_mask_|ControlMask, XK_4,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector3))
};
static const Key key57_ = {  // Sends the selected client to workspace 4 and follows it
  mod_mask_|ControlMask, XK_5,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector4))
};
static const Key key58_ = {  // Sends the selected client to workspace 5 and follows it
  mod_mask_|ControlMask, XK_6,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector5))
};
static const Key key59_ = {  // Sends the selected client to workspace 6 and follows it
  mod_mask_|ControlMask, XK_7,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector6))
};
static const Key key60_ = {  // Sends the selected client to workspace 7 and follows it
  mod_mask_|ControlMask, XK_8,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector7))
};
static const Key key61_ = {  // Sends the selected client to workspace 8 and follows it
  mod_mask_|ControlMask, XK_9,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector8))
};
static const Key key62_ = {  // Sends the selected client to workspace 9 and follows it
  mod_mask_|ControlMask, XK_0,
  CHAIN(NeuroActionListSendFollowCurrClient, ARG_WSF(NeuroWorkspaceSelector9))
};
static const Key key63_ = {  // Exits the window manager
  mod_mask_|ShiftMask, XK_q,
  CHAIN_NULL(NeuroActionListQuit)
};
static const Key key64_ = {  // Reloads the window manager
  mod_mask_, XK_q,
  CHAIN_NULL(NeuroActionListReload)
};
static const Key key65_ = {  // Launches the application launcher (gmrun)
  Mod1Mask, XK_F2,
  CHAIN(NeuroActionListSpawn, ARG_CMD(NeuroConfigDefaultLauncherCommand))
};
static const Key key66_ = {  // Resets the current layout to itds default values
  mod_mask_|ShiftMask, XK_space,
  CHAIN_NULL(NeuroActionListResetLayout)
};
static const Key key67_ = {  // Minimizes the selected client
  mod_mask_, XK_n,
  CHAIN_NULL(NeuroActionListMinimizeCurrClient)
};
static const Key key68_ = {  // Restores the last minimized client
  mod_mask_|ShiftMask, XK_n,
  CHAIN_NULL(NeuroActionListRestoreLastMinimized)
};
static const Key key69_ = {  // Sets the selected client into big center free mode
  mod_mask_|ShiftMask, XK_t,
  CHAIN(NeuroActionListFreeCurrClient, ARG_FSF(NeuroRuleFreeSetterBigCenter))
};
static const Key key70_ = {  // Toggles the scratchpad
  mod_mask_, XK_grave,
  CHAIN(NeuroActionListToggleScratchpad, ARG_CMD(nspcmd_))
};
static const Key key71_ = {  // Toggles the scratchpad
  mod_mask_, XK_masculine,
  CHAIN(NeuroActionListToggleScratchpad, ARG_CMD(nspcmd_))
};
static const Key key72_ = {  // Rises the volume
  Mod1Mask, XK_Up,
  CHAIN(NeuroActionListSpawn, ARG_CMD(volupcmd_))
};
static const Key key73_ = {  // Lowers the volume
  Mod1Mask, XK_Down,
  CHAIN(NeuroActionListSpawn, ARG_CMD(voldocmd_))
};
static const Key key74_ = {  // Jumps to the next song
  Mod1Mask, XK_Right,
  CHAIN(NeuroActionListSpawn, ARG_CMD(songncmd_))
};
static const Key key75_ = {  // Jumps to the previous song
  Mod1Mask, XK_Left,
  CHAIN(NeuroActionListSpawn, ARG_CMD(songpcmd_))
};

// KEYSET
static const Key* key_list_[] = {
  &key00_, &key01_, &key02_, &key03_, &key04_, &key05_, &key06_, &key07_, &key08_, &key09_,
  &key10_, &key11_, &key12_, &key13_, &key14_, &key15_, &key16_, &key17_, &key18_, &key19_,
  &key20_, &key21_, &key22_, &key23_, &key24_, &key25_, &key26_, &key27_, &key28_, &key29_,
  &key30_, &key31_, &key32_, &key33_, &key34_, &key35_, &key36_, &key37_, &key38_, &key39_,
  &key40_, &key41_, &key42_, &key43_, &key44_, &key45_, &key46_, &key47_, &key48_, &key49_,
  &key50_, &key51_, &key52_, &key53_, &key54_, &key55_, &key56_, &key57_, &key58_, &key59_,
  &key60_, &key61_, &key62_, &key63_, &key64_, &key65_, &key66_, &key67_, &key68_, &key69_,
  &key70_, &key71_, &key72_, &key73_, &key74_, &key75_, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// BUTTONS (GRABBED TO EACH WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// BUTTON (MOD, BUTTON, ACTIONCHAIN, UNWRAPONFOCUS)
static const Button button0_ = {
    NULL_MASK,           Button1, CHAIN_NULL(NeuroActionListFocusPtrClient),            true  };
static const Button button1_ = {
    mod_mask_,           Button1, CHAIN_NULL(NeuroActionListFreeMovePtrClient),         false };
static const Button button2_ = {
    mod_mask_,           Button2, CHAIN_NULL(NeuroActionListToggleFreePtrClient),       false };
static const Button button3_ = {
    mod_mask_,           Button3, CHAIN_NULL(NeuroActionListFreeResizePtrClient),       false };
static const Button button4_ = {
    mod_mask_|ShiftMask, Button1, CHAIN_NULL(NeuroActionListFloatMovePtrClient),        false };
static const Button button5_ = {
    mod_mask_|ShiftMask, Button2, CHAIN_NULL(NeuroActionListToggleFullscreenPtrClient), false };
static const Button button6_ = {
    mod_mask_|ShiftMask, Button3, CHAIN_NULL(NeuroActionListFloatResizePtrClient),      false };

// BUTTONSET
static const Button* button_list_[] = {
    &button0_, &button1_, &button2_, &button3_, &button4_, &button5_, &button6_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// CONFIGURATION
//----------------------------------------------------------------------------------------------------------------------

static const Configuration configuration_ = {
  CHAIN_NULL(init_action_list_),
  CHAIN_NULL(NeuroActionListStopCpuCalc),
  NeuroConfigDefaultNormalBorderColor,
  NeuroConfigDefaultCurrentBorderColor,
  NeuroConfigDefaultOldBorderColor,
  NeuroConfigDefaultFreeBorderColor,
  NeuroConfigDefaultUrgentBorderColor,
  NeuroConfigDefaultBorderWidth,
  NeuroConfigDefaultBorderGap,
  workspace_list_,
  rule_list_,
  dzen_panel_list_,  // Dzen must be installed
  key_list_,
  button_list_,
};


//----------------------------------------------------------------------------------------------------------------------
// MAIN
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  return NeuroWmRun(&configuration_);
  // return NeuroWmRun(NULL);
}

