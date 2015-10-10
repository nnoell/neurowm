//----------------------------------------------------------------------------------------------------------------------
// Module      :  system
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_SYSTEM_H_
#define NEURO_SYSTEM_H_

// Includes
#include "type.h"

// Defines
#define CLIENT_MASK (FocusChangeMask|PropertyChangeMask|StructureNotifyMask|EnterWindowMask)
#define CLIENT_MASK_NO_ENTER (FocusChangeMask|PropertyChangeMask|StructureNotifyMask)
#define ROOT_MASK (SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask|StructureNotifyMask|CLIENT_MASK)

#define NeuroSystemGetXPos 0
#define NeuroSystemGetYPos 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Global configuration
extern const Color normBorderColorS;
extern const Color currBorderColorS;
extern const Color prevBorderColorS;
extern const Color freeBorderColorS;
extern const Color urgtBorderColorS;
extern const int borderWidthS;
extern const int borderGapS;
extern const Key *const *const keyBindingsS;
extern const Button *const *const buttonBindingsS;
extern const Rule *const *const ruleSetS;
extern const Workspace *const *const workspaceSetS;
extern const DzenPanel *const *const dzenPanelSetS;
extern const ActionChain *const startUpHookS;
extern const ActionChain *const endUpHookS;

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
Display *NeuroSystemGetDisplay();
Window NeuroSystemGetRoot();
int NeuroSystemGetScreen();
int NeuroSystemGetXRes();
int NeuroSystemGetYRes();
const Rectangle *NeuroSystemGetScreenRegion();


void NeuroSystemSetConfiguration(const Configuration *c);
Bool NeuroSystemInit();
void NeuroSystemStop();
Color NeuroSystemGetColor(const char *color);
void NeuroSystemChangeNeurowmName(const char *name);
void NeuroSystemChangeProcName(const char *newname);
int NeuroSystemSpawn(const char *const *cmd, pid_t *p);
int NeuroSystemSpawnPipe(const char *const *cmd, pid_t *p);
void NeuroSystemError(const char *msg);

// Binding functions
void NeuroSystemGrabKeys(Window w);
void NeuroSystemUngrabKeys(Window w);
void NeuroSystemGrabButtons(Window w);
void NeuroSystemUngrabButtons(Window w);

#endif  // NEURO_SYSTEM_H_

