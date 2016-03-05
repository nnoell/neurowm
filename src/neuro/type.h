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

// Conditional Includes
#ifdef XRANDR
  #include <X11/extensions/Xrandr.h>
#endif

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
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <assert.h>

// Arg constructors
#define NEURO_ARG_NULL     {.pointer_ = NULL}
#define NEURO_ARG_PTR(X)   {.pointer_ = (X)}
#define NEURO_ARG_CHAR(X)  {.char_ = (X)}
#define NEURO_ARG_INT(X)   {.int_ = (X)}
#define NEURO_ARG_UINT(X)  {.uint_ = (X)}
#define NEURO_ARG_IDX(X)   {.idx_ = (X)}
#define NEURO_ARG_FLOAT(X) {.float_ = (X)}
#define NEURO_ARG_STR(X)   {.string_ = (X)}
#define NEURO_ARG_CMD(X)   {.command_ = (X)}
#define NEURO_ARG_LMOD(X)  {.LayoutMod_ = (X)}
#define NEURO_ARG_GAF(X)   {.GenericArgFn_ = (X)}
#define NEURO_ARG_FSF(X)   {.GenericArgFn_.FreeSetterFn_ = (X)}
#define NEURO_ARG_CSF(X)   {.GenericArgFn_.ClientSelectorFn_ = (X)}
#define NEURO_ARG_WSF(X)   {.GenericArgFn_.WorkspaceSelectorFn_ = (X)}
#define NEURO_ARG_MSF(X)   {.GenericArgFn_.MonitorSelectorFn_ = (X)}

// Arg functions
#define NEURO_ARG_PTR_GET(X)   ((X).pointer_)
#define NEURO_ARG_CHAR_GET(X)  ((X).char_)
#define NEURO_ARG_INT_GET(X)   ((X).int_)
#define NEURO_ARG_UINT_GET(X)  ((X).uint_)
#define NEURO_ARG_IDX_GET(X)   ((X).idx_)
#define NEURO_ARG_FLOAT_GET(X) ((X).float_)
#define NEURO_ARG_STR_GET(X)   ((X).string_)
#define NEURO_ARG_CMD_GET(X)   ((X).command_)
#define NEURO_ARG_LMOD_GET(X)  ((X).LayoutMod_)
#define NEURO_ARG_GAF_GET(X)   ((X).GenericArgFn_)
#define NEURO_ARG_FLF_GET(X)   ((X).GenericArgFn_.FreeSetterFn_)
#define NEURO_ARG_CSF_GET(X)   ((X).GenericArgFn_.ClientSelectorFn_)
#define NEURO_ARG_WSF_GET(X)   ((X).GenericArgFn_.WorkspaceSelectorFn_)
#define NEURO_ARG_MSF_GET(X)   ((X).GenericArgFn_.MonitorSelectorFn_)

// Maybe Arg constructors
#define NEURO_MAYBE_ARG_NOTHING {.is_nothing = true, .value = NEURO_ARG_NULL}
#define NEURO_MAYBE_ARG_JUST(X) {.is_nothing = false, .value = X}

// Maybe Arg functions
#define NEURO_MAYBE_ARG_IS_NOTHING(X) ((X).is_nothing)
#define NEURO_MAYBE_ARG_GET_JUST(X)   ((X).value)

// Chainned Actions
#define NEURO_CHAIN_NULL(X) {.action_list = (X), .arg = NEURO_MAYBE_ARG_NOTHING}
#define NEURO_CHAIN(X, Y)   {.action_list = (X), .arg = NEURO_MAYBE_ARG_JUST(Y)}

// Default sizes
#define NEURO_NAME_SIZE_MAX    256
#define NEURO_ARRANGE_ARGS_MAX 4

// Reload
#define NEURO_EXIT_RELOAD ((int)'R')

// NeuroClientPtrPtr
#define NEURO_CLIENT_PTR(X) (*(X))

// Config
#define NEURO_NULL_MASK 0


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// ENUM TYPES ----------------------------------------------------------------------------------------------------------

// LayoutMod
enum NeuroLayoutMod {
  NEURO_LAYOUT_MOD_NULL = 0,
  NEURO_LAYOUT_MOD_MIRROR = 1 << 0,
  NEURO_LAYOUT_MOD_REFLECTX = 1 << 1,
  NEURO_LAYOUT_MODE_REFLECTY = 1 << 2
};
typedef enum NeuroLayoutMod NeuroLayoutMod;

// RuleFixedPosition
enum NeuroFixedPosition {
  NEURO_FIXED_POSITION_NULL = 0,
  NEURO_FIXED_POSITION_UP = 1,
  NEURO_FIXED_POSITION_DOWN = 2,
  NEURO_FIXED_POSITION_LEFT = 3,
  NEURO_FIXED_POSITION_RIGHT = 4
};
typedef enum NeuroFixedPosition NeuroFixedPosition;


// INDEX TYPES ---------------------------------------------------------------------------------------------------------

// NeuroIndex
typedef size_t NeuroIndex;


// GEOMETRY TYPES ------------------------------------------------------------------------------------------------------

// NeuroPoint
struct NeuroPoint {
  int x;
  int y;
};
typedef struct NeuroPoint NeuroPoint;

// NeuroRectangle
struct NeuroRectangle {
  NeuroPoint p;  // top left point position
  int w;         // Width
  int h;         // Height
};
typedef struct NeuroRectangle NeuroRectangle;

// NeuroFreeSetterFn
typedef void (*NeuroFreeSetterFn)(NeuroRectangle *a, const NeuroRectangle *r);


// CLIENT TYPES --------------------------------------------------------------------------------------------------------

// NeuroClient
struct NeuroClient {
  NeuroRectangle float_region;
  Window win;
  NeuroIndex ws;
  bool is_nsp;
  char class[ NEURO_NAME_SIZE_MAX ];
  char name[ NEURO_NAME_SIZE_MAX ];
  char title[ NEURO_NAME_SIZE_MAX ];
  bool is_fullscreen;
  NeuroFreeSetterFn free_setter_fn;
  NeuroFixedPosition fixed_pos;
  float fixed_size;
  bool is_urgent;
};
typedef struct NeuroClient NeuroClient;

// NeuroClientPtrPtr
typedef NeuroClient **NeuroClientPtrPtr;

// NeuroClientTesterFn
typedef bool (*NeuroClientTesterFn)(NeuroClientPtrPtr c, const void *p);

// NeuroClientSelectorFn
typedef NeuroClientPtrPtr (*NeuroClientSelectorFn)(NeuroClientPtrPtr c);


// DZEN TYPES ----------------------------------------------------------------------------------------------------------

// NeuroDzenBox
struct NeuroDzenBox {
  const char *bg_color;
  const char *fg_color;
  const char *box_color;
  const char *left_icon;
  const char *right_icon;
  int box_height;
};
typedef struct NeuroDzenBox NeuroDzenBox;

// NeuroDzenClickableArea
typedef struct NeuroDzenClickableArea NeuroDzenClickableArea;
struct NeuroDzenClickableArea {
  const char *left_click;
  const char *middle_click;
  const char *right_click;
  const char *wheel_up;
  const char *wheel_down;
};

// NeuroDzenFlags
struct NeuroDzenFlags {
  int x, y, w, h;
  const char *fg_color;
  const char *bg_color;
  char align;
  const char *font;
  const char *event;
  const char *extras;
};
typedef struct NeuroDzenFlags NeuroDzenFlags;

// NeuroDzenLoggerFn
typedef struct NeuroMonitor NeuroMonitor;  // Forward declaration
typedef void (*const NeuroDzenLoggerFn)(const NeuroMonitor *m, char *);

// NeuroDzenPanel
struct NeuroDzenPanel {
  const NeuroDzenFlags *const df;
  const NeuroDzenLoggerFn *const loggers;
  const char *const sep;
  const uint32_t refresh_rate;
};
typedef struct NeuroDzenPanel NeuroDzenPanel;


// MONITOR TYPES -----------------------------------------------------------------------------------------------------

// NeuroMonitorConf
struct NeuroMonitorConf {
  const char *const name;
  const NeuroIndex default_ws;
  const int gaps[ 4 ];
  const NeuroDzenPanel *const *const dzen_panel_list;
};
typedef struct NeuroMonitorConf NeuroMonitorConf;

// NeuroMonitor
struct NeuroMonitor {
  const char *name;
  NeuroIndex default_ws;
  const int *gaps;
  NeuroRectangle region;  // The region does not include the gaps (region + gaps = total_monitor_area)
  const NeuroDzenPanel *const *dzen_panel_list;
};
typedef struct NeuroMonitor NeuroMonitor;

// NeuroMonitorSelectorFn
typedef const NeuroMonitor *(*NeuroMonitorSelectorFn)(const NeuroMonitor *m);


// WORKSPACE TYPES -----------------------------------------------------------------------------------------------------

// NeuroWorkspaceSelectorFn
typedef NeuroIndex (*NeuroWorkspaceSelectorFn)(void);


// FUNCTION TYPES ------------------------------------------------------------------------------------------------------

// NeuroArgFn (Needed to wrap the function pointers into a union/struct so that they can be treated as data)
union NeuroArgFn {
  NeuroFreeSetterFn FreeSetterFn_;
  NeuroClientSelectorFn ClientSelectorFn_;
  NeuroWorkspaceSelectorFn WorkspaceSelectorFn_;
  NeuroMonitorSelectorFn MonitorSelectorFn_;
};
typedef union NeuroArgFn NeuroArgFn;

// NeuroArg
union NeuroArg {
  const void *pointer_;
  char char_;
  int int_;
  unsigned int uint_;
  NeuroIndex idx_;
  NeuroLayoutMod LayoutMod_;
  float float_;
  const char *string_;
  const char *const *command_;
  NeuroArgFn GenericArgFn_;
};
typedef union NeuroArg NeuroArg;

// NeuroMaybeArg
struct NeuroMaybeArg {
  bool is_nothing;
  NeuroArg value;
};
typedef struct NeuroMaybeArg NeuroMaybeArg;

// NeuroFn
typedef void (*NeuroFn)(NeuroArg arg);


// ACTION TYPES --------------------------------------------------------------------------------------------------------

// NeuroAction
struct NeuroAction {
  NeuroFn handler;
  NeuroArg arg;
};
typedef struct NeuroAction NeuroAction;

// NeuroActionChain
struct NeuroActionChain {
  const NeuroAction *const *action_list;
  NeuroMaybeArg arg;
};
typedef struct NeuroActionChain NeuroActionChain;


// LAYOUT TYPES --------------------------------------------------------------------------------------------------------

// NeuroColor
typedef unsigned long NeuroColor;

// NeuroColorSetterFn
typedef NeuroColor (*NeuroColorSetterFn)(NeuroClientPtrPtr c);

// NeuroBorderSetterFn
typedef int (*NeuroBorderSetterFn)(NeuroClientPtrPtr c);

// NeuroArrange
struct NeuroArrange {
  NeuroIndex size;                       // Number of tiled clients
  NeuroRectangle region;                  // Tiled layout region
  NeuroRectangle **client_regions;        // Region of each client
  NeuroRectangle **client_float_regions;  // Float region of each client
  NeuroArg *parameters;            // Parameters of the arrange
};
typedef struct NeuroArrange NeuroArrange;

// NeuroArrangerFn
typedef NeuroArrange *(*NeuroArrangerFn)(NeuroArrange *);

// NeuroLayout
struct NeuroLayout {
  NeuroArrangerFn arranger_fn;
  NeuroColorSetterFn border_color_setter_fn;
  NeuroBorderSetterFn border_width_setter_fn;
  NeuroBorderSetterFn border_gap_setter_fn;
  const float *region;
  NeuroLayoutMod mod;
  bool follow_mouse;
  NeuroArg parameters[ NEURO_ARRANGE_ARGS_MAX ];
};
typedef struct NeuroLayout NeuroLayout;


// CONFIG TYPES --------------------------------------------------------------------------------------------------------

// NeuroLayoutConf
struct NeuroLayoutConf {
  const char *const name;
  const NeuroArrangerFn arranger_fn;
  const NeuroColorSetterFn border_color_setter_fn;
  const NeuroBorderSetterFn border_width_setter_fn;
  const NeuroBorderSetterFn border_gap_setter_fn;
  const float region[ 4 ];
  const NeuroLayoutMod mod;
  const bool follow_mouse;
  const NeuroArg parameters[ NEURO_ARRANGE_ARGS_MAX ];
};
typedef struct NeuroLayoutConf NeuroLayoutConf;

// NeuroWorkspace
struct NeuroWorkspace {
  const char *const name;
  const NeuroLayoutConf *const *const layouts;
  const NeuroLayoutConf *const *const toggled_layouts;
};
typedef struct NeuroWorkspace NeuroWorkspace;

// NeuroKey
struct NeuroKey {
  const unsigned int mod;
  const KeySym key;
  const NeuroActionChain action_chain;
};
typedef struct NeuroKey NeuroKey;

// NeuroButton
struct NeuroButton {
  const unsigned int mod;
  const unsigned int button;
  const NeuroActionChain action_chain;
  const bool ungrab_on_focus;
};
typedef struct NeuroButton NeuroButton;

// NeuroRule
struct NeuroRule {
  const char *const class;
  const char *const name;
  const char *const title;
  const bool is_fullscreen;
  const NeuroFreeSetterFn free_setter_fn;
  const NeuroFixedPosition fixed_pos;
  const float fixed_size;
  const NeuroWorkspaceSelectorFn workspace_selector_fn;
  const bool follow;
};
typedef struct NeuroRule NeuroRule;

// NeuroConfiguration
struct NeuroConfiguration {
  const NeuroActionChain init_action_chain;
  const NeuroActionChain stop_action_chain;
  const char *const normal_border_color;
  const char *const current_border_color;
  const char *const old_border_color;
  const char *const free_border_color;
  const char *const urgent_border_color;
  const int border_width;
  const int border_gap;
  const NeuroMonitorConf *const *const monitor_list;
  const NeuroWorkspace *const *const workspace_list;
  const NeuroRule *const *const rule_list;
  const NeuroKey *const *const key_list;
  const NeuroButton *const *const button_list;
};
typedef struct NeuroConfiguration NeuroConfiguration;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Creation and Destruction
NeuroClient *NeuroTypeNewClient(Window w, const XWindowAttributes *wa);
void NeuroTypeDeleteClient(NeuroClient *c);

// Basic Functions
NeuroIndex NeuroTypeArrayLength(const void *const *array_ptr);

