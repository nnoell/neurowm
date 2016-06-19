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
static NeuroRectangle screen_region_;
static NeuroRectangle hidden_region_;
static const int hidden_gaps_[ 4 ] = { 0 };
static Cursor cursors_[ NEURO_SYSTEM_CURSOR_END ];
static Atom wm_atoms_[ NEURO_SYSTEM_WMATOM_END ];
static Atom net_atoms_[ NEURO_SYSTEM_NETATOM_END ];
static NeuroColor colors_[ NEURO_SYSTEM_COLOR_END ];

// Version
static const char *const version_ = PKG_NAME " " PKG_VERSION;

// Recompile command
static char recompile_cmd_output_[ NEURO_NAME_SIZE_MAX ];
static char recompile_cmd_source_[ NEURO_NAME_SIZE_MAX ];
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
  NeuroSystemError(__func__, "xerror_handler_start - Another window manager is already running");
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

static bool set_colors_cursors_atoms(void) {
  if (!NeuroConfigGet()->normal_border_color || !NeuroConfigGet()->current_border_color ||
      !NeuroConfigGet()->old_border_color || !NeuroConfigGet()->free_border_color ||
      !NeuroConfigGet()->urgent_border_color)
    return false;

  // Colors
  colors_[ NEURO_SYSTEM_COLOR_NORMAL ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->normal_border_color);
  colors_[ NEURO_SYSTEM_COLOR_CURRENT ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->current_border_color);
  colors_[ NEURO_SYSTEM_COLOR_OLD ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->old_border_color);
  colors_[ NEURO_SYSTEM_COLOR_FREE ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->free_border_color);
  colors_[ NEURO_SYSTEM_COLOR_URGENT ] = NeuroSystemGetColorFromHex(NeuroConfigGet()->urgent_border_color);

  // Cursors
  cursors_[ NEURO_SYSTEM_CURSOR_NORMAL ] = XCreateFontCursor(display_, XC_left_ptr);
  cursors_[ NEURO_SYSTEM_CURSOR_RESIZE ] = XCreateFontCursor(display_, XC_bottom_right_corner);
  cursors_[ NEURO_SYSTEM_CURSOR_MOVE ] = XCreateFontCursor(display_, XC_fleur);

  // WM Atoms
  wm_atoms_[ NEURO_SYSTEM_WMATOM_PROTOCOLS ] = XInternAtom(display_, "WM_PROTOCOLS", false);
  wm_atoms_[ NEURO_SYSTEM_WMATOM_DELETEWINDOW ] = XInternAtom(display_, "WM_DELETE_WINDOW", false);

  // Net Atoms
  net_atoms_[ NEURO_SYSTEM_NETATOM_SUPPORTED ] = XInternAtom(display_, "_NET_SUPPORTED", false);
  net_atoms_[ NEURO_SYSTEM_NETATOM_STATE ] = XInternAtom(display_, "_NET_WM_STATE", false);
  net_atoms_[ NEURO_SYSTEM_NETATOM_NAME ] = XInternAtom(display_, "_NET_WM_NAME", false);
  net_atoms_[ NEURO_SYSTEM_NETATOM_ACTIVE ] = XInternAtom(display_, "_NET_ACTIVE_WINDOW", false);
  net_atoms_[ NEURO_SYSTEM_NETATOM_FULLSCREEN ] = XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", false);
  net_atoms_[ NEURO_SYSTEM_NETATOM_STRUT ] = XInternAtom(display_, "_NET_WM_STRUT", false);
  net_atoms_[ NEURO_SYSTEM_NETATOM_CLOSEWINDOW ] = XInternAtom(display_, "_NET_CLOSE_WINDOW", false);

  // EWMH support per view
  XChangeProperty(display_, root_, net_atoms_[ NEURO_SYSTEM_NETATOM_SUPPORTED ], XA_ATOM, 32, PropModeReplace,
      (unsigned char *)net_atoms_, NEURO_SYSTEM_NETATOM_END);

  return true;
}

// Note: This helper function will always exit
static void run_command(const char *const *cmd) {
  assert(cmd);
  assert(*cmd);

  // Create a new command in order to avoid const correctness issues in execvp 2nd parameter
  const NeuroIndex size = NeuroTypeArrayLength((const void *const *)cmd) + 1;  // We need an extra slot for NULL
  char **command = (char **)calloc(size, sizeof(void *));
  if (!command)
    NeuroSystemError(__func__, "Could not calloc");

  // Copy the data from cmd
  command[ size - 1 ] = NULL;
  NeuroIndex i = 0U;
  for ( ; cmd[ i ]; ++i) {
    const char *const src = cmd[ i ];
    const NeuroIndex n = strlen(src);
    command[ i ] = (char *)calloc(n, sizeof(char));
    if (!command[ i ])
      NeuroSystemError(__func__, "Could not calloc");
    strncpy(command[ i ], src, n);
  }

  // execute the command
  execvp(command[ 0 ], command);

  // If execvp failed, release command and terminate child process
  for (i = 0U; command[ i ]; ++i) {
    free(command[ i ]);
    command[ i ] = NULL;
  }
  free(command);
  command = NULL;

  NeuroSystemError(__func__, "Could not execvp");
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// X functions
bool NeuroSystemInit(void) {
  // WM global variables
  display_ = XOpenDisplay(NULL);
  if (!display_)
    return false;
  screen_ = DefaultScreen(display_);
  root_ = RootWindow(display_, screen_);

  // Get the regions
  const int width = XDisplayWidth(display_, screen_);
  const int height = XDisplayHeight(display_, screen_);
  screen_region_ = (NeuroRectangle){ (NeuroPoint){ 0, 0 }, width, height };
  hidden_region_ = (NeuroRectangle){ (NeuroPoint){ width, height }, 1920, 1080 };

  // Set colors, cursors and atoms
  if (!set_colors_cursors_atoms())
    return false;

  // Check if another window manager is already running
  XSetErrorHandler(xerror_handler_start);

  // Setup root window mask
  XSetWindowAttributes wa;
  wa.cursor = NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_NORMAL);
  wa.event_mask = NEURO_SYSTEM_ROOT_MASK;
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

void NeuroSystemStop(void) {
  XFreeCursor(display_, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_NORMAL));
  XFreeCursor(display_, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_RESIZE));
  XFreeCursor(display_, NeuroSystemGetCursor(NEURO_SYSTEM_CURSOR_MOVE));
  XCloseDisplay(display_);
}

// Basic functions
Display *NeuroSystemGetDisplay(void) {
  return display_;
}

Window NeuroSystemGetRoot(void) {
  return root_;
}

int NeuroSystemGetScreen(void) {
  return screen_;
}

const NeuroRectangle *NeuroSystemGetScreenRegion(void) {
  return &screen_region_;
}

const NeuroRectangle *NeuroSystemGetHiddenRegion(void) {
  return &hidden_region_;
}

const int *NeuroSystemGetHiddenGaps(void) {
  return hidden_gaps_;
}

void NeuroSystemGetPointerWindowLocation(NeuroPoint *p, Window *w) {
  Window root_win = 0UL, child_win = 0UL;
  int px = 0, py = 0, xc = 0, yc = 0;
  unsigned int state = 0;
  if (!XQueryPointer(display_, root_, &root_win, w ? w : &child_win, p ? &p->x : &px, p ? &p->y : &py, &xc, &yc,
      &state) ? p : NULL)
    NeuroSystemError(__func__, "Could not query pointer");
}

Cursor NeuroSystemGetCursor(NeuroSystemCursor c) {
  return cursors_[ c ];
}

Atom NeuroSystemGetWmAtom(NeuroSystemWmatom a) {
  return wm_atoms_[ a ];
}

Atom NeuroSystemGetNetAtom(NeuroSystemNetatom a) {
  return net_atoms_[ a ];
}

NeuroColor NeuroSystemGetColor(NeuroSystemColor c) {
  return colors_[ c ];
}

NeuroColor NeuroSystemGetColorFromHex(const char* color) {
  assert(color);
  XColor c;
  if (!XAllocNamedColor(display_, DefaultColormap(display_, screen_), color, &c, &c))
    NeuroSystemError(__func__, "Could not allocate color");
  return (NeuroColor)c.pixel;
}

// System functions
const char *NeuroSystemGetVersion(void) {
  return version_;
}

const char * const *NeuroSystemGetRecompileCommand(const char **output, const char **source) {
  snprintf((char *)recompile_cmd_output_, NEURO_NAME_SIZE_MAX, "%s/." PKG_NAME "/" PKG_MYNAME, getenv("HOME"));
  snprintf((char *)recompile_cmd_source_, NEURO_NAME_SIZE_MAX, "%s/." PKG_NAME "/" PKG_NAME ".c", getenv("HOME"));
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
  XChangeProperty(display_, root_, netwmname, utf8_str, 8, PropModeReplace, (const unsigned char *)name, strlen(name));
}

void NeuroSystemChangeProcName(const char *name) {
  assert(name);
  prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);
}

pid_t NeuroSystemGetWmPid(void) {
  char pidstr[ NEURO_NAME_SIZE_MAX ];
  FILE *const cmd = popen("/usr/bin/pidof -s " PKG_MYNAME, "r");
  fgets(pidstr, NEURO_NAME_SIZE_MAX, cmd);
  return (pid_t)strtoul(pidstr, NULL, 10);
}

bool NeuroSystemSpawn(const char *const *cmd, pid_t *p) {
  if (!cmd && !*cmd)
    return false;

  // Create a new process for the new command
  const pid_t pid = fork();
  if (pid == -1)
    return false;

  // Run the command within the child process
  if (pid == 0) {
    if (display_)
      close(ConnectionNumber(display_));
    setsid();

    run_command(cmd);
  }

  // Store the pid if needed
  if (p)
    *p = pid;

  return true;
}

int NeuroSystemSpawnPipe(const char *const *cmd, pid_t *p) {
  if (!cmd && !*cmd)
    return -1;

  // Create a fd array for the pipe
  int filedes[ 2 ];
  if (pipe(filedes))
    return -1;

  // Run command within the child process
  const pid_t pid = fork();
  if (pid < 0)
    return -1;

  // Run the command within the child process
  if (pid == 0) {
    if (display_)
      close(ConnectionNumber(display_));
    setsid();

    // redirect the output of the command
    dup2(filedes[ 0 ], STDIN_FILENO);
    close(filedes[ 0 ]);

    run_command(cmd);
  }

  // Store the pid if needed
  if (p)
    *p = pid;

  return filedes[ 1 ];
}

void NeuroSystemError(const char *function_name, const char *msg) {
  fprintf(stderr, "%s|%s:", function_name, msg);
  perror("");
  exit(EXIT_FAILURE);
}

// Binding functions
void NeuroSystemGrabKeys(Window w, const NeuroKey *const *key_list) {
  if (!key_list)
    return;
  XUngrabKey(display_, AnyKey, AnyModifier, w);
  for (NeuroIndex i = 0U; key_list[ i ]; ++i) {
    const NeuroKey *const k = key_list[ i ];
    const KeyCode code = XKeysymToKeycode(display_, k->key);
    if (code)
      XGrabKey(display_, code, k->mod, w, true, GrabModeAsync, GrabModeAsync);
  }
}

void NeuroSystemUngrabKeys(Window w, const NeuroKey *const *key_list) {
  if (!key_list)
    return;
  for (NeuroIndex i = 0U; key_list[ i ]; ++i) {
    const NeuroKey *const k = key_list[ i ];
    const KeyCode code = XKeysymToKeycode(display_, k->key);
    if (code)
      XUngrabKey(display_, code, k->mod, w);
  }
}

void NeuroSystemGrabButtons(Window w, const NeuroButton *const *button_list) {
  if (!button_list)
    return;
  XUngrabButton(display_, AnyButton, AnyModifier, w);
  for (NeuroIndex i = 0U; button_list[ i ]; ++i) {
    const NeuroButton *const b = button_list[ i ];
    XGrabButton(display_, b->button, b->mod, w, false, ButtonPressMask|ButtonReleaseMask, GrabModeAsync, GrabModeSync,
        None, None);
  }
}

void NeuroSystemUngrabButtons(Window w, const NeuroButton *const *button_list) {
  if (!button_list)
    return;
  for (NeuroIndex i = 0U; button_list[ i ]; ++i) {
    const NeuroButton *b = button_list[ i ];
    if (b->ungrab_on_focus)
      XUngrabButton(display_, b->button, b->mod, w);
  }
}

