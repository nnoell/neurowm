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


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// NeuroSystemCursor
enum NeuroSystemCursor {
  NeuroSystemCursorNormal = 0,
  NeuroSystemCursorResize,
  NeuroSystemCursorMove,
  NeuroSystemCursorCount
};
typedef enum NeuroSystemCursor NeuroSystemCursor;

// NeuroSystemWmAtom
enum NeuroSystemWmAtom {
  NeuroSystemWmAtomProtocols = 0,
  NeuroSystemWmAtomDeleteWindow,
  NeuroSystemWmAtomCount
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
  NeuroSystemNetAtomCount
};
typedef enum NeuroSystemNetAtom NeuroSystemNetAtom;

// NeuroSystemColor
enum NeuroSystemColor {
  NeuroSystemColorNormal = 0,
  NeuroSystemColorCurrent,
  NeuroSystemColorOld,
  NeuroSystemColorFree,
  NeuroSystemColorUrgent,
  NeuroSystemColorCount
};
typedef enum NeuroSystemColor NeuroSystemColor;


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// X functions
Bool NeuroSystemInit();
void NeuroSystemStop();
Display *NeuroSystemGetDisplay();
Window NeuroSystemGetRoot();
int NeuroSystemGetScreen();
int NeuroSystemGetXPos();
int NeuroSystemGetYPos();
int NeuroSystemGetXRes();
int NeuroSystemGetYRes();
const Rectangle *NeuroSystemGetScreenRegion();
Cursor NeuroSystemGetCursor(NeuroSystemCursor c);
Atom NeuroSystemGetWmAtom(NeuroSystemWmAtom a);
Atom NeuroSystemGetNetAtom(NeuroSystemNetAtom a);
Color NeuroSystemGetColor(NeuroSystemColor c);
Color NeuroSystemGetColorFromHex(const char *color);

// System functions
const char * const *NeuroSystemGetRecompileCommand(const char **output, const char **source);
void NeuroSystemChangeWmName(const char *name);
void NeuroSystemChangeProcName(const char *newname);
int NeuroSystemSpawn(const char *const *cmd, pid_t *p);
int NeuroSystemSpawnPipe(const char *const *cmd, pid_t *p);
void NeuroSystemError(const char *msg);

// Binding functions
void NeuroSystemGrabKeys(Window w, const Key *const *key_set);
void NeuroSystemUngrabKeys(Window w, const Key *const *key_set);
void NeuroSystemGrabButtons(Window w, const Button *const *button_set);
void NeuroSystemUngrabButtons(Window w, const Button *const *button_set);

#endif  // NEURO_SYSTEM_H_

