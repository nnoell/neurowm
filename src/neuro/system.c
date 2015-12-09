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


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Main variables (Do not make them static)
Display *const display_;
const int screen_;
const Window root_;
const int x_pos_ = 0;
const int y_pos_ = 0;
const int x_res_;
const int y_res_;
const Rectangle screen_region_;
const Cursor cursors_[ NeuroSystemCursorCount ];
const Atom wm_atoms_[ NeuroSystemWmAtomCount ];
const Atom net_atoms_[ NeuroSystemNetAtomCount ];
const Color colors_[ NeuroSystemColorCount ];

// Configuration
static const Configuration *configuration_;

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
  "-l" WM_NAME,
  "-lX11",
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

static void set_colors_cursors_atoms() {
  // Colors
  *(Color *)&colors_[ NeuroSystemColorNormal ] = NeuroSystemGetColorFromHex(configuration_->normal_border_color);
  *(Color *)&colors_[ NeuroSystemColorCurrent ] = NeuroSystemGetColorFromHex(configuration_->current_border_color);
  *(Color *)&colors_[ NeuroSystemColorOld ] = NeuroSystemGetColorFromHex(configuration_->old_border_color);
  *(Color *)&colors_[ NeuroSystemColorFree ] = NeuroSystemGetColorFromHex(configuration_->free_border_color);
  *(Color *)&colors_[ NeuroSystemColorUrgent ] = NeuroSystemGetColorFromHex(configuration_->urgent_border_color);

  // Cursors
  *(Cursor *)&cursors_[ NeuroSystemCursorNormal ] = XCreateFontCursor(display_, XC_left_ptr);
  *(Cursor *)&cursors_[ NeuroSystemCursorResize ] = XCreateFontCursor(display_, XC_bottom_right_corner);
  *(Cursor *)&cursors_[ NeuroSystemCursorMove ] = XCreateFontCursor(display_, XC_fleur);

  // WM Atoms
  *(Atom *)&wm_atoms_[ NeuroSystemWmAtomProtocols ] = XInternAtom(display_, "WM_PROTOCOLS", False);
  *(Atom *)&wm_atoms_[ NeuroSystemWmAtomDeleteWindow ] = XInternAtom(display_, "WM_DELETE_WINDOW", False);

  // Net Atoms
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomSupported ] = XInternAtom(display_, "_NET_SUPPORTED", False);
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomState ] = XInternAtom(display_, "_NET_WM_STATE", False);
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomName ] = XInternAtom(display_, "_NET_WM_NAME", False);
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomActive ] = XInternAtom(display_, "_NET_ACTIVE_WINDOW", False);
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomFullscreen ] = XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", False);
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomStrut ] = XInternAtom(display_, "_NET_WM_STRUT", False);
  *(Atom *)&net_atoms_[ NeuroSystemNetAtomCloseWindow ] = XInternAtom(display_, "_NET_CLOSE_WINDOW", False);

  // EWMH support per view
  XChangeProperty(display_, root_, net_atoms_[ NeuroSystemNetAtomSupported ], XA_ATOM, 32, PropModeReplace,
      (unsigned char *)net_atoms_, NeuroSystemNetAtomCount);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Configuration functions
Bool NeuroSystemSetConfiguration(const Configuration *c) {
  if (!c)
    return False;
  configuration_ = c;
  return True;
}

const Configuration *NeuroSystemGetConfiguration() {
  return configuration_;
}

// X functions
Bool NeuroSystemInit() {
  // WM global variables
  *(Display **)&display_ = XOpenDisplay(NULL);
  if (!display_)
    return False;
  *(int *)&screen_ = DefaultScreen(display_);
  *(Window *)&root_ = RootWindow(display_, screen_);
  *(int *)&x_res_ = XDisplayWidth(display_, screen_);
  *(int *)&y_res_ = XDisplayHeight(display_, screen_);
  *(Rectangle *)&screen_region_ = (Rectangle){ .x = y_pos_, .y = x_pos_, .w = x_res_, .h = y_res_ };

  // Set colors, cursors and atoms
  set_colors_cursors_atoms();

  // Check if other window manager is already running
  XSetErrorHandler(xerror_handler_start);

  // Setup root window mask
  XSetWindowAttributes wa;
  wa.cursor = NeuroSystemGetCursor(NeuroSystemCursorNormal);
  wa.event_mask = ROOT_MASK;
  XChangeWindowAttributes(display_, root_, CWEventMask|CWCursor, &wa);
  XSelectInput(display_, root_, wa.event_mask);
  XSync(display_, False);

  // Set custom X error handler
  XSetErrorHandler(xerror_handler);
  XSync(display_, False);

  // Grab key bindings
  NeuroSystemGrabKeys(root_);

  return True;
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

int NeuroSystemGetXPos() {
  return x_pos_;
}

int NeuroSystemGetYPos() {
  return y_pos_;
}

int NeuroSystemGetXRes() {
  return x_res_;
}

int NeuroSystemGetYRes() {
  return y_res_;
}

const Rectangle *NeuroSystemGetScreenRegion() {
  return &screen_region_;
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
  snprintf((char *)recompile_cmd_output_, NAME_MAX, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  snprintf((char *)recompile_cmd_source_, NAME_MAX, "%s/." WM_NAME "/" WM_NAME ".c", getenv("HOME"));
  if (output)
    *output = recompile_cmd_output_;
  if (source)
    *source = recompile_cmd_source_;
  return recompile_cmd_;
}

void NeuroSystemChangeNeurowmName(const char *name) {
  assert(name);
  const Atom netwmcheck = XInternAtom(display_, "_NET_SUPPORTING_WM_CHECK", False);
  const Atom netwmname = XInternAtom(display_, "_NET_WM_NAME", False);
  const Atom utf8_str = XInternAtom(display_, "UTF8_STRING", False);
  XChangeProperty(display_, root_, netwmcheck, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&root_, 1);
  XChangeProperty(display_, root_, netwmname, utf8_str, 8, PropModeReplace, (unsigned char *)name, strlen(name));
}

void NeuroSystemChangeProcName(const char *newname) {
  assert(newname);
  prctl(PR_SET_NAME, (unsigned long)newname, 0, 0, 0);
}

int NeuroSystemSpawn(const char *const *cmd, pid_t *p) {
  assert(cmd);
  pid_t pid = fork();
  if (pid == -1)
    return -1;
  if (!pid) {  // Child
    if (display_)
      close(ConnectionNumber(display_));
    setsid();
    execvp(cmd[ 0 ], (char *const *)cmd);
    NeuroSystemError("NeuroSystemSpawn - Could not execvp");
  }
  if (p)
    *p = pid;
  return 0;
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
void NeuroSystemGrabKeys(Window w) {
  XUngrabKey(display_, AnyKey, AnyModifier, w);
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; configuration_->keys[ i ]; ++i) {
    k = configuration_->keys[ i ];
    code = XKeysymToKeycode(display_, k->key);
    if (code)
      XGrabKey(display_, code, k->mod, w, True, GrabModeAsync, GrabModeAsync);
  }
}

void NeuroSystemUngrabKeys(Window w) {
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; configuration_->keys[ i ]; ++i) {
    k = configuration_->keys[ i ];
    code = XKeysymToKeycode(display_, k->key);
    if (code)
      XUngrabKey(display_, code, k->mod, w);
  }
}

void NeuroSystemGrabButtons(Window w) {
  XUngrabButton(display_, AnyButton, AnyModifier, w);
  const Button *b;
  int i;
  for (i=0; configuration_->buttons[ i ]; ++i) {
    b = configuration_->buttons[ i ];
    XGrabButton(display_, b->button, b->mod, w, False, ButtonPressMask|ButtonReleaseMask,
        GrabModeAsync, GrabModeSync, None, None);
  }
}

void NeuroSystemUngrabButtons(Window w) {
  const Button *b;
  int i;
  for (i=0; configuration_->buttons[ i ]; ++i) {
    b = configuration_->buttons[ i ];
    if (b->ungrab_on_focus)
      XUngrabButton(display_, b->button, b->mod, w);
  }
}

