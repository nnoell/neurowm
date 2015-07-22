//----------------------------------------------------------------------------------------------------------------------
// Module      :  base
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_BASE_H_
#define NEURO_BASE_H_

// Includes
#include "general.h"

// Defines
#define CLIENT_MASK (FocusChangeMask|PropertyChangeMask|StructureNotifyMask|EnterWindowMask)
#define CLIENT_MASK_NO_ENTER (FocusChangeMask|PropertyChangeMask|StructureNotifyMask)
#define ROOT_MASK (SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask|StructureNotifyMask|CLIENT_MASK)

#define xPos 0
#define yPos 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// WM global variables
extern Display *const display;
extern const int screen;
extern const Window root;
extern const uint32_t xRes;
extern const uint32_t yRes;
extern const Area screenArea;

// Global configuration
extern const Color normBorderColorB;
extern const Color currBorderColorB;
extern const Color prevBorderColorB;
extern const Color freeBorderColorB;
extern const Color urgtBorderColorB;
extern const int borderWidthB;
extern const int borderGapB;
extern const Key *const *const keyBindingsB;
extern const Button *const *const buttonBindingsB;
extern const Rule *const *const ruleSetB;
extern const Workspace *const *const workspaceSetB;
extern const DzenPanel *const *const dzenPanelSetB;
extern const WMFunc *const *const startUpHookB;
extern const WMFunc *const *const endUpHookB;

// Cursors and Atoms
enum { CurNormal, CurResize, CurMove, CurLast };
enum { WM_PROTOCOLS, WM_DELETE_WINDOW, WM_COUNT };
enum { NET_SUPPORTED, NET_FULLSCREEN, NET_WM_STATE, NET_WM_NAME, NET_ACTIVE, NET_CLOSE_WINDOW, NET_STRUT, NET_COUNT };
extern const Cursor cursors[ CurLast ];
extern const Atom wmatoms[ WM_COUNT ];
extern const Atom netatoms[ NET_COUNT ];


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void setConfigB(const WMConfig *c);
Bool initB();
void endB();

// Binding functions
void grabKeysB(Window w);
void ungrabKeysB(Window w);
void grabButtonsB(Window w);
void ungrabButtonsB(Window w);

// Other useful X functions
Color getColorB(const char* color);
void changeWMNameB(Arg arg);

#endif  // NEURO_BASE_H_

