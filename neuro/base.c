//----------------------------------------------------------------------------------------------------------------------
// Module      :  base
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "base.h"


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// WM const global variables
Display *display;
const int screen;
const Window root;
const uint32_t xRes;
const uint32_t yRes;
const Area screenArea;

// Global configuration
const Color normBorderColorB;
const Color currBorderColorB;
const Color prevBorderColorB;
const Color freeBorderColorB;
const Color urgtBorderColorB;
const int borderWidthB;
const int borderGapB;
const Key **keyBindingsB;
const Button **buttonBindingsB;
const Rule **ruleSetB;
const Workspace **workspaceSetB;
const DzenPanel **dzenPanelSetB;
const WMFunc **startUpHookB;
const WMFunc **endUpHookB;

// Cursors and Atoms
const Cursor cursors[ CurLast ];
const Atom wmatoms[ WM_COUNT ];
const Atom netatoms[ NET_COUNT ];


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Starting error handler, used to check if another window manager is already running
static int XErrorStartHandlerB(Display *d, XErrorEvent *ee) {
  (void)d;
  (void)ee;
  exitErrorG("XErrorStartHandlerB - another window manager is already running");
  return -1;
}

// Default error handler, which might call exit
static int XErrorHandlerB(Display *d, XErrorEvent *ee) {
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
      || (ee->request_code == X_CopyArea          && ee->error_code == BadDrawable))
    return 0;
  printf("error: request code=%d, error code=%d\n", ee->request_code, ee->error_code);
  return -1;
}

static void setCursorsAndAtomsB() {
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
Bool initB(const WMConfig *c) {
  // WM global variables
  display = XOpenDisplay(NULL);
  if (!display)
    return 1;
  *(int *)&screen = DefaultScreen(display);
  *(Window *)&root = RootWindow(display, screen);
  *(int *)&xRes = XDisplayWidth(display, screen);
  *(int *)&yRes = XDisplayHeight(display, screen);
  *(Area *)&screenArea = (Area){ .x = xPos, .y = yPos, .w = xRes, .h = yRes };

  // Global configuration
  *(Color *)&normBorderColorB = getColorB(c->normBorderColor);
  *(Color *)&currBorderColorB = getColorB(c->currBorderColor);
  *(Color *)&prevBorderColorB = getColorB(c->prevBorderColor);
  *(Color *)&freeBorderColorB = getColorB(c->freeBorderColor);
  *(Color *)&urgtBorderColorB = getColorB(c->urgtBorderColor);
  *(int *)&borderWidthB = c->borderWidth;
  *(int *)&borderGapB = c->borderGap;
  keyBindingsB = c->keys;
  buttonBindingsB = c->buttons;
  ruleSetB = c->ruleSet;
  workspaceSetB = c->workspaceSet;
  dzenPanelSetB = c->dzenPanelSet;
  startUpHookB = c->startUpHook;
  endUpHookB = c->endUpHook;

  // Set cursors and atoms
  setCursorsAndAtomsB();

  // Check if other window manager is already running
  XSetErrorHandler(XErrorStartHandlerB);

  // Setup root window mask
  XSetWindowAttributes wa;
  wa.cursor = cursors[ CurNormal ];
  wa.event_mask = ROOT_MASK;
  XChangeWindowAttributes(display, root, CWEventMask|CWCursor, &wa);
  XSelectInput(display, root, wa.event_mask);
  XSync(display, False);

  // Set custom X error handler
  XSetErrorHandler(XErrorHandlerB);
  XSync(display, False);

  // Grab key bindings
  grabKeysB(root);

  return True;
}

void endB() {
  XFreeCursor(display, cursors[ CurNormal ]);
  XFreeCursor(display, cursors[ CurResize ]);
  XFreeCursor(display, cursors[ CurMove ]);
  XCloseDisplay(display);
}

// Binding functions
void grabKeysB(Window w) {
  XUngrabKey(display, AnyKey, AnyModifier, w);
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; keyBindingsB[ i ]; ++i) {
    k = keyBindingsB[ i ];
    code = XKeysymToKeycode(display, k->keySym);
    if (code)
      XGrabKey(display, code, k->mod, w, True, GrabModeAsync, GrabModeAsync);
  }
}

void ungrabKeysB(Window w) {
  KeyCode code;
  const Key *k;
  int i;
  for (i = 0; keyBindingsB[ i ]; ++i) {
    k = keyBindingsB[ i ];
    code = XKeysymToKeycode(display, k->keySym);
    if (code)
      XUngrabKey(display, code, k->mod, w);
  }
}

void grabButtonsB(Window w) {
  XUngrabButton(display, AnyButton, AnyModifier, w);
  const Button *b;
  int i;
  for (i=0; buttonBindingsB[ i ]; ++i) {
    b = buttonBindingsB[ i ];
    XGrabButton(display, b->button, b->mod, w, False, ButtonPressMask|ButtonReleaseMask,
        GrabModeAsync, GrabModeSync, None, None);
  }
}

void ungrabButtonsB(Window w) {
  const Button *b;
  int i;
  for (i=0; buttonBindingsB[ i ]; ++i) {
    b = buttonBindingsB[ i ];
    if (b->ungrabOnFocus)
      XUngrabButton(display, b->button, b->mod, w);
  }
}

// Other useful X functions
Color getColorB(const char* color) {
  XColor c;
  Colormap map = DefaultColormap(display, screen);
  if (!XAllocNamedColor(display, map, color, &c, &c))
    exitErrorG("getColorB - could not allocate color");
  return c.pixel;
}

void changeWMNameB(Arg arg) {
  Atom netwmcheck = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
  Atom netwmname = XInternAtom(display, "_NET_WM_NAME", False);
  Atom utf8_str = XInternAtom(display, "UTF8_STRING", False);
  XChangeProperty(display, root, netwmcheck, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&root, 1);
  XChangeProperty(display, root, netwmname, utf8_str, 8, PropModeReplace, (unsigned char *)arg.str, strlen(arg.str));
}
