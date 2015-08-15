//----------------------------------------------------------------------------------------------------------------------
// Module      :  type
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_TYPE_H_
#define NEURO_TYPE_H_

// Includes
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <assert.h>

// WM
#define WM_NAME            "neurowm"
#define WM_MYNAME          "myneurowm"
#define WM_SCRATCHPAD_NAME "neurowm_scratchpad"

// Default sizes
#define NAME_MAX    256
#define COLOR_MAX   8
#define ARRSET_MAX  4

// Reload
#define EXIT_RELOAD ((int)'R')

// Client
#define CLIVAL(X)   (**(X))
#define CLI(X)      (*(X))


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// GEOMETRY TYPES ------------------------------------------------------------------------------------------------------

// Rectangle
typedef struct Rectangle Rectangle;
struct Rectangle {
  int x;  // X position
  int y;  // Y position
  int w;  // Width
  int h;  // Height
};

// FreeLocFn
typedef void (*FreeLocFn)(Rectangle *a, const Rectangle *r);


// CLIENT TYPES --------------------------------------------------------------------------------------------------------

// Client
typedef struct Client Client;
struct Client {
  const Window win;
  int ws;
  Bool isNSP;
  char class[ NAME_MAX ];
  char name[ NAME_MAX ];
  char title[ NAME_MAX ];
  Rectangle floatRegion;
  Bool isHidden;
  Bool isFullScreen;
  FreeLocFn freeLocFn;
  unsigned int fixPos;  // Can be: notFixedR | upFixedR | downFixedR | leftFixedR | rightFixedR
  int fixSize;
  Bool isUrgent;
};

// CliPtr
typedef Client *const *CliPtr;

// TestCliPtrFn
typedef Bool (*TestCliPtrFn)(const CliPtr c, const void *const p);

// SelectCliFn
typedef CliPtr (*SelectCliFn)(const CliPtr c);


// FUNCTIONAL TYPES ----------------------------------------------------------------------------------------------------

// GenericAr
typedef union GenericAr GenericAr;
union GenericAr {
  const char c;
  const int i;
  const unsigned int ui;
  const float f;
  const void *const v;
  const char *const *const com;
  const char *const str;
  const FreeLocFn ff;
  const SelectCliFn sf;
};

// GenericFn
typedef void (*GenericFn)(GenericAr arg);

// WMFn
typedef struct WMFn WMFn;
struct WMFn {
  const GenericFn func;
  const GenericAr arg;
};


// LAYOUT TYPES --------------------------------------------------------------------------------------------------------

// Color
typedef unsigned long Color;

// ColorFn
typedef Color (*ColorFn)(const CliPtr c);

// BorderFn
typedef int (*BorderFn)(const CliPtr c);

// Arrange
typedef struct Arrange Arrange;
struct Arrange {
  int size;                     // Number of tiled clients
  Rectangle region;             // Tiled layout region
  Rectangle **cliRegions;       // Region of each client
  Rectangle **cliFloatRegions;  // Float region of each client
  GenericAr *as;                // Settings of the arrange
};

// ArrangeFn
typedef Arrange *(*ArrangeFn)(Arrange *);

// Layout
typedef struct Layout Layout;
struct Layout {
  const ArrangeFn arrangeFn;
  const ColorFn borderColorFn;
  const BorderFn borderWidthFn;
  const BorderFn borderGapFn;
  const float *const region;
  unsigned int mod;  // Can be: notModL | mirrModL | reflXModL | reflYModL
  Bool followMouse;
  GenericAr as[ ARRSET_MAX ];
};


// CONFIG TYPES --------------------------------------------------------------------------------------------------------

// LayoutConf
typedef struct LayoutConf LayoutConf;
struct LayoutConf {
  const char *const name;
  const ArrangeFn arrangeFn;
  const ColorFn borderColorFn;
  const BorderFn borderWidthFn;
  const BorderFn borderGapFn;
  const float region[ 4 ];
  const unsigned int mod;  // Can be: notModL | mirrModL | reflXModL | reflYModL
  const Bool followMouse;
  const GenericAr as[ ARRSET_MAX ];
};

// Workspace
typedef struct Workspace Workspace;
struct Workspace {
  const char *const name;
  const int gaps[ 4 ];
  const LayoutConf *const *const layouts;
  const LayoutConf *const *const togLayouts;
};

// Key
typedef struct Key Key;
struct Key {
  const unsigned int mod;
  const KeySym keySym;
  const GenericFn handler;
  const GenericAr arg;
};

// Button
typedef struct Button Button;
struct Button {
  const unsigned int mod;
  const unsigned int button;
  const GenericFn handler;
  const GenericAr arg;
  const Bool ungrabOnFocus;
};

// Rule
typedef struct Rule Rule;
struct Rule {
  const char *const class;
  const char *const name;
  const char *const title;
  const Bool isFullScreen;
  const FreeLocFn freeLocFn;
  const unsigned int fixPos;
  const float fixSize;
  const int workspace;
  const Bool goWorkspace;
};

// BoxPP
typedef struct BoxPP BoxPP;
struct BoxPP {
  const char *const bgColor;
  const char *const fgColor;
  const char *const boxColor;
  const char *const leftIcon;
  const char *const rightIcon;
  const int boxHeight;
};

// CA
typedef struct CA CA;
struct CA {
  const char *const leftClick;
  const char *const middleClick;
  const char *const rightClick;
  const char *const wheelUp;
  const char *const wheelDown;
};

// DzenFlags
typedef struct DzenFlags DzenFlags;
struct DzenFlags {
  const int x, y, w, h;
  const char *const fgColor;
  const char *const bgColor;
  const char align;
  const char *const font;
  const char *const event;
  const char *const extras;
};

// LoggerFn
typedef void (*const LoggerFn)(char *);

// DzenPanel
typedef struct DzenPanel DzenPanel;
struct DzenPanel {
  const DzenFlags *const df;
  const LoggerFn *const loggers;
  const char *const sep;
  const int refreshRate;
};

// WMConfig
typedef struct WMConfig WMConfig;
struct WMConfig {
  const char *const normBorderColor;
  const char *const currBorderColor;
  const char *const prevBorderColor;
  const char *const freeBorderColor;
  const char *const urgtBorderColor;
  const int borderWidth;
  const int borderGap;
  const Workspace *const *const workspaceSet;
  const Rule *const *const ruleSet;
  const DzenPanel *const *const dzenPanelSet;
  const Key *const *const keys;
  const Button *const *const buttons;
  const WMFn *const *const startUpHook;
  const WMFn *const *const endUpHook;
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Allocs and Frees
Client *allocClientT(Window w, const XWindowAttributes *wa);
void freeClientT(Client *c);

// Useful type functions
size_t ptrArrayLengthT(const void *const *arrayPtr);

#endif  // NEURO_TYPE_H_

