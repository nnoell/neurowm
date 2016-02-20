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

#pragma once

// Includes
#include "type.h"

// Defines
#define NEURO_SYSTEM_CLIENT_MASK (FocusChangeMask|PropertyChangeMask|StructureNotifyMask|EnterWindowMask)
#define NEURO_SYSTEM_CLIENT_MASK_NO_ENTER (FocusChangeMask|PropertyChangeMask|StructureNotifyMask)
#define NEURO_SYSTEM_ROOT_MASK (SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask|StructureNotifyMask|\
                                NEURO_SYSTEM_CLIENT_MASK)


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// NeuroSystemCursor
enum NeuroSystemCursor {
  NEURO_SYSTEM_CURSOR_NORMAL = 0,
  NEURO_SYSTEM_CURSOR_RESIZE,
  NEURO_SYSTEM_CURSOR_MOVE,
  NEURO_SYSTEM_CURSOR_END
};
typedef enum NeuroSystemCursor NeuroSystemCursor;

// NeuroSystemWmatom
enum NeuroSystemWmatom {
  NEURO_SYSTEM_WMATOM_PROTOCOLS = 0,
  NEURO_SYSTEM_WMATOM_DELETEWINDOW,
  NEURO_SYSTEM_WMATOM_END
};
typedef enum NeuroSystemWmatom NeuroSystemWmatom;

// NeuroSystemNetatom
enum NeuroSystemNetatom {
  NEURO_SYSTEM_NETATOM_SUPPORTED = 0,
  NEURO_SYSTEM_NETATOM_FULLSCREEN,
  NEURO_SYSTEM_NETATOM_STATE,
  NEURO_SYSTEM_NETATOM_NAME,
  NEURO_SYSTEM_NETATOM_ACTIVE,
  NEURO_SYSTEM_NETATOM_CLOSEWINDOW,
  NEURO_SYSTEM_NETATOM_STRUT,
  NEURO_SYSTEM_NETATOM_END
};
typedef enum NeuroSystemNetatom NeuroSystemNetatom;

// NeuroSystemColor
enum NeuroSystemColor {
  NEURO_SYSTEM_COLOR_NORMAL = 0,
  NEURO_SYSTEM_COLOR_CURRENT,
  NEURO_SYSTEM_COLOR_OLD,
  NEURO_SYSTEM_COLOR_FREE,
  NEURO_SYSTEM_COLOR_URGENT,
  NEURO_SYSTEM_COLOR_END
};
typedef enum NeuroSystemColor NeuroSystemColor;


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// X functions
bool NeuroSystemInit();
void NeuroSystemStop();
Display *NeuroSystemGetDisplay();
Window NeuroSystemGetRoot();
int NeuroSystemGetScreen();
const NeuroRectangle *NeuroSystemGetScreenRegion();
const NeuroRectangle *NeuroSystemGetHiddenRegion();
const int *NeuroSystemGetHiddenGaps();
Cursor NeuroSystemGetCursor(NeuroSystemCursor c);
Atom NeuroSystemGetWmAtom(NeuroSystemWmatom a);
Atom NeuroSystemGetNetAtom(NeuroSystemNetatom a);
NeuroColor NeuroSystemGetColor(NeuroSystemColor c);
NeuroColor NeuroSystemGetColorFromHex(const char *color);

// System functions
const char *const *NeuroSystemGetRecompileCommand(const char **output, const char **source);
void NeuroSystemChangeWmName(const char *name);
void NeuroSystemChangeProcName(const char *name);
pid_t NeuroSystemGetWmPid();
bool NeuroSystemSpawn(const char *const *cmd, pid_t *p);
int NeuroSystemSpawnPipe(const char *const *cmd, pid_t *p);
void NeuroSystemError(const char *msg);

// Binding functions
void NeuroSystemGrabKeys(Window w, const NeuroKey *const *key_list);
void NeuroSystemUngrabKeys(Window w, const NeuroKey *const *key_list);
void NeuroSystemGrabButtons(Window w, const NeuroButton *const *button_list);
void NeuroSystemUngrabButtons(Window w, const NeuroButton *const *button_list);

