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
// STARTUP HOOK
//----------------------------------------------------------------------------------------------------------------------

// FUNCS                              FUNC            ARGS
static const WMFn chgwmname[] = { { changeWMNameN,  {.str = "LG3D"}   } };
static const WMFn loadxdef[]  = { { spawnN,         {.com = xdefload} } };
static const WMFn wallpaper[] = { { spawnN,         {.com = wallcmd}  } };
static const WMFn startcpu[]  = { { startCpuCalcDP, {NULL}            } };
static const WMFn endcpu[]    = { { endCpuCalcDP,   {NULL}            } };

// STARTUP
static const WMFn *myStartUpHook[] = { chgwmname, loadxdef, wallpaper, startcpu, NULL };

// ENDUP
static const WMFn *myEndUpHook[] = { endcpu, NULL };


//----------------------------------------------------------------------------------------------------------------------
// LAYOUTS
//----------------------------------------------------------------------------------------------------------------------

// LAYOUTS (NAME, ARRANGE, BORDERCOLORF, BORDERWIDTHF, BORDERGAPF, REGION (XYWH), MOD, FOLLOWMOUSE, ARGS)
static const LayoutConf tile[] = { {
  "Tile", tallArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {1.0f, 0.5f, 0.03f}
} };
static const LayoutConf mirr[] = { {
  "Mirr", tallArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL, True, {1.0f, 0.5f, 0.03f}
} };
static const LayoutConf grid[] = { {
  "Grid", gridArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL|reflXModL|reflYModL, True, {0.0f}
} };
static const LayoutConf full[] = { {
  "Full",  fullArrL,  allBorderColorC, smartBorderWidthC,  alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {0.0f}
} };
static const LayoutConf floa[] = { {
  "Float", floatArrL, allBorderColorC, alwaysBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, False, {0.0f}
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

// KEY                         MOD                    KEY           FUNC                    ARGS
static const Key key00[] = { { myModMask|ShiftMask,  XK_Return,    spawnN,                 {.com = termcmd}       } };
static const Key key01[] = { { myModMask,            XK_c,         killCliN,               {NULL}                 } };
static const Key key02[] = { { myModMask,            XK_j,         moveFocusN,             {.sf = nextC}          } };
static const Key key03[] = { { myModMask,            XK_k,         moveFocusN,             {.sf = prevC}          } };
static const Key key04[] = { { myModMask,            XK_Tab,       moveFocusN,             {.sf = oldC}           } };
static const Key key05[] = { { myModMask,            XK_m,         moveFocusN,             {.sf = headC}          } };
static const Key key06[] = { { myModMask|ShiftMask,  XK_j,         swapCliN,               {.sf = nextC}          } };
static const Key key07[] = { { myModMask|ShiftMask,  XK_k,         swapCliN,               {.sf = prevC}          } };
static const Key key08[] = { { myModMask|ShiftMask,  XK_p,         swapCliN,               {.sf = oldC}           } };
static const Key key09[] = { { myModMask|ShiftMask,  XK_m,         swapCliN,               {.sf = lastC}          } };
static const Key key10[] = { { myModMask,            XK_Up,        moveFocusN,             {.sf = upC}            } };
static const Key key11[] = { { myModMask,            XK_Down,      moveFocusN,             {.sf = downC}          } };
static const Key key12[] = { { myModMask,            XK_Left,      moveFocusN,             {.sf = leftC}          } };
static const Key key13[] = { { myModMask,            XK_Right,     moveFocusN,             {.sf = rightC}         } };
static const Key key14[] = { { myModMask|ShiftMask,  XK_Up,        swapCliN,               {.sf = upC}            } };
static const Key key15[] = { { myModMask|ShiftMask,  XK_Down,      swapCliN,               {.sf = downC}          } };
static const Key key16[] = { { myModMask|ShiftMask,  XK_Left,      swapCliN,               {.sf = leftC}          } };
static const Key key17[] = { { myModMask|ShiftMask,  XK_Right,     swapCliN,               {.sf = rightC}         } };
static const Key key18[] = { { myModMask,            XK_space,     changeLayoutN,          {.i =  1}              } };
static const Key key19[] = { { myModMask,            XK_t,         toggleFreeCliN,         {.ff = defFreeR }      } };
static const Key key20[] = { { myModMask,            XK_z,         toggleLayoutModN,       {.ui = mirrModL}       } };
static const Key key21[] = { { myModMask,            XK_x,         toggleLayoutModN,       {.ui = reflXModL}      } };
static const Key key22[] = { { myModMask,            XK_y,         toggleLayoutModN,       {.ui = reflYModL}      } };
static const Key key23[] = { { myModMask,            XK_f,         toggleLayoutN,          {.i =  0}              } };
static const Key key24[] = { { myModMask,            XK_o,         toggleLayoutN,          {.i =  1}              } };
static const Key key25[] = { { myModMask|ShiftMask,  XK_f,         toggleFullScreenCliN,   {NULL}                 } };
static const Key key26[] = { { myModMask,            XK_comma,     increaseMasterN,        {.i =  1}              } };
static const Key key27[] = { { myModMask,            XK_period,    increaseMasterN,        {.i = -1}              } };
static const Key key28[] = { { myModMask,            XK_l,         resizeMasterN,          {.i =  1}              } };
static const Key key29[] = { { myModMask,            XK_h,         resizeMasterN,          {.i = -1}              } };
static const Key key30[] = { { myModMask,            XK_1,         changeToWorkspaceN,     {.i =  0}              } };
static const Key key31[] = { { myModMask,            XK_2,         changeToWorkspaceN,     {.i =  1}              } };
static const Key key32[] = { { myModMask,            XK_3,         changeToWorkspaceN,     {.i =  2}              } };
static const Key key33[] = { { myModMask,            XK_4,         changeToWorkspaceN,     {.i =  3}              } };
static const Key key34[] = { { myModMask,            XK_5,         changeToWorkspaceN,     {.i =  4}              } };
static const Key key35[] = { { myModMask,            XK_6,         changeToWorkspaceN,     {.i =  5}              } };
static const Key key36[] = { { myModMask,            XK_7,         changeToWorkspaceN,     {.i =  6}              } };
static const Key key37[] = { { myModMask,            XK_8,         changeToWorkspaceN,     {.i =  7}              } };
static const Key key38[] = { { myModMask,            XK_9,         changeToWorkspaceN,     {.i =  8}              } };
static const Key key39[] = { { myModMask,            XK_0,         changeToWorkspaceN,     {.i =  9}              } };
static const Key key40[] = { { ControlMask|Mod1Mask, XK_Left,      changeToPrevWorkspaceN, {NULL}                 } };
static const Key key41[] = { { ControlMask|Mod1Mask, XK_Right,     changeToNextWorkspaceN, {NULL}                 } };
static const Key key42[] = { { myModMask|ShiftMask,  XK_Tab,       changeToLastWorkspaceN, {NULL}                 } };
static const Key key43[] = { { myModMask|ShiftMask,  XK_1,         moveCliToWorkspaceN,    {.i =  0}              } };
static const Key key44[] = { { myModMask|ShiftMask,  XK_2,         moveCliToWorkspaceN,    {.i =  1}              } };
static const Key key45[] = { { myModMask|ShiftMask,  XK_3,         moveCliToWorkspaceN,    {.i =  2}              } };
static const Key key46[] = { { myModMask|ShiftMask,  XK_4,         moveCliToWorkspaceN,    {.i =  3}              } };
static const Key key47[] = { { myModMask|ShiftMask,  XK_5,         moveCliToWorkspaceN,    {.i =  4}              } };
static const Key key48[] = { { myModMask|ShiftMask,  XK_6,         moveCliToWorkspaceN,    {.i =  5}              } };
static const Key key49[] = { { myModMask|ShiftMask,  XK_7,         moveCliToWorkspaceN,    {.i =  6}              } };
static const Key key50[] = { { myModMask|ShiftMask,  XK_8,         moveCliToWorkspaceN,    {.i =  7}              } };
static const Key key51[] = { { myModMask|ShiftMask,  XK_9,         moveCliToWorkspaceN,    {.i =  8}              } };
static const Key key52[] = { { myModMask|ShiftMask,  XK_0,         moveCliToWorkspaceN,    {.i =  9}              } };
static const Key key53[] = { { myModMask|ShiftMask,  XK_q,         quitN,                  {NULL}                 } };
static const Key key54[] = { { myModMask,            XK_q,         reloadN,                {NULL}                 } };
static const Key key55[] = { { Mod1Mask,             XK_F2,        spawnN,                 {.com = lchrcmd}       } };
static const Key key56[] = { { myModMask|ShiftMask,  XK_space,     resetLayoutN,           {NULL}                 } };
static const Key key57[] = { { myModMask,            XK_n,         minimizeCliN,           {NULL}                 } };
static const Key key58[] = { { myModMask|ShiftMask,  XK_n,         restoreCliN,            {NULL}                 } };
static const Key key59[] = { { myModMask|ShiftMask,  XK_t,         freeCliN,               {.ff = bigCenterFreeR} } };
static const Key key60[] = { { myModMask,            XK_grave,     toggleNSPN,             {.com = nspcmd}        } };
static const Key key61[] = { { myModMask,            XK_masculine, toggleNSPN,             {.com = nspcmd}        } };
static const Key key62[] = { { Mod1Mask,             XK_Up,        spawnN,                 {.com = volupcmd}      } };
static const Key key63[] = { { Mod1Mask,             XK_Down,      spawnN,                 {.com = voldocmd}      } };
static const Key key64[] = { { Mod1Mask,             XK_Right,     spawnN,                 {.com = songncmd}      } };
static const Key key65[] = { { Mod1Mask,             XK_Left,      spawnN,                 {.com = songpcmd}      } };

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

// BUTTON (MOD, BUTTON, FUNCT, ARGS, UNWRAPONFOCUS)
static const Button button00[] = { { noModMask,           Button1, moveFocusN,              {.sf = pointerC}, True  } };
static const Button button01[] = { { myModMask,           Button1, freeMovePointerCliN,     {NULL},           False } };
static const Button button02[] = { { myModMask,           Button2, toggleFreePtrCliN,       {.ff = defFreeR}, False } };
static const Button button03[] = { { myModMask,           Button3, freeResizePointerCliN,   {NULL},           False } };
static const Button button04[] = { { myModMask|ShiftMask, Button1, movePointerCliN,         {NULL},           False } };
static const Button button05[] = { { myModMask|ShiftMask, Button2, toggleFullScreenPtrCliN, {NULL},           False } };
static const Button button06[] = { { myModMask|ShiftMask, Button3, resizePointerCliN,       {NULL},           False } };

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

