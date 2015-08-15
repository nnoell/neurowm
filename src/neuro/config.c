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
#include "neurowm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Commands
static const char* termcmd[]  = { "/usr/bin/xterm", NULL };
static const char* lchrcmd[]  = { "/usr/bin/gmrun", NULL };

// Startup
const WMFn *defStartUpHook[] = { NULL };

// Endup
const WMFn *defEndUpHook[] = { NULL };

// Layouts
static const LayoutConf tile[] = { {
  "Tile", tallArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC,
  {0.0f, 0.0f, 1.0f, 1.0f}, notModL, True, {1.0f, 0.5f, 0.03f}
} };
static const LayoutConf mirr[] = { {
  "Mirr", tallArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC,
  {0.0f, 0.0f, 1.0f, 1.0f}, mirrModL, True, {1.0f, 0.5f, 0.03f}
} };
static const LayoutConf grid[] = { {
  "Grid", gridArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC,
  {0.0f, 0.0f, 1.0f, 1.0f}, notModL, True, {0.0f}
} };
static const LayoutConf full[] = { {
  "Full", fullArrL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC,
  {0.0f, 0.0f, 1.0f, 1.0f}, notModL, True, {0.0f}
} };
static const LayoutConf floa[] = { {
  "Float", floatArrL, allBorderColorC, alwaysBorderWidthC, alwaysBorderGapC,
  {0.0f, 0.0f, 1.0f, 1.0f}, notModL, False, {0.0f}
} };

// Layouts
const LayoutConf *defLayoutSet[] = { tile, mirr, grid, NULL };

// TogLayoutSet
const LayoutConf *defTogLayoutSet[] = { full, floa, NULL };

// Workspaces
static const Workspace ws0[] = { { "ws1", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws1[] = { { "ws2", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws2[] = { { "ws3", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws3[] = { { "ws4", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws4[] = { { "ws5", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws5[] = { { "ws6", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws6[] = { { "ws7", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws7[] = { { "ws8", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws8[] = { { "ws9", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws9[] = { { "ws0", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };

// WorkspaceSet
const Workspace *defWorkspaceSet[] = { ws0, ws1, ws2, ws3, ws4, ws5, ws6, ws7, ws8, ws9, NULL };

// RuleSet
const Rule *defRuleSet[] = { NULL };

// DzenPanelSet
const DzenPanel *defDzenPanelSet[] = { NULL };

// Key
static const Key key00[] = { { Mod1Mask|ShiftMask,   XK_Return, spawnN,                 {.com = termcmd}  } };
static const Key key01[] = { { Mod1Mask,             XK_c,      killCliN,               {NULL}            } };
static const Key key02[] = { { Mod1Mask,             XK_j,      moveFocusN,             {.sf = nextC}     } };
static const Key key03[] = { { Mod1Mask,             XK_k,      moveFocusN,             {.sf = prevC}     } };
static const Key key04[] = { { Mod1Mask,             XK_Tab,    moveFocusN,             {.sf = oldC}      } };
static const Key key05[] = { { Mod1Mask,             XK_m,      moveFocusN,             {.sf = headC}     } };
static const Key key06[] = { { Mod1Mask|ShiftMask,   XK_j,      swapCliN,               {.sf = nextC}     } };
static const Key key07[] = { { Mod1Mask|ShiftMask,   XK_k,      swapCliN,               {.sf = prevC}     } };
static const Key key08[] = { { Mod1Mask|ShiftMask,   XK_p,      swapCliN,               {.sf = oldC}      } };
static const Key key09[] = { { Mod1Mask|ShiftMask,   XK_m,      swapCliN,               {.sf = headC}     } };
static const Key key10[] = { { Mod1Mask,             XK_Up,     moveFocusN,             {.sf = upC}       } };
static const Key key11[] = { { Mod1Mask,             XK_Down,   moveFocusN,             {.sf = downC}     } };
static const Key key12[] = { { Mod1Mask,             XK_Left,   moveFocusN,             {.sf = leftC}     } };
static const Key key13[] = { { Mod1Mask,             XK_Right,  moveFocusN,             {.sf = rightC}    } };
static const Key key14[] = { { Mod1Mask|ShiftMask,   XK_Up,     swapCliN,               {.sf = upC}       } };
static const Key key15[] = { { Mod1Mask|ShiftMask,   XK_Down,   swapCliN,               {.sf = downC}     } };
static const Key key16[] = { { Mod1Mask|ShiftMask,   XK_Left,   swapCliN,               {.sf = leftC}     } };
static const Key key17[] = { { Mod1Mask|ShiftMask,   XK_Right,  swapCliN,               {.sf = rightC}    } };
static const Key key18[] = { { Mod1Mask,             XK_space,  changeLayoutN,          {.i =  1}         } };
static const Key key19[] = { { Mod1Mask,             XK_t,      toggleFreeCliN,         {.ff = defFreeR}  } };
static const Key key20[] = { { Mod1Mask,             XK_z,      toggleLayoutModN,       {.ui = mirrModL}  } };
static const Key key21[] = { { Mod1Mask,             XK_x,      toggleLayoutModN,       {.ui = reflXModL} } };
static const Key key22[] = { { Mod1Mask,             XK_y,      toggleLayoutModN,       {.ui = reflYModL} } };
static const Key key23[] = { { Mod1Mask,             XK_f,      toggleLayoutN,          {.i =  0}         } };
static const Key key24[] = { { Mod1Mask,             XK_o,      toggleLayoutN,          {.i =  1}         } };
static const Key key25[] = { { Mod1Mask|ShiftMask,   XK_f,      toggleFullScreenCliN,   {NULL}            } };
static const Key key26[] = { { Mod1Mask,             XK_comma,  increaseMasterN,        {.i =  1}         } };
static const Key key27[] = { { Mod1Mask,             XK_period, increaseMasterN,        {.i = -1}         } };
static const Key key28[] = { { Mod1Mask,             XK_l,      resizeMasterN,          {.i =  1}         } };
static const Key key29[] = { { Mod1Mask,             XK_h,      resizeMasterN,          {.i = -1}         } };
static const Key key30[] = { { Mod1Mask,             XK_1,      changeToWorkspaceN,     {.i =  0}         } };
static const Key key31[] = { { Mod1Mask,             XK_2,      changeToWorkspaceN,     {.i =  1}         } };
static const Key key32[] = { { Mod1Mask,             XK_3,      changeToWorkspaceN,     {.i =  2}         } };
static const Key key33[] = { { Mod1Mask,             XK_4,      changeToWorkspaceN,     {.i =  3}         } };
static const Key key34[] = { { Mod1Mask,             XK_5,      changeToWorkspaceN,     {.i =  4}         } };
static const Key key35[] = { { Mod1Mask,             XK_6,      changeToWorkspaceN,     {.i =  5}         } };
static const Key key36[] = { { Mod1Mask,             XK_7,      changeToWorkspaceN,     {.i =  6}         } };
static const Key key37[] = { { Mod1Mask,             XK_8,      changeToWorkspaceN,     {.i =  7}         } };
static const Key key38[] = { { Mod1Mask,             XK_9,      changeToWorkspaceN,     {.i =  8}         } };
static const Key key39[] = { { Mod1Mask,             XK_0,      changeToWorkspaceN,     {.i =  9}         } };
static const Key key40[] = { { ControlMask|Mod1Mask, XK_Left,   changeToPrevWorkspaceN, {NULL}            } };
static const Key key41[] = { { ControlMask|Mod1Mask, XK_Right,  changeToNextWorkspaceN, {NULL}            } };
static const Key key42[] = { { Mod1Mask|ShiftMask,   XK_Tab,    changeToLastWorkspaceN, {NULL}            } };
static const Key key43[] = { { Mod1Mask|ShiftMask,   XK_1,      moveCliToWorkspaceN,    {.i =  0}         } };
static const Key key44[] = { { Mod1Mask|ShiftMask,   XK_2,      moveCliToWorkspaceN,    {.i =  1}         } };
static const Key key45[] = { { Mod1Mask|ShiftMask,   XK_3,      moveCliToWorkspaceN,    {.i =  2}         } };
static const Key key46[] = { { Mod1Mask|ShiftMask,   XK_4,      moveCliToWorkspaceN,    {.i =  3}         } };
static const Key key47[] = { { Mod1Mask|ShiftMask,   XK_5,      moveCliToWorkspaceN,    {.i =  4}         } };
static const Key key48[] = { { Mod1Mask|ShiftMask,   XK_6,      moveCliToWorkspaceN,    {.i =  5}         } };
static const Key key49[] = { { Mod1Mask|ShiftMask,   XK_7,      moveCliToWorkspaceN,    {.i =  6}         } };
static const Key key50[] = { { Mod1Mask|ShiftMask,   XK_8,      moveCliToWorkspaceN,    {.i =  7}         } };
static const Key key51[] = { { Mod1Mask|ShiftMask,   XK_9,      moveCliToWorkspaceN,    {.i =  8}         } };
static const Key key52[] = { { Mod1Mask|ShiftMask,   XK_0,      moveCliToWorkspaceN,    {.i =  9}         } };
static const Key key53[] = { { Mod1Mask|ShiftMask,   XK_q,      quitN,                  {NULL}            } };
static const Key key54[] = { { Mod1Mask,             XK_q,      reloadN,                {NULL}            } };
static const Key key55[] = { { Mod1Mask,             XK_F2,     spawnN,                 {.com = lchrcmd}  } };
static const Key key56[] = { { Mod1Mask|ShiftMask,   XK_space,  resetLayoutN,           {NULL}            } };
static const Key key57[] = { { Mod1Mask,             XK_n,      minimizeCliN,           {NULL}            } };
static const Key key58[] = { { Mod1Mask|ShiftMask,   XK_n,      restoreCliN,            {NULL}            } };

// Keys
const Key *defKeys[] = { key00, key01, key02, key03, key04, key05, key06, key07, key08, key09,
                         key10, key11, key12, key13, key14, key15, key16, key17, key18, key19,
                         key20, key21, key22, key23, key24, key25, key26, key27, key28, key29,
                         key30, key31, key32, key33, key34, key35, key36, key37, key38, key39,
                         key40, key41, key42, key43, key44, key45, key46, key47, key48, key49,
                         key50, key51, key52, key53, key54, key55, key56, key57, key58, NULL };

// Button
static const Button button00[] = { { noModMask,          Button1, moveFocusN,              {.sf = pointerC},  True  } };
static const Button button01[] = { { Mod1Mask,           Button1, freeMovePointerCliN,     {NULL},            False } };
static const Button button02[] = { { Mod1Mask,           Button2, toggleFreeCliN,          {.ff = defFreeR},  False } };
static const Button button03[] = { { Mod1Mask,           Button3, freeResizePointerCliN,   {NULL},            False } };
static const Button button04[] = { { Mod1Mask|ShiftMask, Button1, movePointerCliN,         {NULL},            False } };
static const Button button05[] = { { Mod1Mask|ShiftMask, Button2, toggleFullScreenPtrCliN, {NULL},            False } };
static const Button button06[] = { { Mod1Mask|ShiftMask, Button3, resizePointerCliN,       {NULL},            False } };

// Buttons
const Button *defButtons[] = { button00, button01, button02, button03, button04, button05, button05, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

const WMConfig defWMConfig = {
  defNormBorderColor,
  defCurrBorderColor,
  defPrevBorderColor,
  defFreeBorderColor,
  defUrgtBorderColor,
  defBorderWidth,
  defBorderGap,
  defWorkspaceSet,
  defRuleSet,
  defDzenPanelSet,
  defKeys,
  defButtons,
  defStartUpHook,
  defEndUpHook
};

