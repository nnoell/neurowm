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
#define WM_EVENT           0

// Arg constructors
#define ARG_NULL     {.pointer_ = NULL}
#define ARG_PTR(X)   {.pointer_ = (X)}
#define ARG_CHAR(X)  {.char_ = (X)}
#define ARG_INT(X)   {.int_ = (X)}
#define ARG_UINT(X)  {.uint_ = (X)}
#define ARG_FLOAT(X) {.float_ = (X)}
#define ARG_STR(X)   {.string_ = (X)}
#define ARG_CMD(X)   {.command_ = (X)}
#define ARG_FLF(X)   {.GenericArgFn_.FreeSetterFn_ = (X)}
#define ARG_CSF(X)   {.GenericArgFn_.ClientSelectorFn_ = (X)}
#define ARG_WSF(X)   {.GenericArgFn_.WorkspaceSelectorFn_ = (X)}

// Arg getters
#define ARG_PTR_GET(X)   (X).pointer_
#define ARG_CHAR_GET(X)  (X).char_
#define ARG_INT_GET(X)   (X).int_
#define ARG_UINT_GET(X)  (X).uint_
#define ARG_FLOAT_GET(X) (X).float_
#define ARG_STR_GET(X)   (X).string_
#define ARG_CMD_GET(X)   (X).command_
#define ARG_GAF_GET(X)   (X).GenericArgFn_
#define ARG_FLF_GET(X)   (X).GenericArgFn_.FreeSetterFn_
#define ARG_CSF_GET(X)   (X).GenericArgFn_.ClientSelectorFn_
#define ARG_WSF_GET(X)   (X).GenericArgFn_.WorkspaceSelectorFn_

// Chainned Actions
#define CHAIN_ARG_NULL {.use = False, .arg = ARG_NULL}
#define CHAIN_ARG(X)   {.use = True, .arg = X}
#define CHAIN_NULL(X)  {.chain = (X), .arg = CHAIN_ARG_NULL}
#define CHAIN(X,Y)     {.chain = (X), .arg = CHAIN_ARG(Y)}

// Default sizes
#define NAME_MAX    256
#define COLOR_MAX   8
#define ARRSET_MAX  4

// Reload
#define EXIT_RELOAD ((int)'R')

// ClientPtrPtr
#define CLI_GET(X)  (**(X))


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

// FreeSetterFn
typedef void (*FreeSetterFn)(Rectangle *a, const Rectangle *r);


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
  FreeSetterFn freeSetterFn;
  unsigned int fixPos;  // Can be: notFixedR | upFixedR | downFixedR | leftFixedR | rightFixedR
  int fixSize;
  Bool isUrgent;
};

// ClientPtrPtr
typedef Client *const *ClientPtrPtr;

// ClientTesterFn
typedef Bool (*ClientTesterFn)(ClientPtrPtr c, const void *p);

// ClientSelectorFn
typedef ClientPtrPtr (*ClientSelectorFn)(ClientPtrPtr c);

// GenericClientFn
typedef void (*GenericClientFn)(ClientPtrPtr c, const void *data);


// WORKSPACE TYPES -----------------------------------------------------------------------------------------------------

// WorkspaceSelectorFn
typedef int (*WorkspaceSelectorFn)();


// GENERIC TYPES -------------------------------------------------------------------------------------------------------

// GenericCliActionFn
typedef void (*GenericCliActionFn)(ClientPtrPtr c, ClientSelectorFn gcf, const void *data);

// GenericWSActionFn
typedef void (*GenericWSActionFn)(int ws);

// GenericArgFn (Needed to wrap the function pointers into a union/struct so that they can be treated as data)
typedef union GenericArgFn GenericArgFn;
union GenericArgFn {
  const FreeSetterFn FreeSetterFn_;
  const ClientSelectorFn ClientSelectorFn_;
  const WorkspaceSelectorFn WorkspaceSelectorFn_;
};

// GenericArg
typedef union GenericArg GenericArg;
union GenericArg {
  const void *const pointer_;
  const char char_;
  const int int_;
  const unsigned int uint_;
  const float float_;
  const char *const string_;
  const char *const *const command_;
  const GenericArgFn GenericArgFn_;
};


// ACTION TYPES --------------------------------------------------------------------------------------------------------

// ActionFn
typedef void (*ActionFn)(GenericArg arg);

// Action
typedef struct Action Action;
struct Action {
  const ActionFn handler;
  const GenericArg arg;
};

// ActionChainArg
typedef struct ActionChainArg ActionChainArg;
struct ActionChainArg {
  const Bool use;
  const GenericArg arg;
};

// ActionChain
typedef struct ActionChain ActionChain;
struct ActionChain {
  const Action *const *const chain;
  const ActionChainArg arg;
};


// LAYOUT TYPES --------------------------------------------------------------------------------------------------------

// Color
typedef unsigned long Color;

// ColorSetterFn
typedef Color (*ColorSetterFn)(ClientPtrPtr c);

// BorderSetterFn
typedef int (*BorderSetterFn)(ClientPtrPtr c);

// Arrange
typedef struct Arrange Arrange;
struct Arrange {
  int size;                    // Number of tiled clients
  Rectangle region;            // Tiled layout region
  Rectangle **cliRegions;      // Region of each client
  Rectangle **cliFloatRegions; // Float region of each client
  GenericArg *arrangeSettings;   // Settings of the arrange
};

// ArrangerFn
typedef Arrange *(*ArrangerFn)(Arrange *);

// Layout
typedef struct Layout Layout;
struct Layout {
  const ArrangerFn arrangerFn;
  const ColorSetterFn borderColorSetterFn;
  const BorderSetterFn borderWidthSetterFn;
  const BorderSetterFn borderGapSetterFn;
  const float *const region;
  unsigned int mod;  // Can be: notModL | mirrModL | reflXModL | reflYModL
  Bool followMouse;
  GenericArg arrangeSettings[ ARRSET_MAX ];
};


// CONFIG TYPES --------------------------------------------------------------------------------------------------------

// LayoutConf
typedef struct LayoutConf LayoutConf;
struct LayoutConf {
  const char *const name;
  const ArrangerFn arrangerFn;
  const ColorSetterFn borderColorSetterFn;
  const BorderSetterFn borderWidthSetterFn;
  const BorderSetterFn borderGapSetterFn;
  const float region[ 4 ];
  const unsigned int mod;  // Can be: notModL | mirrModL | reflXModL | reflYModL
  const Bool followMouse;
  const GenericArg arrangeSettings[ ARRSET_MAX ];
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
  const KeySym key;
  const ActionChain actionChain;
};

// Button
typedef struct Button Button;
struct Button {
  const unsigned int mod;
  const unsigned int button;
  const ActionChain actionChain;
  const Bool ungrabOnFocus;
};

// Rule
typedef struct Rule Rule;
struct Rule {
  const char *const class;
  const char *const name;
  const char *const title;
  const Bool isFullScreen;
  const FreeSetterFn freeSetterFn;
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
  const ActionChain *const startUpHook;
  const ActionChain *const endUpHook;
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

