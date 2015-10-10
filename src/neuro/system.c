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

// Main variables
Display *const display_;
const int screen_;
const Window root_;
const int x_res_;
const int y_res_;
const Rectangle screen_region_;
const Cursor cursors[ NeuroSystemCursorLast ];
const Atom wmatoms[ NeuroSystemWmAtomLast ];
const Atom netatoms[ NeuroSystemNetAtomLast ];

// Global configuration
static const char *normBorderColor;
static const char *currBorderColor;
static const char *prevBorderColor;
static const char *freeBorderColor;
static const char *urgtBorderColor;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Configuration
const Color normBorderColorS;
const Color currBorderColorS;
const Color prevBorderColorS;
const Color freeBorderColorS;
const Color urgtBorderColorS;
const int borderWidthS;
const int borderGapS;
const Key *const *const keyBindingsS;
const Button *const *const buttonBindingsS;
const Rule *const *const ruleSetS;
const Workspace *const *const workspaceSetS;
const DzenPanel *const *const dzenPanelSetS;
const ActionChain *const startUpHookS;
const ActionChain *const endUpHookS;


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

static void set_cursors_and_atoms() {
  // Cursors
  *(Cursor *)&cursors[ NeuroSystemCursorNormal ] = XCreateFontCursor(display_, XC_left_ptr);
  *(Cursor *)&cursors[ NeuroSystemCursorResize ] = XCreateFontCursor(display_, XC_bottom_right_corner);
  *(Cursor *)&cursors[ NeuroSystemCursorMove ]   = XCreateFontCursor(display_, XC_fleur);

  // WM Atoms
  *(Atom *)&wmatoms[ NeuroSystemWmAtomProtocols ]     = XInternAtom(display_, "WM_PROTOCOLS", False);
  *(Atom *)&wmatoms[ NeuroSystemWmAtomDeleteWindow ]  = XInternAtom(display_, "WM_DELETE_WINDOW", False);

  // Net Atoms
  *(Atom *)&netatoms[ NeuroSystemNetAtomSupported ]   = XInternAtom(display_, "_NET_SUPPORTED", False);
  *(Atom *)&netatoms[ NeuroSystemNetAtomState ]       = XInternAtom(display_, "_NET_WM_STATE", False);
  *(Atom *)&netatoms[ NeuroSystemNetAtomName ]        = XInternAtom(display_, "_NET_WM_NAME", False);
  *(Atom *)&netatoms[ NeuroSystemNetAtomActive ]      = XInternAtom(display_, "_NET_ACTIVE_WINDOW", False);
  *(Atom *)&netatoms[ NeuroSystemNetAtomFullscreen ]  = XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", False);
  *(Atom *)&netatoms[ NeuroSystemNetAtomStrut ]       = XInternAtom(display_, "_NET_WM_STRUT", False);
  *(Atom *)&netatoms[ NeuroSystemNetAtomCloseWindow ] = XInternAtom(display_, "_NET_CLOSE_WINDOW", False);

  // EWMH support per view
  XChangeProperty(display_, root_, netatoms[ NeuroSystemNetAtomSupported ], XA_ATOM, 32, PropModeReplace,
      (unsigned char *)netatoms, NeuroSystemNetAtomLast);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

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
  return cursors[ c ];
}

Atom NeuroSystemGetWmAtom(NeuroSystemWmAtom a) {
  return wmatoms[ a ];
}

Atom NeuroSystemGetNetAtom(NeuroSystemNetAtom a) {
  return netatoms[ a ];
}

void NeuroSystemSetConfiguration(const Configuration *c) {
  assert(c);
  *(const char **)&normBorderColor = c->normBorderColor;
  *(const char **)&currBorderColor = c->currBorderColor;
  *(const char **)&prevBorderColor = c->prevBorderColor;
  *(const char **)&freeBorderColor = c->freeBorderColor;
  *(const char **)&urgtBorderColor = c->urgtBorderColor;
  *(int *)&borderWidthS = c->borderWidth;
  *(int *)&borderGapS = c->borderGap;
  *(const Key *const **)&keyBindingsS = c->keys;
  *(const Button *const **)&buttonBindingsS = c->buttons;
  *(const Rule *const **)&ruleSetS = c->ruleSet;
  *(const Workspace *const **)&workspaceSetS = c->workspaceSet;
  *(const DzenPanel *const **)&dzenPanelSetS = c->dzenPanelSet;
  *(const ActionChain **)&startUpHookS = c->startUpHook;
  *(const ActionChain **)&endUpHookS = c->endUpHook;
}

Bool NeuroSystemInit() {
  // WM global variables
  *(Display **)&display_ = XOpenDisplay(NULL);
  if (!display_)
    return False;
  *(int *)&screen_ = DefaultScreen(display_);
  *(Window *)&root_ = RootWindow(display_, screen_);
  *(int *)&x_res_ = XDisplayWidth(display_, screen_);
  *(int *)&y_res_ = XDisplayHeight(display_, screen_);
  *(Rectangle *)&screen_region_ = (Rectangle){
      .x = NeuroSystemGetXPos, .y = NeuroSystemGetYPos, .w = x_res_, .h = y_res_ };
  *(Color *)&normBorderColorS = NeuroSystemGetColor(normBorderColor);
  *(Color *)&currBorderColorS = NeuroSystemGetColor(currBorderColor);
  *(Color *)&prevBorderColorS = NeuroSystemGetColor(prevBorderColor);
  *(Color *)&freeBorderColorS = NeuroSystemGetColor(freeBorderColor);
  *(Color *)&urgtBorderColorS = NeuroSystemGetColor(urgtBorderColor);

  // Set cursors and atoms
  set_cursors_and_atoms();

  // Check if other window manager is already running
  XSetErrorHandler(xerror_handler_start);

  // Setup root window mask
  XSetWindowAttributes wa;
  wa.cursor = cursors[ NeuroSystemCursorNormal ];
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
  XFreeCursor(display_, cursors[ NeuroSystemCursorNormal ]);
  XFreeCursor(display_, cursors[ NeuroSystemCursorResize ]);
  XFreeCursor(display_, cursors[ NeuroSystemCursorMove ]);
  XCloseDisplay(display_);
}

Color NeuroSystemGetColor(const char* color) {
  assert(color);
  XColor c;
  Colormap map = DefaultColormap(display_, screen_);
  if (!XAllocNamedColor(display_, map, color, &c, &c))
    NeuroSystemError("NeuroSystemGetColor - Could not allocate color");
  return c.pixel;
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
  for (i = 0; keyBindingsS[ i ]; ++i) {
    k = keyBindingsS[ i ];
    code = XKeysymToKeycode(display_, k->key);
    if (code)
      XGrabKey(display_, code, k->mod, w, True, GrabModeAsync, GrabModeAsync);
  }
}

void NeuroSystemUngrabKeys(Window w) {
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; keyBindingsS[ i ]; ++i) {
    k = keyBindingsS[ i ];
    code = XKeysymToKeycode(display_, k->key);
    if (code)
      XUngrabKey(display_, code, k->mod, w);
  }
}

void NeuroSystemGrabButtons(Window w) {
  XUngrabButton(display_, AnyButton, AnyModifier, w);
  const Button *b;
  int i;
  for (i=0; buttonBindingsS[ i ]; ++i) {
    b = buttonBindingsS[ i ];
    XGrabButton(display_, b->button, b->mod, w, False, ButtonPressMask|ButtonReleaseMask,
        GrabModeAsync, GrabModeSync, None, None);
  }
}

void NeuroSystemUngrabButtons(Window w) {
  const Button *b;
  int i;
  for (i=0; buttonBindingsS[ i ]; ++i) {
    b = buttonBindingsS[ i ];
    if (b->ungrabOnFocus)
      XUngrabButton(display_, b->button, b->mod, w);
  }
}

