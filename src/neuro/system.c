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

// Includes
#include "system.h"
#include "config.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Main variables
static Display *display_;
static int screen_;
static Window root_;
static Rectangle screen_region_;
static Rectangle hidden_region_;
static const int hidden_gaps_[ 4 ] = { 0 };
static Cursor cursors_[ NeuroSystemCursorCount ];
static Atom wm_atoms_[ NeuroSystemWmAtomCount ];
static Atom net_atoms_[ NeuroSystemNetAtomCount ];
static Color colors_[ NeuroSystemColorCount ];

// Recompile command
static const char recompile_cmd_output_[ NAME_MAX ];
static const char recompile_cmd_source_[ NAME_MAX ];
static const char *const recompile_cmd_[] = {
  "/usr/bin/cc",
  "-fpic",
  "-O3",
  "-o",
  recompile_cmd_output_,
  recompile_cmd_source_,
  "-L/usr/lib/neuro",
  "-l" PKG_NAME,
  "-lX11",
  "-lXrandr",
  "-pthread",
  NULL
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Starting error handler, used to check if another window manager is already running
static int xerror_handler_start(Display *d, XErrorEvent *ee) {
  (void)d;
  (void)ee;
  NeuroSystemError("xerror_handler_start - Another window manager is already running");
  return -1;
}

// Default error handler, which might call exit
static int xerror_handler(Display *d, XErrorEvent *ee) {
  (void)d;
  (void)ee;
  if (ee->error_code == BadWindow \
      || (ee->request_code == X_SetInputFocus     && ee->error_code == BadMatch)
      || (ee->request_code == X_PolyText8         && ee->error_code == BadDrawable)
      || (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable)
      || (ee->request_code == X_PolySegment       && ee->error_code == BadDrawable)
      || (ee->request_code == X_ConfigureWindow   && ee->error_code == BadMatch)
      || (ee->request_code == X_GrabButton        && ee->error_code == BadAccess)
      || (ee->request_code == X_GrabKey           && ee->error_code == BadAccess)
      || (ee->request_code == X_CopyArea          && ee->error_code == BadDrawable)
      || (ee->request_code == X_KillClient        && ee->error_code == BadValue))
    return 0;
  printf("error: Request code=%d, error code=%d\n", ee->request_code, ee->error_code);
  return -1;
}

static bool set_colors_cursors_atoms() {
  if (!NeuroConfigGet()->normal_border_color || !NeuroConfigGet()->current_border_color ||
      !NeuroConfigGet()->old_border_color || !NeuroConfigGet()->free_border_color ||
      !NeuroConfigGet()->urgent_border_color)
    return false;

  // Colors
  colors_[ NeuroSystemColorNormal ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->normal_border_color);
  colors_[ NeuroSystemColorCurrent ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->current_border_color);
  colors_[ NeuroSystemColorOld ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->old_border_color);
  colors_[ NeuroSystemColorFree ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->free_border_color);
  colors_[ NeuroSystemColorUrgent ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->urgent_border_color);

  // Cursors
  cursors_[ NeuroSystemCursorNormal ] = XCreateFontCursor(display_, XC_left_ptr);
  cursors_[ NeuroSystemCursorResize ] = XCreateFontCursor(display_, XC_bottom_right_corner);
  cursors_[ NeuroSystemCursorMove ] = XCreateFontCursor(display_, XC_fleur);

  // WM Atoms
  wm_atoms_[ NeuroSystemWmAtomProtocols ] = XInternAtom(display_, "WM_PROTOCOLS", false);
  wm_atoms_[ NeuroSystemWmAtomDeleteWindow ] = XInternAtom(display_, "WM_DELETE_WINDOW", false);

  // Net Atoms
  net_atoms_[ NeuroSystemNetAtomSupported ] = XInternAtom(display_, "_NET_SUPPORTED", false);
  net_atoms_[ NeuroSystemNetAtomState ] = XInternAtom(display_, "_NET_WM_STATE", false);
  net_atoms_[ NeuroSystemNetAtomName ] = XInternAtom(display_, "_NET_WM_NAME", false);
  net_atoms_[ NeuroSystemNetAtomActive ] = XInternAtom(display_, "_NET_ACTIVE_WINDOW", false);
  net_atoms_[ NeuroSystemNetAtomFullscreen ] = XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", false);
  net_atoms_[ NeuroSystemNetAtomStrut ] = XInternAtom(display_, "_NET_WM_STRUT", false);
  net_atoms_[ NeuroSystemNetAtomCloseWindow ] = XInternAtom(display_, "_NET_CLOSE_WINDOW", false);

  // EWMH support per view
  XChangeProperty(display_, root_, net_atoms_[ NeuroSystemNetAtomSupported ], XA_ATOM, 32, PropModeReplace,
      (unsigned char *)net_atoms_, NeuroSystemNetAtomCount);

  return true;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// X functions
bool NeuroSystemInit() {
  // WM global variables
  display_ = XOpenDisplay(NULL);
  if (!display_)
    return false;
  screen_ = DefaultScreen(display_);
  root_ = RootWindow(display_, screen_);

  // Get the regions
  const int width = XDisplayWidth(display_, screen_);
  const int height = XDisplayHeight(display_, screen_);
  screen_region_ = (Rectangle){ 0, 0, width, height };
  hidden_region_ = (Rectangle){ width, height, 1920, 1080 };

  // Set colors, cursors and atoms
  if (!set_colors_cursors_atoms())
    return false;

  // Check if other window manager is already running
  XSetErrorHandler(xerror_handler_start);

  // Setup root window mask
  XSetWindowAttributes wa;
  wa.cursor = NeuroSystemGetCursor(NeuroSystemCursorNormal);
  wa.event_mask = ROOT_MASK;
  XChangeWindowAttributes(display_, root_, CWEventMask|CWCursor, &wa);
  XSelectInput(display_, root_, wa.event_mask);
  XSync(display_, false);

  // Set custom X error handler
  XSetErrorHandler(xerror_handler);
  XSync(display_, false);

  // Grab key bindings
  NeuroSystemGrabKeys(root_, NeuroConfigGet()->key_list);

  return true;
}

void NeuroSystemStop() {
  XFreeCursor(display_, NeuroSystemGetCursor(NeuroSystemCursorNormal));
  XFreeCursor(display_, NeuroSystemGetCursor(NeuroSystemCursorResize));
  XFreeCursor(display_, NeuroSystemGetCursor(NeuroSystemCursorMove));
  XCloseDisplay(display_);
}

// Basic functions
Display *NeuroSystemGetDisplay() {
  return display_;
}

Window NeuroSystemGetRoot() {
  return root_;
}

int NeuroSystemGetScreen() {
  return screen_;
}

const Rectangle *NeuroSystemGetScreenRegion() {
  return &screen_region_;
}

const Rectangle *NeuroSystemGetHiddenRegion() {
  return &hidden_region_;
}

const int *NeuroSystemGetHiddenGaps() {
  return hidden_gaps_;
}

Cursor NeuroSystemGetCursor(NeuroSystemCursor c) {
  return cursors_[ c ];
}

Atom NeuroSystemGetWmAtom(NeuroSystemWmAtom a) {
  return wm_atoms_[ a ];
}

Atom NeuroSystemGetNetAtom(NeuroSystemNetAtom a) {
  return net_atoms_[ a ];
}

Color NeuroSystemGetColor(NeuroSystemColor c) {
  return colors_[ c ];
}

Color NeuroSystemGetColorFromHex(const char* color) {
  assert(color);
  XColor c;
  Colormap map = DefaultColormap(display_, screen_);
  if (!XAllocNamedColor(display_, map, color, &c, &c))
    NeuroSystemError("NeuroSystemGetColorFromHex - Could not allocate color");
  return c.pixel;
}

// System functions
const char * const *NeuroSystemGetRecompileCommand(const char **output, const char **source) {
  snprintf((char *)recompile_cmd_output_, NAME_MAX, "%s/." PKG_NAME "/" PKG_MYNAME, getenv("HOME"));
  snprintf((char *)recompile_cmd_source_, NAME_MAX, "%s/." PKG_NAME "/" PKG_NAME ".c", getenv("HOME"));
  if (output)
    *output = recompile_cmd_output_;
  if (source)
    *source = recompile_cmd_source_;
  return recompile_cmd_;
}

void NeuroSystemChangeWmName(const char *name) {
  assert(name);
  const Atom netwmcheck = XInternAtom(display_, "_NET_SUPPORTING_WM_CHECK", false);
  const Atom netwmname = XInternAtom(display_, "_NET_WM_NAME", false);
  const Atom utf8_str = XInternAtom(display_, "UTF8_STRING", false);
  XChangeProperty(display_, root_, netwmcheck, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&root_, 1);
  XChangeProperty(display_, root_, netwmname, utf8_str, 8, PropModeReplace, (unsigned char *)name, strlen(name));
}

void NeuroSystemChangeProcName(const char *name) {
  assert(name);
  prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);
}

pid_t NeuroSystemGetWmPid() {
  char pidstr[ NAME_MAX ];
  FILE *cmd = popen("/usr/bin/pidof -s " PKG_MYNAME, "r");
  fgets(pidstr, NAME_MAX, cmd);
  return (pid_t)strtoul(pidstr, NULL, 10);
}

bool NeuroSystemSpawn(const char *const *cmd, pid_t *p) {
  assert(cmd);
  pid_t pid = fork();
  if (pid == -1)
    return false;
  if (!pid) {  // Child
    if (display_)
      close(ConnectionNumber(display_));
    setsid();
    execvp(cmd[ 0 ], (char *const *)cmd);
    NeuroSystemError("NeuroSystemSpawn - Could not execvp");
  }
  if (p)
    *p = pid;
  return true;
}

int NeuroSystemSpawnPipe(const char *const *cmd, pid_t *p) {
  assert(cmd);
  int filedes[ 2 ];
  if (pipe(filedes))
    return -1;
  pid_t pid = fork();
  if (pid < 0)
    return -1;
  if (pid == 0) {  // Child
    if (display_)
      close(ConnectionNumber(display_));
    setsid();
    dup2(filedes[ 0 ], STDIN_FILENO);
    close(filedes[ 0 ]);
    execvp(cmd[ 0 ], (char *const *)cmd);
    exit(EXIT_FAILURE);
  }
  if (p)
    *p = pid;
  return filedes[ 1 ];
}

void NeuroSystemError(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

// Binding functions
void NeuroSystemGrabKeys(Window w, const Key *const *key_list) {
  if (!key_list)
    return;
  XUngrabKey(display_, AnyKey, AnyModifier, w);
  KeyCode code;
  const Key *k;
  for (size_t i = 0U; key_list[ i ]; ++i) {
    k = key_list[ i ];
    code = XKeysymToKeycode(display_, k->key);
    if (code)
      XGrabKey(display_, code, k->mod, w, true, GrabModeAsync, GrabModeAsync);
  }
}

void NeuroSystemUngrabKeys(Window w, const Key *const *key_list) {
  if (!key_list)
    return;
  KeyCode code;
  const Key *k;
  for (size_t i = 0U; key_list[ i ]; ++i) {
    k = key_list[ i ];
    code = XKeysymToKeycode(display_, k->key);
    if (code)
      XUngrabKey(display_, code, k->mod, w);
  }
}

void NeuroSystemGrabButtons(Window w, const Button *const *button_list) {
  if (!button_list)
    return;
  XUngrabButton(display_, AnyButton, AnyModifier, w);
  const Button *b;
  for (size_t i = 0U; button_list[ i ]; ++i) {
    b = button_list[ i ];
    XGrabButton(display_, b->button, b->mod, w, false, ButtonPressMask|ButtonReleaseMask, GrabModeAsync, GrabModeSync,
        None, None);
  }
}

void NeuroSystemUngrabButtons(Window w, const Button *const *button_list) {
  if (!button_list)
    return;
  const Button *b;
  for (size_t i = 0U; button_list[ i ]; ++i) {
    b = button_list[ i ];
    if (b->ungrab_on_focus)
      XUngrabButton(display_, b->button, b->mod, w);
  }
}

