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

#pragma once

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
#include <asm-generic/errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <assert.h>

// Arg constructors
#define ARG_NULL     {.pointer_ = NULL}
#define ARG_PTR(X)   {.pointer_ = (X)}
#define ARG_CHAR(X)  {.char_ = (X)}
#define ARG_INT(X)   {.int_ = (X)}
#define ARG_FLOAT(X) {.float_ = (X)}
#define ARG_STR(X)   {.string_ = (X)}
#define ARG_CMD(X)   {.command_ = (X)}
#define ARG_LMOD(X)  {.LayoutMod_ = (X)}
#define ARG_GAF(X)   {.GenericArgFn_ = (X)}
#define ARG_FSF(X)   {.GenericArgFn_.FreeSetterFn_ = (X)}
#define ARG_CSF(X)   {.GenericArgFn_.ClientSelectorFn_ = (X)}
#define ARG_WSF(X)   {.GenericArgFn_.WorkspaceSelectorFn_ = (X)}

// Arg functions
#define ARG_PTR_GET(X)   ((X).pointer_)
#define ARG_CHAR_GET(X)  ((X).char_)
#define ARG_INT_GET(X)   ((X).int_)
#define ARG_FLOAT_GET(X) ((X).float_)
#define ARG_STR_GET(X)   ((X).string_)
#define ARG_CMD_GET(X)   ((X).command_)
#define ARG_LMOD_GET(X)  ((X).LayoutMod_)
#define ARG_GAF_GET(X)   ((X).GenericArgFn_)
#define ARG_FLF_GET(X)   ((X).GenericArgFn_.FreeSetterFn_)
#define ARG_CSF_GET(X)   ((X).GenericArgFn_.ClientSelectorFn_)
#define ARG_WSF_GET(X)   ((X).GenericArgFn_.WorkspaceSelectorFn_)

// Maybe Arg constructors
#define MAYBE_ARG_NOTHING       {.is_nothing = True, .value = ARG_NULL}
#define MAYBE_ARG_JUST(X)       {.is_nothing = False, .value = X}

// Maybe Arg functions
#define MAYBE_ARG_IS_NOTHING(X) ((X).is_nothing)
#define MAYBE_ARG_GET_JUST(X)   ((X).value)

// Chainned Actions
#define CHAIN_NULL(X)     {.action_list = (X), .arg = MAYBE_ARG_NOTHING}
#define CHAIN(X, Y)       {.action_list = (X), .arg = MAYBE_ARG_JUST(Y)}

// Default sizes
#define NAME_MAX    256
#define COLOR_MAX   8
#define ARRSET_MAX  4

// Reload
#define EXIT_RELOAD ((int)'R')

// ClientPtrPtr
#define CLI_GET(X)  (**(X))

// Config
#define NULL_MASK 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ENUM TYPES ----------------------------------------------------------------------------------------------------------

// LayoutMod
enum LayoutMod {
  LayoutModNull = 0,
  LayoutModMirror = 1 << 0,
  LayoutModReflectX = 1 << 1,
  LayoutModReflectY = 1 << 2
};
typedef enum LayoutMod LayoutMod;

// RuleFixedPosition
enum RuleFixedPosition {
  RuleFixedPositionNull = 0,
  RuleFixedPositionUp = 1,
  RuleFixedPositionDown = 2,
  RuleFixedPositionLeft = 3,
  RuleFixedPositionRigth = 4
};
typedef enum RuleFixedPosition RuleFixedPosition;


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
  Bool is_nsp;
  char class[ NAME_MAX ];
  char name[ NAME_MAX ];
  char title[ NAME_MAX ];
  Rectangle float_region;
  Bool is_hidden;
  Bool is_fullscreen;
  FreeSetterFn free_setter_fn;
  RuleFixedPosition fixed_pos;
  int fixed_size;
  Bool is_urgent;
};

// ClientPtrPtr
typedef Client *const *ClientPtrPtr;

// ClientTesterFn
typedef Bool (*ClientTesterFn)(ClientPtrPtr c, const void *p);

// ClientSelectorFn
typedef ClientPtrPtr (*ClientSelectorFn)(ClientPtrPtr c);


// WORKSPACE TYPES -----------------------------------------------------------------------------------------------------

// WorkspaceSelectorFn
typedef int (*WorkspaceSelectorFn)();


// GENERIC TYPES -------------------------------------------------------------------------------------------------------

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
  const LayoutMod LayoutMod_;
  const float float_;
  const char *const string_;
  const char *const *const command_;
  const GenericArgFn GenericArgFn_;
};

// GenericMaybeArg
typedef struct GenericMaybeArg GenericMaybeArg;
struct GenericMaybeArg {
  const Bool is_nothing;
  const GenericArg value;
};

// GenericFn
typedef void (*GenericFn)(GenericArg arg);


// ACTION TYPES --------------------------------------------------------------------------------------------------------

// Action
typedef struct Action Action;
struct Action {
  const GenericFn handler;
  const GenericArg arg;
};

// ActionChain
typedef struct ActionChain ActionChain;
struct ActionChain {
  const Action *const *const action_list;
  const GenericMaybeArg arg;
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
  int size;                          // Number of tiled clients
  Rectangle region;                  // Tiled layout region
  Rectangle **client_regions;        // Region of each client
  Rectangle **client_float_regions;  // Float region of each client
  GenericArg *arrange_settings;      // Settings of the arrange
};

// ArrangerFn
typedef Arrange *(*ArrangerFn)(Arrange *);

// Layout
typedef struct Layout Layout;
struct Layout {
  const ArrangerFn arranger_fn;
  const ColorSetterFn border_color_setter_fn;
  const BorderSetterFn border_width_setter_fn;
  const BorderSetterFn border_gap_setter_fn;
  const float *const region;
  LayoutMod mod;
  Bool follow_mouse;
  GenericArg arrange_settings[ ARRSET_MAX ];
};


// CONFIG TYPES --------------------------------------------------------------------------------------------------------

// LayoutConf
typedef struct LayoutConf LayoutConf;
struct LayoutConf {
  const char *const name;
  const ArrangerFn arranger_fn;
  const ColorSetterFn border_color_setter_fn;
  const BorderSetterFn border_width_setter_fn;
  const BorderSetterFn border_gap_setter_fn;
  const float region[ 4 ];
  const LayoutMod mod;
  const Bool follow_mouse;
  const GenericArg arrange_settings[ ARRSET_MAX ];
};

// Workspace
typedef struct Workspace Workspace;
struct Workspace {
  const char *const name;
  const int gaps[ 4 ];
  const LayoutConf *const *const layouts;
  const LayoutConf *const *const toggled_layouts;
};

// Key
typedef struct Key Key;
struct Key {
  const unsigned int mod;
  const KeySym key;
  const ActionChain action_chain;
};

// Button
typedef struct Button Button;
struct Button {
  const unsigned int mod;
  const unsigned int button;
  const ActionChain action_chain;
  const Bool ungrab_on_focus;
};

// Rule
typedef struct Rule Rule;
struct Rule {
  const char *const class;
  const char *const name;
  const char *const title;
  const Bool is_fullscreen;
  const FreeSetterFn free_setter_fn;
  const RuleFixedPosition fixed_pos;
  const float fixed_size;
  const WorkspaceSelectorFn workspace_selector_fn;
  const Bool follow;
};

// BoxPP
typedef struct BoxPP BoxPP;
struct BoxPP {
  const char *const bg_color;
  const char *const fg_color;
  const char *const box_color;
  const char *const left_icon;
  const char *const right_icon;
  const int box_height;
};

// CA
typedef struct CA CA;
struct CA {
  const char *const left_click;
  const char *const middle_click;
  const char *const right_click;
  const char *const wheel_up;
  const char *const wheel_down;
};

// DzenFlags
typedef struct DzenFlags DzenFlags;
struct DzenFlags {
  const int x, y, w, h;
  const char *const fg_color;
  const char *const bg_color;
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
  const int refresh_rate;
};

// Configuration
typedef struct Configuration Configuration;
struct Configuration {
  const ActionChain init_action_chain;
  const ActionChain stop_action_chain;
  const char *const normal_border_color;
  const char *const current_border_color;
  const char *const old_border_color;
  const char *const free_border_color;
  const char *const urgent_border_color;
  const int border_width;
  const int border_gap;
  const Workspace *const *const workspace_list;
  const Rule *const *const rule_list;
  const DzenPanel *const *const dzen_panel_list;
  const Key *const *const key_list;
  const Button *const *const button_list;
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Creation and Destruction
Client *NeuroTypeNewClient(Window w, const XWindowAttributes *wa);
void NeuroTypeDeleteClient(Client *c);

// Basic Functions
size_t NeuroTypeArrayLength(const void *const *arrayPtr);

