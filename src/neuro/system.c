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

// Global configuration
static const char *normBorderColor;
static const char *currBorderColor;
static const char *prevBorderColor;
static const char *freeBorderColor;
static const char *urgtBorderColor;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// WM const global variables
Display *const display;
const int screen;
const Window root;
const uint32_t xRes;
const uint32_t yRes;
const Rectangle screenRegion;

// Global configuration
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

// Cursors and Atoms
const Cursor cursors[ CurLast ];
const Atom wmatoms[ WM_COUNT ];
const Atom netatoms[ NET_COUNT ];


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Starting error handler, used to check if another window manager is already running
static int XErrorStartWindowManagerHandler(Display *d, XErrorEvent *ee) {
  (void)d;
  (void)ee;
  exitErrorS("XErrorStartHandlerS - another window manager is already running");
  return -1;
}

// Default error handler, which might call exit
static int XErrorWindowManagerHandler(Display *d, XErrorEvent *ee) {
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
  printf("error: request code=%d, error code=%d\n", ee->request_code, ee->error_code);
  return -1;
}

static void setCursorsAndAtomsS() {
  // Cursors
  *(Cursor *)&cursors[ CurNormal ] = XCreateFontCursor(display, XC_left_ptr);
  *(Cursor *)&cursors[ CurResize ] = XCreateFontCursor(display, XC_bottom_right_corner);
  *(Cursor *)&cursors[ CurMove ]   = XCreateFontCursor(display, XC_fleur);

  // Atoms
  *(Atom *)&wmatoms[ WM_PROTOCOLS ]      = XInternAtom(display, "WM_PROTOCOLS", False);
  *(Atom *)&wmatoms[ WM_DELETE_WINDOW ]  = XInternAtom(display, "WM_DELETE_WINDOW", False);
  *(Atom *)&netatoms[ NET_SUPPORTED ]    = XInternAtom(display, "_NET_SUPPORTED", False);
  *(Atom *)&netatoms[ NET_WM_STATE ]     = XInternAtom(display, "_NET_WM_STATE", False);
  *(Atom *)&netatoms[ NET_WM_NAME ]      = XInternAtom(display, "_NET_WM_NAME", False);
  *(Atom *)&netatoms[ NET_ACTIVE ]       = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
  *(Atom *)&netatoms[ NET_FULLSCREEN ]   = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
  *(Atom *)&netatoms[ NET_STRUT ]        = XInternAtom(display, "_NET_WM_STRUT", False);
  *(Atom *)&netatoms[ NET_CLOSE_WINDOW ] = XInternAtom(display, "_NET_CLOSE_WINDOW", False);

  // EWMH support per view
  XChangeProperty(display, root, netatoms[ NET_SUPPORTED ], XA_ATOM, 32, PropModeReplace, (unsigned char *)netatoms, \
      NET_COUNT);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Basic functions
void setConfigS(const Configuration *c) {
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

Bool initS() {
  // WM global variables
  *(Display **)&display = XOpenDisplay(NULL);
  if (!display)
    return False;
  *(int *)&screen = DefaultScreen(display);
  *(Window *)&root = RootWindow(display, screen);
  *(int *)&xRes = XDisplayWidth(display, screen);
  *(int *)&yRes = XDisplayHeight(display, screen);
  *(Rectangle *)&screenRegion = (Rectangle){ .x = xPos, .y = yPos, .w = xRes, .h = yRes };
  *(Color *)&normBorderColorS = getColorS(normBorderColor);
  *(Color *)&currBorderColorS = getColorS(currBorderColor);
  *(Color *)&prevBorderColorS = getColorS(prevBorderColor);
  *(Color *)&freeBorderColorS = getColorS(freeBorderColor);
  *(Color *)&urgtBorderColorS = getColorS(urgtBorderColor);

  // Set cursors and atoms
  setCursorsAndAtomsS();

  // Check if other window manager is already running
  XSetErrorHandler(XErrorStartWindowManagerHandler);

  // Setup root window mask
  XSetWindowAttributes wa;
  wa.cursor = cursors[ CurNormal ];
  wa.event_mask = ROOT_MASK;
  XChangeWindowAttributes(display, root, CWEventMask|CWCursor, &wa);
  XSelectInput(display, root, wa.event_mask);
  XSync(display, False);

  // Set custom X error handler
  XSetErrorHandler(XErrorWindowManagerHandler);
  XSync(display, False);

  // Grab key bindings
  grabKeysS(root);

  return True;
}

void stopS() {
  XFreeCursor(display, cursors[ CurNormal ]);
  XFreeCursor(display, cursors[ CurResize ]);
  XFreeCursor(display, cursors[ CurMove ]);
  XCloseDisplay(display);
}

// Binding functions
void grabKeysS(Window w) {
  XUngrabKey(display, AnyKey, AnyModifier, w);
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; keyBindingsS[ i ]; ++i) {
    k = keyBindingsS[ i ];
    code = XKeysymToKeycode(display, k->key);
    if (code)
      XGrabKey(display, code, k->mod, w, True, GrabModeAsync, GrabModeAsync);
  }
}

void ungrabKeysS(Window w) {
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; keyBindingsS[ i ]; ++i) {
    k = keyBindingsS[ i ];
    code = XKeysymToKeycode(display, k->key);
    if (code)
      XUngrabKey(display, code, k->mod, w);
  }
}

void grabButtonsS(Window w) {
  XUngrabButton(display, AnyButton, AnyModifier, w);
  const Button *b;
  int i;
  for (i=0; buttonBindingsS[ i ]; ++i) {
    b = buttonBindingsS[ i ];
    XGrabButton(display, b->button, b->mod, w, False, ButtonPressMask|ButtonReleaseMask,
        GrabModeAsync, GrabModeSync, None, None);
  }
}

void ungrabButtonsS(Window w) {
  const Button *b;
  int i;
  for (i=0; buttonBindingsS[ i ]; ++i) {
    b = buttonBindingsS[ i ];
    if (b->ungrabOnFocus)
      XUngrabButton(display, b->button, b->mod, w);
  }
}

// Other useful X functions
Color getColorS(const char* color) {
  assert(color);
  XColor c;
  Colormap map = DefaultColormap(display, screen);
  if (!XAllocNamedColor(display, map, color, &c, &c))
    exitErrorS("getColorS - could not allocate color");
  return c.pixel;
}

void changeWMNameS(const char *name) {
  assert(name);
  Atom netwmcheck = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
  Atom netwmname = XInternAtom(display, "_NET_WM_NAME", False);
  Atom utf8_str = XInternAtom(display, "UTF8_STRING", False);
  XChangeProperty(display, root, netwmcheck, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&root, 1);
  XChangeProperty(display, root, netwmname, utf8_str, 8, PropModeReplace, (unsigned char *)name, strlen(name));
}

void changeProcNameS(const char *newname) {
  assert(newname);
  prctl(PR_SET_NAME, (unsigned long)newname, 0, 0, 0);
}

int spawnS(const char *const *cmd, pid_t *p) {
  assert(cmd);
  pid_t pid = fork();
  if (pid == -1)
    return -1;
  if (!pid) {  // Child
    if (display)
      close(ConnectionNumber(display));
    setsid();
    execvp(cmd[ 0 ], (char *const *)cmd);
    exitErrorS("spawnS - Could not execvp");
  }
  if (p)
    *p = pid;
  return 0;
}

int spawnPipeS(const char *const *cmd, pid_t *p) {
  assert(cmd);
  int filedes[ 2 ];
  if (pipe(filedes))
    return -1;
  pid_t pid = fork();
  if (pid < 0)
    return -1;
  if (pid == 0) {  // Child
    if (display)
      close(ConnectionNumber(display));
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

void exitErrorS(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
