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

// NeuroSystemCursor
enum NeuroSystemCursor {
  NeuroSystemCursorNormal = 0,
  NeuroSystemCursorResize,
  NeuroSystemCursorMove,
  NeuroSystemCursorLast
};
typedef enum NeuroSystemCursor NeuroSystemCursor;

// NeuroSystemWmAtom
enum NeuroSystemWmAtom {
  NeuroSystemWmAtomProtocols = 0,
  NeuroSystemWmAtomDeleteWindow,
  NeuroSystemWmAtomLast
};
typedef enum NeuroSystemWmAtom NeuroSystemWmAtom;

// NeuroSystemNetAtom
enum NeuroSystemNetAtom {
  NeuroSystemNetAtomSupported = 0,
  NeuroSystemNetAtomFullscreen,
  NeuroSystemNetAtomState,
  NeuroSystemNetAtomName,
  NeuroSystemNetAtomActive,
  NeuroSystemNetAtomCloseWindow,
  NeuroSystemNetAtomStrut,
  NeuroSystemNetAtomLast
};
typedef enum NeuroSystemNetAtom NeuroSystemNetAtom;

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


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// X functions
Display *NeuroSystemGetDisplay();
Window NeuroSystemGetRoot();
int NeuroSystemGetScreen();
int NeuroSystemGetXRes();
int NeuroSystemGetYRes();
const Rectangle *NeuroSystemGetScreenRegion();
Cursor NeuroSystemGetCursor(NeuroSystemCursor c);
Atom NeuroSystemGetWmAtom(NeuroSystemWmAtom a);
Atom NeuroSystemGetNetAtom(NeuroSystemNetAtom a);

// Main functions
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

