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
static const char* termcmd_[] = { "/usr/bin/urxvt", NULL };
static const char* nspcmd_[] = { "/usr/bin/urxvt", "-name", NEURO_RULE_SCRATCHPAD_NAME, NULL };
static const char* xdefload_[] = { "/usr/bin/xrdb", "-load", "/home/julian/.config/xorg/Xdefaults", NULL };
static const char* wallcmd_[] = { "/usr/bin/feh", "--bg-scale", "/home/julian/Pictures/wallpapers/neurowm.png", NULL };
static const char* volupcmd_[] = { "/home/julian/bin/voldzen.sh", "+", "-d", NULL };
static const char* voldocmd_[] = { "/home/julian/bin/voldzen.sh", "-", "-d", NULL };
static const char* songncmd_[] = { "/usr/bin/mpc", "next", NULL };
static const char* songpcmd_[] = { "/usr/bin/mpc", "prev", NULL };


//----------------------------------------------------------------------------------------------------------------------
// ACTIONS
//----------------------------------------------------------------------------------------------------------------------

// ACTION (HANDLER, ARG)
static const NeuroAction action0_ = { NeuroActionHandlerChangeWmName, NEURO_ARG_STR("LG3D") };
static const NeuroAction action1_ = { NeuroActionHandlerSpawn, NEURO_ARG_CMD(xdefload_) };
static const NeuroAction action2_ = { NeuroActionHandlerSpawn, NEURO_ARG_CMD(wallcmd_) };

// ACTION LIST
static const NeuroAction* init_action_list_[] = { &action0_, &action1_, &action2_, &NeuroActionInitCpuCalc, NULL };


//----------------------------------------------------------------------------------------------------------------------
// LAYOUTS
//----------------------------------------------------------------------------------------------------------------------

// LAYOUTCONF (NAME, ARRANGERF, BORDERCOLORF, BORDERWIDTHF, BORDERGAPF, REGION (XYWH), MOD, FOLLOWMOUSE, ARGS)
static const NeuroLayoutConf tile_ = {
  "Tile",
  NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  NEURO_LAYOUT_MOD_NULL,
  true,
  {NEURO_ARG_IDX(1U), NEURO_ARG_FLOAT(0.5f), NEURO_ARG_FLOAT(0.03f), NEURO_ARG_NULL}
};
static const NeuroLayoutConf mirror_ = {
  "Mirr",
  NeuroLayoutArrangerTall,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  NEURO_LAYOUT_MOD_MIRROR,
  true,
  {NEURO_ARG_IDX(1U), NEURO_ARG_FLOAT(0.5f), NEURO_ARG_FLOAT(0.03f), NEURO_ARG_NULL}
};
static const NeuroLayoutConf grid_ = {
  "Grid",
  NeuroLayoutArrangerGrid,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  NEURO_LAYOUT_MOD_MIRROR|NEURO_LAYOUT_MOD_REFLECTX|NEURO_LAYOUT_MODE_REFLECTY,
  true,
  {NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL}
};
static const NeuroLayoutConf full_ = {
  "Full",
  NeuroLayoutArrangerFull,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterSmart,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  NEURO_LAYOUT_MOD_NULL,
  true,
  {NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL}
};
static const NeuroLayoutConf float_ = {
  "Float",
  NeuroLayoutArrangerFloat,
  NeuroClientColorSetterAll,
  NeuroClientBorderWidthSetterAlways,
  NeuroClientBorderGapSetterAlways,
  {0.0f, 0.0f, 1.0f, 1.0f},
  NEURO_LAYOUT_MOD_NULL,
  false,
  {NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL, NEURO_ARG_NULL}
};

// LAYOUT LIST
static const NeuroLayoutConf* layout_list_[] = { &tile_, &mirror_, &grid_, NULL };
static const NeuroLayoutConf* toggled_layout_list_[] = { &full_, &float_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// WORKSPACES
//----------------------------------------------------------------------------------------------------------------------

// WORKSPACE (NAME, LAYOUTS, TOGLAYOUTS)
static const NeuroWorkspace ws0_ = { "Terminal",    layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws1_ = { "Network",     layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws2_ = { "Development", layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws3_ = { "Graphics",    layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws4_ = { "Chatting",    layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws5_ = { "Video",       layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws6_ = { "Alternate1",  layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws7_ = { "Alternate2",  layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws8_ = { "Alternate3",  layout_list_, toggled_layout_list_ };
static const NeuroWorkspace ws9_ = { "Alternate4",  layout_list_, toggled_layout_list_ };

// WORKSPACE LIST
static const NeuroWorkspace* workspace_list_[] = {
    &ws0_, &ws1_, &ws2_, &ws3_, &ws4_, &ws5_, &ws6_, &ws7_, &ws8_, &ws9_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// RULES
//----------------------------------------------------------------------------------------------------------------------

// RULE (ClASS, NAME, TITLE, FULLSCREEN, FREE, FIXED, FIXSIZE, WS, FOLLOW)
static const NeuroRule rule00_ = {
  "URxvt", NEURO_RULE_SCRATCHPAD_NAME, "urxvt",
  false, NeuroRuleFreeSetterScratchpad, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const NeuroRule rule01_ = {
  NULL, NULL, "Firefox Preferences",
  false, NeuroRuleFreeSetterCenter, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const NeuroRule rule02_ = {
  NULL, NULL, "Buddy List",
  false, NeuroRuleFreeSetterNull, NEURO_FIXED_POSITION_LEFT, 0.2f, NeuroWorkspaceSelector4, false
};
static const NeuroRule rule03_ = {
  "Pidgin", NULL, NULL,
  false, NeuroRuleFreeSetterNull, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelector4, false
};
static const NeuroRule rule04_ = {
  "Firefox", NULL, NULL,
  false, NeuroRuleFreeSetterNull, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelector1, false
};
static const NeuroRule rule05_ = {
  "chromium", NULL, NULL,
  false, NeuroRuleFreeSetterNull, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelector1, false
};
static const NeuroRule rule06_ = {
  "MPlayer", NULL, NULL,
  false, NeuroRuleFreeSetterCenter, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const NeuroRule rule07_ = {
  "Gmrun", NULL, NULL,
  false, NeuroRuleFreeSetterCenter, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const NeuroRule rule08_ = {
  "Xephyr", NULL, NULL,
  true, NeuroRuleFreeSetterBigCenter, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const NeuroRule rule09_ = {
  "Transmission-gtk", NULL, NULL,
  false, NeuroRuleFreeSetterNull, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelector8, false
};
static const NeuroRule rule10_ = {
  "PPSSPPSDL", "PPSSPPSDL", NULL,
  false, NeuroRuleFreeSetterCenter, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};
static const NeuroRule rule11_ = {
  "XCalc", NULL, NULL,
  false, NeuroRuleFreeSetterCenter, NEURO_FIXED_POSITION_NULL, 0.0f, NeuroWorkspaceSelectorCurr, false
};

// RULE LIST
static const NeuroRule* rule_list_[] = {
  &rule00_, &rule01_, &rule02_, &rule03_, &rule04_, &rule05_, &rule06_, &rule07_, &rule08_, &rule09_,
  &rule10_, &rule11_, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// DZEN PANELS
//----------------------------------------------------------------------------------------------------------------------

// TOP LEFT
static const NeuroDzenFlags top_left_dzen_flags0_ = {
  0, 0, 1200, 16,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  'l', NEURO_THEME_NNOELL_DZEN_FONT, "onstart=lower", "-p"
};
static const NeuroDzenLoggerFn top_left_loggers0_[] = {
  NeuroThemeNnoellLoggerMonitorCurrLayout,
  NeuroThemeNnoellLoggerMonitorCurrLayoutMod,
  NeuroThemeNnoellLoggerMonitorWorkspace,
  NeuroThemeNnoellLoggerMonitorCurrTitle,
  NULL
};
static const NeuroDzenFlags top_left_dzen_flags1_ = {
  0, 0, 1500, 16,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  'l', NEURO_THEME_NNOELL_DZEN_FONT, "onstart=lower", "-p"
};
static const NeuroDzenLoggerFn top_left_loggers1_[] = {
  NeuroThemeNnoellLoggerMonitorCurrLayout,
  NeuroThemeNnoellLoggerMonitorCurrLayoutMod,
  NeuroThemeNnoellLoggerMonitorWorkspace,
  NeuroThemeNnoellLoggerMonitorCurrTitle,
  NULL
};

// TOP RIGHT
static const NeuroDzenFlags top_right_dzen_flags0_ = {
  1200, 0, 240, 16,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  'r', NEURO_THEME_NNOELL_DZEN_FONT, "onstart=lower", "-p"
};
static const NeuroDzenLoggerFn top_right_loggers0_[] = {
  NeuroThemeNnoellLoggerMonitorStackSize,
  NeuroThemeNnoellLoggerMonitorNumMinimized,
  NULL
};
static const NeuroDzenFlags top_right_dzen_flags1_ = {
  1500, 0, 420, 16,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  'r', NEURO_THEME_NNOELL_DZEN_FONT, "onstart=lower", "-p"
};
static const NeuroDzenLoggerFn top_right_loggers1_[] = {
  NeuroThemeNnoellLoggerUptime,
  NeuroThemeNnoellLoggerDateTime,
  NULL
};

// BOTTOM LEFT
static const NeuroDzenFlags bottom_left_dzen_flags1_ = {
  0, 1064, 920, 16,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  'l', NEURO_THEME_NNOELL_DZEN_FONT, "onstart=lower", "-p"
};
static const NeuroDzenLoggerFn bottom_left_loggers1_[] = {
  NeuroThemeNnoellLoggerWorkspaceList,
  NeuroThemeNnoellLoggerMonitorStackSize,
  NeuroThemeNnoellLoggerMonitorNumMinimized,
  NeuroThemeNnoellLoggerScreen,
  NeuroThemeNnoellLoggerMonitorList,
  NULL
};

// BOTTOM RIGHT
static const NeuroDzenFlags bottom_right_dzen_flags1_ = {
  920, 1064, 1000, 16,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  'r', NEURO_THEME_NNOELL_DZEN_FONT, "onstart=lower", "-p"
};
static const NeuroDzenLoggerFn bottom_right_loggers1_[] = {
  NeuroThemeNnoellLoggerCpu,
  NeuroThemeNnoellLoggerRam,
  NeuroThemeNnoellLoggerTemperature,
  NeuroThemeNnoellLoggerWifiStrength,
  NULL
};

// DZENPANEL (DZENFLAGS, LOGGERS, SEP, REFRESH)
static const NeuroDzenPanel top_left_panel0_     = {
    &top_left_dzen_flags0_,     top_left_loggers0_,     " ",  NEURO_DZEN_REFRESH_ON_EVENT };
static const NeuroDzenPanel top_left_panel1_     = {
    &top_left_dzen_flags1_,     top_left_loggers1_,     " ",  NEURO_DZEN_REFRESH_ON_EVENT };
static const NeuroDzenPanel top_right_panel0_    = {
    &top_right_dzen_flags0_,    top_right_loggers0_,    " ",  NEURO_DZEN_REFRESH_ON_EVENT };
static const NeuroDzenPanel top_right_panel1_    = {
    &top_right_dzen_flags1_,    top_right_loggers1_,    " ",  1                           };
static const NeuroDzenPanel bottom_left_panel1_  = {
    &bottom_left_dzen_flags1_,  bottom_left_loggers1_,  " ",  NEURO_DZEN_REFRESH_ON_EVENT };
static const NeuroDzenPanel bottom_right_panel1_ = {
    &bottom_right_dzen_flags1_, bottom_right_loggers1_, " ",  1                           };

// DZENPANEL LIST
static const NeuroDzenPanel* dzen_panel_list0_[] = {
    &top_left_panel0_, &top_right_panel0_, NULL };
static const NeuroDzenPanel* dzen_panel_list1_[] = {
    &top_left_panel1_, &top_right_panel1_, &bottom_left_panel1_, &bottom_right_panel1_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// MONITORS
//----------------------------------------------------------------------------------------------------------------------

// MONITORCONF (NAME, DEFAULT_WORKSPACE, GAPS (UDLR), DZENPANELS)
static const NeuroMonitorConf m0_ = { "DVI",  1U, {16, 0, 0, 0},  dzen_panel_list0_ };
static const NeuroMonitorConf m1_ = { "HDMI", 0U, {16, 16, 0, 0}, dzen_panel_list1_ };

// MONITORCONF LIST
const NeuroMonitorConf* monitor_list_[] = { &m0_, &m1_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// KEYS (GRABBED TO THE ROOT WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// KEY (MOD, KEY, ACTIONCHAIN)
static const NeuroKey key00_ = {  // Launches the terminal (urxvtc)
  mod_mask_|ShiftMask, XK_Return,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(termcmd_))
};
static const NeuroKey key01_ = {  // Closes the current client
  mod_mask_, XK_c,
  NEURO_CHAIN_NULL(NeuroActionListKillClient)
};
static const NeuroKey key02_ = {  // Selects the next client
  mod_mask_, XK_j,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext))
};
static const NeuroKey key03_ = {  // Selects the previous client
  mod_mask_, XK_k,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorPrev))
};
static const NeuroKey key04_ = {  // Selects the previous selected client
  mod_mask_, XK_Tab,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorOld))
};
static const NeuroKey key05_ = {  // Selects the master client
  mod_mask_, XK_m,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorHead))
};
static const NeuroKey key06_ = {  // Swaps the current client with the next one
  mod_mask_|ShiftMask, XK_j,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext))
};
static const NeuroKey key07_ = {  // Swaps the current client with the previous one
  mod_mask_|ShiftMask, XK_k,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorPrev))
};
static const NeuroKey key08_ = {  // Swaps the current client with the previous selected one
  mod_mask_|ShiftMask, XK_p,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorOld))
};
static const NeuroKey key09_ = {  // Swaps the current client with the master one
  mod_mask_|ShiftMask, XK_m,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorHead))
};
static const NeuroKey key10_ = {  // Selects the upper client by position
  mod_mask_, XK_Up,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorUpper))
};
static const NeuroKey key11_ = {  // Selects the lower client by position
  mod_mask_, XK_Down,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLower))
};
static const NeuroKey key12_ = {  // Selects the left client by position
  mod_mask_, XK_Left,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLeft))
};
static const NeuroKey key13_ = {  // Selects the right client by position
  mod_mask_, XK_Right,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorRight))
};
static const NeuroKey key14_ = {  // Swaps the current client with the upper one by position
  mod_mask_|ShiftMask, XK_Up,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorUpper))
};
static const NeuroKey key15_ = {  // Swaps the current client with the lower one by position
  mod_mask_|ShiftMask, XK_Down,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLower))
};
static const NeuroKey key16_ = {  // Swaps the current client with the left one by position
  mod_mask_|ShiftMask, XK_Left,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLeft))
};
static const NeuroKey key17_ = {  // Swaps the current client with the right one by position
  mod_mask_|ShiftMask, XK_Right,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorRight))
};
static const NeuroKey key18_ = {  // Cicles through all the layouts
  mod_mask_, XK_space,
  NEURO_CHAIN_NULL(NeuroActionListChangeLayout)
};
static const NeuroKey key19_ = {  // Toggles the selected client into tile/free mode
  mod_mask_, XK_t,
  NEURO_CHAIN_NULL(NeuroActionListToggleFreeCurrClient)
};
static const NeuroKey key20_ = {  // Toggles mirror mod to the current layout
  mod_mask_, XK_z,
  NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_MIRROR))
};
static const NeuroKey key21_ = {  // Toggles reflect X mod to the current layout
  mod_mask_, XK_x,
  NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_REFLECTX))
};
static const NeuroKey key22_ = {  // Toggles reflect Y mod to the current layout
  mod_mask_, XK_y,
  NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MODE_REFLECTY))
};
static const NeuroKey key23_ = {  // Toggles the full layout
  mod_mask_, XK_f,
  NEURO_CHAIN(NeuroActionListToggleLayout, NEURO_ARG_IDX(0U))
};
static const NeuroKey key24_ = {  // Toggles the float layout
  mod_mask_, XK_o,
  NEURO_CHAIN(NeuroActionListToggleLayout, NEURO_ARG_IDX(1U))
};
static const NeuroKey key25_ = {  // Toggles the selected client into normal/fullscreen mode
  mod_mask_|ShiftMask, XK_f,
  NEURO_CHAIN_NULL(NeuroActionListToggleFullscreenCurrClient)
};
static const NeuroKey key26_ = {  // Increases the number of clients in the master area
  mod_mask_, XK_comma,
  NEURO_CHAIN_NULL(NeuroActionListIncreaseMasterLayout)
};
static const NeuroKey key27_ = {  // Reduces the number of clients in the master area
  mod_mask_, XK_period,
  NEURO_CHAIN(NeuroActionListIncreaseMasterLayout, NEURO_ARG_INT(-1))
};
static const NeuroKey key28_ = {  // Increases layout's master size
  mod_mask_, XK_l,
  NEURO_CHAIN_NULL(NeuroActionListResizeMasterLayout)
};
static const NeuroKey key29_ = {  // Reduces layout's master size
  mod_mask_, XK_h,
  NEURO_CHAIN(NeuroActionListResizeMasterLayout, NEURO_ARG_FLOAT(-1.0f))
};
static const NeuroKey key30_ = {  // Changes to workspace 0
  mod_mask_, XK_1,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector0))
};
static const NeuroKey key31_ = {  // Changes to workspace 1
  mod_mask_, XK_2,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector1))
};
static const NeuroKey key32_ = {  // Changes to workspace 2
  mod_mask_, XK_3,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector2))
};
static const NeuroKey key33_ = {  // Changes to workspace 3
  mod_mask_, XK_4,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector3))
};
static const NeuroKey key34_ = {  // Changes to workspace 4
  mod_mask_, XK_5,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector4))
};
static const NeuroKey key35_ = {  // Changes to workspace 5
  mod_mask_, XK_6,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector5))
};
static const NeuroKey key36_ = {  // Changes to workspace 6
  mod_mask_, XK_7,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector6))
};
static const NeuroKey key37_ = {  // Changes to workspace 7
  mod_mask_, XK_8,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector7))
};
static const NeuroKey key38_ = {  // Changes to workspace 8
  mod_mask_, XK_9,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector8))
};
static const NeuroKey key39_ = {  // Changes to workspace 9
  mod_mask_, XK_0,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector9))
};
static const NeuroKey key40_ = {  // Changes to the previous workspace
  ControlMask|Mod1Mask, XK_Left,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorPrev))
};
static const NeuroKey key41_ = {  // Changes to the next workspace
  ControlMask|Mod1Mask, XK_Right,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorNext))
};
static const NeuroKey key42_ = {  // Changes to the previous selected workspace
  mod_mask_|ShiftMask, XK_Tab,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorOld))
};
static const NeuroKey key43_ = {  // Sends the selected client to workspace 0
  mod_mask_|ShiftMask, XK_1,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector0))
};
static const NeuroKey key44_ = {  // Sends the selected client to workspace 1
  mod_mask_|ShiftMask, XK_2,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector1))
};
static const NeuroKey key45_ = {  // Sends the selected client to workspace 2
  mod_mask_|ShiftMask, XK_3,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector2))
};
static const NeuroKey key46_ = {  // Sends the selected client to workspace 3
  mod_mask_|ShiftMask, XK_4,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector3))
};
static const NeuroKey key47_ = {  // Sends the selected client to workspace 4
  mod_mask_|ShiftMask, XK_5,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector4))
};
static const NeuroKey key48_ = {  // Sends the selected client to workspace 5
  mod_mask_|ShiftMask, XK_6,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector5))
};
static const NeuroKey key49_ = {  // Sends the selected client to workspace 6
  mod_mask_|ShiftMask, XK_7,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector6))
};
static const NeuroKey key50_ = {  // Sends the selected client to workspace 7
  mod_mask_|ShiftMask, XK_8,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector7))
};
static const NeuroKey key51_ = {  // Sends the selected client to workspace 8
  mod_mask_|ShiftMask, XK_9,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector8))
};
static const NeuroKey key52_ = {  // Sends the selected client to workspace 9
  mod_mask_|ShiftMask, XK_0,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector9))
};
static const NeuroKey key53_ = {  // Sends the selected client to workspace 0 and follows it
  mod_mask_|ControlMask, XK_1,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector0))
};
static const NeuroKey key54_ = {  // Sends the selected client to workspace 1 and follows it
  mod_mask_|ControlMask, XK_2,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector1))
};
static const NeuroKey key55_ = {  // Sends the selected client to workspace 2 and follows it
  mod_mask_|ControlMask, XK_3,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector2))
};
static const NeuroKey key56_ = {  // Sends the selected client to workspace 3 and follows it
  mod_mask_|ControlMask, XK_4,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector3))
};
static const NeuroKey key57_ = {  // Sends the selected client to workspace 4 and follows it
  mod_mask_|ControlMask, XK_5,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector4))
};
static const NeuroKey key58_ = {  // Sends the selected client to workspace 5 and follows it
  mod_mask_|ControlMask, XK_6,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector5))
};
static const NeuroKey key59_ = {  // Sends the selected client to workspace 6 and follows it
  mod_mask_|ControlMask, XK_7,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector6))
};
static const NeuroKey key60_ = {  // Sends the selected client to workspace 7 and follows it
  mod_mask_|ControlMask, XK_8,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector7))
};
static const NeuroKey key61_ = {  // Sends the selected client to workspace 8 and follows it
  mod_mask_|ControlMask, XK_9,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector8))
};
static const NeuroKey key62_ = {  // Sends the selected client to workspace 9 and follows it
  mod_mask_|ControlMask, XK_0,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector9))
};
static const NeuroKey key63_ = {  // Exits the window manager
  mod_mask_|ShiftMask, XK_q,
  NEURO_CHAIN_NULL(NeuroActionListQuit)
};
static const NeuroKey key64_ = {  // Reloads the window manager
  mod_mask_, XK_q,
  NEURO_CHAIN_NULL(NeuroActionListReload)
};
static const NeuroKey key65_ = {  // Launches the application launcher (gmrun)
  Mod1Mask, XK_F2,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(NeuroConfigDefaultLauncherCommand))
};
static const NeuroKey key66_ = {  // Resets the current layout to itds default values
  mod_mask_|ShiftMask, XK_space,
  NEURO_CHAIN_NULL(NeuroActionListResetLayout)
};
static const NeuroKey key67_ = {  // Minimizes the selected client
  mod_mask_, XK_n,
  NEURO_CHAIN_NULL(NeuroActionListMinimizeCurrClient)
};
static const NeuroKey key68_ = {  // Restores the last minimized client
  mod_mask_|ShiftMask, XK_n,
  NEURO_CHAIN_NULL(NeuroActionListRestoreLastMinimized)
};
static const NeuroKey key69_ = {  // Sets the selected client into big center free mode
  mod_mask_|ShiftMask, XK_t,
  NEURO_CHAIN(NeuroActionListFreeCurrClient, NEURO_ARG_FSF(NeuroRuleFreeSetterBigCenter))
};
static const NeuroKey key70_ = {  // Toggles the scratchpad
  mod_mask_, XK_grave,
  NEURO_CHAIN(NeuroActionListToggleScratchpad, NEURO_ARG_CMD(nspcmd_))
};
static const NeuroKey key71_ = {  // Toggles the scratchpad
  mod_mask_, XK_masculine,
  NEURO_CHAIN(NeuroActionListToggleScratchpad, NEURO_ARG_CMD(nspcmd_))
};
static const NeuroKey key72_ = {  // Rises the volume
  Mod1Mask, XK_Up,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(volupcmd_))
};
static const NeuroKey key73_ = {  // Lowers the volume
  Mod1Mask, XK_Down,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(voldocmd_))
};
static const NeuroKey key74_ = {  // Jumps to the next song
  Mod1Mask, XK_Right,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(songncmd_))
};
static const NeuroKey key75_ = {  // Jumps to the previous song
  Mod1Mask, XK_Left,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(songpcmd_))
};
static const NeuroKey key76_ = {  // Selects the previous monitor
  mod_mask_|ControlMask, XK_Right,
  NEURO_CHAIN(NeuroActionListSelectMonitor, NEURO_ARG_MSF(NeuroMonitorSelectorPrev))
};
static const NeuroKey key77_ = {  // Selects the next monitor
  mod_mask_|ControlMask, XK_Left,
  NEURO_CHAIN(NeuroActionListSelectMonitor, NEURO_ARG_MSF(NeuroMonitorSelectorNext))
};

// KEY LIST
static const NeuroKey* key_list_[] = {
  &key00_, &key01_, &key02_, &key03_, &key04_, &key05_, &key06_, &key07_, &key08_, &key09_,
  &key10_, &key11_, &key12_, &key13_, &key14_, &key15_, &key16_, &key17_, &key18_, &key19_,
  &key20_, &key21_, &key22_, &key23_, &key24_, &key25_, &key26_, &key27_, &key28_, &key29_,
  &key30_, &key31_, &key32_, &key33_, &key34_, &key35_, &key36_, &key37_, &key38_, &key39_,
  &key40_, &key41_, &key42_, &key43_, &key44_, &key45_, &key46_, &key47_, &key48_, &key49_,
  &key50_, &key51_, &key52_, &key53_, &key54_, &key55_, &key56_, &key57_, &key58_, &key59_,
  &key60_, &key61_, &key62_, &key63_, &key64_, &key65_, &key66_, &key67_, &key68_, &key69_,
  &key70_, &key71_, &key72_, &key73_, &key74_, &key75_, &key76_, &key77_, NULL
};


//----------------------------------------------------------------------------------------------------------------------
// BUTTONS (GRABBED TO EACH WINDOW)
//----------------------------------------------------------------------------------------------------------------------

// BUTTON (MOD, BUTTON, ACTIONCHAIN, UNWRAPONFOCUS)
static const NeuroButton button0_ = {
  NEURO_NULL_MASK, Button1,
  NEURO_CHAIN_NULL(NeuroActionListFocusPtrClient), true
};
static const NeuroButton button1_ = {
  mod_mask_, Button1,
  NEURO_CHAIN_NULL(NeuroActionListFreeMovePtrClient), false
};
static const NeuroButton button2_ = {
  mod_mask_, Button2,
  NEURO_CHAIN_NULL(NeuroActionListToggleFreePtrClient), false
};
static const NeuroButton button3_ = {
  mod_mask_, Button3,
  NEURO_CHAIN_NULL(NeuroActionListFreeResizePtrClient), false
};
static const NeuroButton button4_ = {
  mod_mask_|ShiftMask, Button1,
  NEURO_CHAIN_NULL(NeuroActionListFloatMovePtrClient), false
};
static const NeuroButton button5_ = {
  mod_mask_|ShiftMask, Button2,
  NEURO_CHAIN_NULL(NeuroActionListToggleFullscreenPtrClient), false
};
static const NeuroButton button6_ = {
  mod_mask_|ShiftMask, Button3,
  NEURO_CHAIN_NULL(NeuroActionListFloatResizePtrClient), false
};

// BUTTON LIST
static const NeuroButton* button_list_[] = {
    &button0_, &button1_, &button2_, &button3_, &button4_, &button5_, &button6_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// CONFIGURATION
//----------------------------------------------------------------------------------------------------------------------

static const NeuroConfiguration configuration_ = {
  NEURO_CHAIN_NULL(init_action_list_),
  NEURO_CHAIN_NULL(NeuroActionListStopCpuCalc),
  NEURO_CONFIG_DEFAULT_NORMAL_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_CURRENT_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_OLD_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_FREE_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_URGENT_BORDER_COLOR,
  NEURO_CONFIG_DEFAULT_BORDER_WIDTH,
  NEURO_CONFIG_DEFAULT_BORDER_GAP,
  monitor_list_,
  workspace_list_,
  rule_list_,
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

