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
  "Launches the terminal (xterm)",
  Mod1Mask|ShiftMask, XK_Return,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(NeuroConfigDefaultTerminalCommand))
};
static const NeuroKey key01_ = {
  "Closes the current client",
  Mod1Mask, XK_c,
  NEURO_CHAIN_NULL(NeuroActionListKillClient)
};
static const NeuroKey key02_ = {
  "Selects the next client",
  Mod1Mask, XK_j,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext))
};
static const NeuroKey key03_ = {
  "Selects the previous client",
  Mod1Mask, XK_k,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorPrev))
};
static const NeuroKey key04_ = {
  "Selects the previously selected client",
  Mod1Mask, XK_Tab,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorOld))
};
static const NeuroKey key05_ = {
  "Selects the master client",
  Mod1Mask, XK_m,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorHead))
};
static const NeuroKey key06_ = {
  "Swaps the current client with the next one",
  Mod1Mask|ShiftMask, XK_j,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorNext))
};
static const NeuroKey key07_ = {
  "Swaps the current client with the previous one",
  Mod1Mask|ShiftMask, XK_k,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorPrev))
};
static const NeuroKey key08_ = {
  "Swaps the current client with the previously selected one",
  Mod1Mask|ShiftMask, XK_p,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorOld))
};
static const NeuroKey key09_ = {
  "Swaps the current client with the master one",
  Mod1Mask|ShiftMask, XK_m,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorHead))
};
static const NeuroKey key10_ = {
  "Selects the upper client by position",
  Mod1Mask, XK_Up,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorUpper))
};
static const NeuroKey key11_ = {
  "Selects the lower client by position",
  Mod1Mask, XK_Down,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLower))
};
static const NeuroKey key12_ = {
  "Selects the left client by position",
  Mod1Mask, XK_Left,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLeft))
};
static const NeuroKey key13_ = {
  "Selects the right client by position",
  Mod1Mask, XK_Right,
  NEURO_CHAIN(NeuroActionListFocusCurrClient, NEURO_ARG_CSF(NeuroClientSelectorRight))
};
static const NeuroKey key14_ = {
  "Swaps the current client with the upper one by position",
  Mod1Mask|ShiftMask, XK_Up,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorUpper))
};
static const NeuroKey key15_ = {
  "Swaps the current client with the lower one by position",
  Mod1Mask|ShiftMask, XK_Down,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLower))
};
static const NeuroKey key16_ = {
  "Swaps the current client with the left one by position",
  Mod1Mask|ShiftMask, XK_Left,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorLeft))
};
static const NeuroKey key17_ = {
  "Swaps the current client with the right one by position",
  Mod1Mask|ShiftMask, XK_Right,
  NEURO_CHAIN(NeuroActionListSwapCurrClient, NEURO_ARG_CSF(NeuroClientSelectorRight))
};
static const NeuroKey key18_ = {
  "Cicles through all the layouts",
  Mod1Mask, XK_space,
  NEURO_CHAIN_NULL(NeuroActionListChangeLayout)
};
static const NeuroKey key19_ = {
  "Toggles the selected client into tile/free mode",
  Mod1Mask, XK_t,
  NEURO_CHAIN_NULL(NeuroActionListToggleFreeCurrClient)
};
static const NeuroKey key20_ = {
  "Toggles mirror mod to the current layout",
  Mod1Mask, XK_z,
  NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_MIRROR))
};
static const NeuroKey key21_ = {
  "Toggles reflect X mod to the current layout",
  Mod1Mask, XK_x,
  NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_REFLECTX))
};
static const NeuroKey key22_ = {
  "Toggles reflect Y mod to the current layout",
  Mod1Mask, XK_y,
  NEURO_CHAIN(NeuroActionListToggleModLayout, NEURO_ARG_LMOD(NEURO_LAYOUT_MOD_REFLECTY))
};
static const NeuroKey key23_ = {
  "Toggles the full layout",
  Mod1Mask, XK_f,
  NEURO_CHAIN(NeuroActionListToggleLayout, NEURO_ARG_IDX(0U))
};
static const NeuroKey key24_ = {
  "Toggles the float layout",
  Mod1Mask, XK_o,
  NEURO_CHAIN(NeuroActionListToggleLayout, NEURO_ARG_IDX(1U))
};
static const NeuroKey key25_ = {
  "Toggles the selected client into normal/fullscreen mode",
  Mod1Mask|ShiftMask, XK_f,
  NEURO_CHAIN_NULL(NeuroActionListToggleFullscreenCurrClient)
};
static const NeuroKey key26_ = {
  "Increases the number of clients in the master area",
  Mod1Mask, XK_comma,
  NEURO_CHAIN_NULL(NeuroActionListIncreaseMasterLayout)
};
static const NeuroKey key27_ = {
  "Reduces the number of clients in the master area",
  Mod1Mask, XK_period,
  NEURO_CHAIN(NeuroActionListIncreaseMasterLayout, NEURO_ARG_INT(-1))
};
static const NeuroKey key28_ = {
  "Increases layout's master size",
  Mod1Mask, XK_l,
  NEURO_CHAIN_NULL(NeuroActionListResizeMasterLayout)
};
static const NeuroKey key29_ = {
  "Reduces layout's master size",
  Mod1Mask, XK_h,
  NEURO_CHAIN(NeuroActionListResizeMasterLayout, NEURO_ARG_FLOAT(-1.0f))
};
static const NeuroKey key30_ = {
  "Changes to workspace 0",
  Mod1Mask, XK_1,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector0))};
static const NeuroKey key31_ = {
  "Changes to workspace 1",
  Mod1Mask, XK_2,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector1))
};
static const NeuroKey key32_ = {
  "Changes to workspace 2",
  Mod1Mask, XK_3,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector2))
};
static const NeuroKey key33_ = {
  "Changes to workspace 3",
  Mod1Mask, XK_4,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector3))
};
static const NeuroKey key34_ = {
  "Changes to workspace 4",
  Mod1Mask, XK_5,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector4))
};
static const NeuroKey key35_ = {
  "Changes to workspace 5",
  Mod1Mask, XK_6,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector5))
};
static const NeuroKey key36_ = {
  "Changes to workspace 6",
  Mod1Mask, XK_7,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector6))
};
static const NeuroKey key37_ = {
  "Changes to workspace 7",
  Mod1Mask, XK_8,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector7))
};
static const NeuroKey key38_ = {
  "Changes to workspace 8",
  Mod1Mask, XK_9,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector8))
};
static const NeuroKey key39_ = {
  "Changes to workspace 9",
  Mod1Mask, XK_0,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelector9))
};
static const NeuroKey key40_ = {
  "Changes to the previous workspace",
  ControlMask|Mod1Mask, XK_Left,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorPrev))
};
static const NeuroKey key41_ = {
  "Changes to the next workspace",
  ControlMask|Mod1Mask, XK_Right,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorNext))
};
static const NeuroKey key42_ = {
  "Changes to the previously selected workspace",
  Mod1Mask|ShiftMask, XK_Tab,
  NEURO_CHAIN(NeuroActionListChangeWorkspace, NEURO_ARG_WSF(NeuroWorkspaceSelectorOld))
};
static const NeuroKey key43_ = {
  "Sends the selected client to workspace 0",
  Mod1Mask|ShiftMask, XK_1,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector0))
};
static const NeuroKey key44_ = {
  "Sends the selected client to workspace 1",
  Mod1Mask|ShiftMask, XK_2,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector1))
};
static const NeuroKey key45_ = {
  "Sends the selected client to workspace 2",
  Mod1Mask|ShiftMask, XK_3,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector2))
};
static const NeuroKey key46_ = {
  "Sends the selected client to workspace 3",
  Mod1Mask|ShiftMask, XK_4,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector3))
};
static const NeuroKey key47_ = {
  "Sends the selected client to workspace 4",
  Mod1Mask|ShiftMask, XK_5,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector4))
};
static const NeuroKey key48_ = {
  "Sends the selected client to workspace 5",
  Mod1Mask|ShiftMask, XK_6,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector5))
};
static const NeuroKey key49_ = {
  "Sends the selected client to workspace 6",
  Mod1Mask|ShiftMask, XK_7,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector6))
};
static const NeuroKey key50_ = {
  "Sends the selected client to workspace 7",
  Mod1Mask|ShiftMask, XK_8,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector7))
};
static const NeuroKey key51_ = {
  "Sends the selected client to workspace 8",
  Mod1Mask|ShiftMask, XK_9,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector8))
};
static const NeuroKey key52_ = {
  "Sends the selected client to workspace 9",
  Mod1Mask|ShiftMask, XK_0,
  NEURO_CHAIN(NeuroActionListSendCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector9))
};
static const NeuroKey key53_ = {
  "Sends the selected client to workspace 0 and follows it",
  Mod1Mask|ControlMask, XK_1,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector0))
};
static const NeuroKey key54_ = {
  "Sends the selected client to workspace 1 and follows it",
  Mod1Mask|ControlMask, XK_2,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector1))
};
static const NeuroKey key55_ = {
  "Sends the selected client to workspace 2 and follows it",
  Mod1Mask|ControlMask, XK_3,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector2))
};
static const NeuroKey key56_ = {
  "Sends the selected client to workspace 3 and follows it",
  Mod1Mask|ControlMask, XK_4,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector3))
};
static const NeuroKey key57_ = {
  "Sends the selected client to workspace 4 and follows it",
  Mod1Mask|ControlMask, XK_5,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector4))
};
static const NeuroKey key58_ = {
  "Sends the selected client to workspace 5 and follows it",
  Mod1Mask|ControlMask, XK_6,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector5))
};
static const NeuroKey key59_ = {
  "Sends the selected client to workspace 6 and follows it",
  Mod1Mask|ControlMask, XK_7,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector6))
};
static const NeuroKey key60_ = {
  "Sends the selected client to workspace 7 and follows it",
  Mod1Mask|ControlMask, XK_8,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector7))
};
static const NeuroKey key61_ = {
  "Sends the selected client to workspace 8 and follows it",
  Mod1Mask|ControlMask, XK_9,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector8))
};
static const NeuroKey key62_ = {
  "Sends the selected client to workspace 9 and follows it",
  Mod1Mask|ControlMask, XK_0,
  NEURO_CHAIN(NeuroActionListSendFollowCurrClient, NEURO_ARG_WSF(NeuroWorkspaceSelector9))
};
static const NeuroKey key63_ = {
  "Exits neurowm",
  Mod1Mask|ShiftMask, XK_q,
  NEURO_CHAIN_NULL(NeuroActionListQuit)
};
static const NeuroKey key64_ = {
  "Reloads neurowm",
  Mod1Mask, XK_q,
  NEURO_CHAIN_NULL(NeuroActionListReload)
};
static const NeuroKey key65_ = {
  "Launches the application launcher (gmrun)",
  Mod1Mask, XK_F2,
  NEURO_CHAIN(NeuroActionListSpawn, NEURO_ARG_CMD(NeuroConfigDefaultLauncherCommand))
};
static const NeuroKey key66_ = {
  "Resets the current layout to its default values",
  Mod1Mask|ShiftMask, XK_space,
  NEURO_CHAIN_NULL(NeuroActionListResetLayout)
};
static const NeuroKey key67_ = {
  "Minimizes the selected client",
  Mod1Mask, XK_n,
  NEURO_CHAIN_NULL(NeuroActionListMinimizeCurrClient)
};
static const NeuroKey key68_ = {
  "Restores the last minimized client",
  Mod1Mask|ShiftMask, XK_n,
  NEURO_CHAIN_NULL(NeuroActionListRestoreLastMinimized)
};

// NeuroButton
static const NeuroButton button0_ = {
  "Focuses the client under the pointer",
  NEURO_NULL_MASK, Button1,
  NEURO_CHAIN_NULL(NeuroActionListFocusPtrClient), true
};
static const NeuroButton button1_ = {
  "Moves in free mode the client under the pointer",
  Mod1Mask, Button1,
  NEURO_CHAIN_NULL(NeuroActionListFreeMovePtrClient), false
};
static const NeuroButton button2_ = {
  "Toggles free/tiled mode the client under the pointer",
  Mod1Mask, Button2,
  NEURO_CHAIN_NULL(NeuroActionListToggleFreePtrClient), false
};
static const NeuroButton button3_ = {
  "Resizes in free mode the client under the pointer",
  Mod1Mask, Button3,
  NEURO_CHAIN_NULL(NeuroActionListFreeResizePtrClient), false
};
static const NeuroButton button4_ = {
  "Moves in float mode the client under the pointer",
  Mod1Mask|ShiftMask, Button1,
  NEURO_CHAIN_NULL(NeuroActionListFloatMovePtrClient), false
};
static const NeuroButton button5_ = {
  "Toggles fullscreen/normal mode the client under the pointer",
  Mod1Mask|ShiftMask, Button2,
  NEURO_CHAIN_NULL(NeuroActionListToggleFullscreenPtrClient), false
};
static const NeuroButton button6_ = {
  "Resizes in float mode the client under the pointer",
  Mod1Mask|ShiftMask, Button3,
  NEURO_CHAIN_NULL(NeuroActionListFloatResizePtrClient), false
};

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
const char* NeuroConfigDefaultScratchpadCommand[] = { "/usr/bin/xterm", "-name", NEURO_RULE_SCRATCHPAD_NAME, NULL };

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
    &button0_, &button1_, &button2_, &button3_, &button4_, &button5_, &button6_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroConfigSet(const NeuroConfiguration *c) {
  if (c)
    configuration_ = c;
}

const NeuroConfiguration *NeuroConfigGet(void) {
  return configuration_;
}

