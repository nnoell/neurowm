//----------------------------------------------------------------------------------------------------------------------
// Module      :  neurowm
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "neurowm.h"
#include "system.h"
#include "stackset.h"
#include "event.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

static Bool stopWhile = False;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int recompileNeurowm(pid_t *pid) {
  char out[ NAME_MAX ] = "", src[ NAME_MAX ] = "";
  snprintf(out, NAME_MAX, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  snprintf(src, NAME_MAX, "%s/." WM_NAME "/" WM_NAME ".c", getenv("HOME"));
  char lib[ NAME_MAX ] = "/usr/lib/neuro/lib" WM_NAME ".a";
  const char *const cmd[] = { "/usr/bin/cc", "-O3", "-o", out, src, lib, "-lX11", "-pthread", NULL };
  return spawnS(cmd, pid);
}

static void endNeurowm() {
  // End endup hook
  int i;
  for (i = 0; endUpHookS[ i ]; ++i)
    endUpHookS[ i ]->func(endUpHookS[ i ]->arg);

  // End panels
  endDP();

  // End stackset
  endSS();

  // End base
  endS();
}

static void signalHandler(int signo) {
  if (signo == SIGUSR1) {
    endNeurowm();
    pid_t pid;
    if (recompileNeurowm(&pid) == -1)
      perror("signalHandler - Could not recompile neurowm");
    waitpid(pid, NULL, WUNTRACED);
    exit(EXIT_RELOAD);
  }
}

static void initNeurowm(const WMConfig *c) {
  if (!c)
    exitErrorS("initNeurowm - could not set configuration");

  // Set configuration
  setConfigS(c);

  // Init base (must free with endB at some point)
  if (!initS())
    exitErrorS("initNeurowm - could not init Base");

  // Init stackset (must free with endSS at some point)
  if (!initSS())
    exitErrorS("initNeurowm - could not init StackSet");

  // Init panels (must free with endDP at some point)
  if (!initDP())
    exitErrorS("initNeurowm - could not init Panels");

  // Init startup hook
  int i;
  for (i = 0; startUpHookS[ i ]; ++i)
    startUpHookS[ i ]->func(startUpHookS[ i ]->arg);

  // Catch asynchronously SIGUSR1
  // if (SIG_ERR == signal(SIGUSR1, signalHandler))
  //   exitErrorG("initNeurowm - could not set SIGHUP handler");

  // Load existing windows if Xsesion was not closed
  loadWindowsE();
}

static void processCliActionN(const GenericCliActionFn gcaf, CliPtr c, const SelectCliFn scf, const void *data) {
  assert(gcaf);
  assert(scf);
  if (!c)
    return;
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  gcaf(c, scf, data);
  addEnterNotifyMaskW(CLIVAL(c).ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void changeWMNameN(ActionAr string_arg) {
  assert(string_arg.string_);
  changeWMNameS(string_arg.string_);
}

void spawnN(ActionAr command_arg) {
  assert(command_arg.command_);
  spawnS(command_arg.command_, NULL);
}

void quitN(ActionAr no_arg) {
  (void)no_arg;
  stopWhile = True;
}

void reloadN(ActionAr no_arg) {
  (void)no_arg;
  signalHandler(SIGUSR1);
}


// Layout
void changeLayoutN(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void resetLayoutN(ActionAr no_arg) {
  (void)no_arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetLayoutL(ws);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutModN(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, int_arg.uint_);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutN(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void increaseMasterLayoutN(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void resizeMasterLayoutN(ActionAr float_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, float_arg.float_);
  addEnterNotifyMaskW(ws);
}


// Workspace
void changeToWorkspaceN(ActionAr int_arg) {
  rmvEnterNotifyMaskW(int_arg.int_);
  changeToWorkspaceW(int_arg.int_);
  addEnterNotifyMaskW(int_arg.int_);
}

void changeToPrevWorkspaceN(ActionAr no_arg) {
  (void)no_arg;
  const int ws = getPrevStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToNextWorkspaceN(ActionAr no_arg) {
  (void)no_arg;
  const int ws = getNextStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToLastWorkspaceN(ActionAr no_arg) {
  (void)no_arg;
  const int ws = getLastStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void moveClientToWorkspaceN(ActionAr int_arg) {
  moveClientToWorkspaceW(getCurrClientCurrStackSS(), int_arg.int_ % getSizeSS());
}

void moveClientToWorkspaceAndFollowN(ActionAr int_arg) {
  moveClientToWorkspaceAndFollowW(getCurrClientCurrStackSS(), int_arg.int_ % getSizeSS());
}

void restoreLastMinimizedN(ActionAr no_arg) {
  (void)no_arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  restoreLastMinimizedW(ws);
  addEnterNotifyMaskW(ws);
}

void toggleNSPN(ActionAr command_arg) {
  assert(command_arg.command_);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  CliPtr c = findNSPClientSS();
  if (c && CLIVAL(c).ws == ws) {
    moveClientToWorkspaceW(c, getNSPStackSS());
  } else if (c) {
    moveClientToWorkspaceW(c, ws);
  } else {
    if (!getSizeNSPSS())
      spawnS(command_arg.command_, NULL);
    else
      moveClientToWorkspaceW(getCurrClientNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}


// Curr Client
void moveFocusCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(moveFocusClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void swapCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(swapClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void killCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(killClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void tileCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(tileClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void normalCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(normalClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void fullScreenCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(fullScreenClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void toggleFullScreenCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(toggleFullScreenClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void minimizeCurrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionN(minimizeClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freeCurrClientN(ActionAr freeLocFn_arg) {
  processCliActionN(freeClientW, getCurrClientCurrStackSS(), selfC, (const void *)&freeLocFn_arg.argfn_);
}

void toggleFreeCurrClientN(ActionAr freeLocFn_arg) {
  processCliActionN(toggleFreeClientW, getCurrClientCurrStackSS(), selfC, (const void *)&freeLocFn_arg.argfn_);
}


// Ptr Client
void moveFocusPtrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionN(moveFocusClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freeMovePtrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionN(freeMoveClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freeResizePtrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionN(freeResizeClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void movePtrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionN(moveClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void resizePtrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionN(resizeClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void toggleFullScreenPtrClientN(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionN(toggleFullScreenClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freePtrClientN(ActionAr freeLocFn_arg) {
  int rx, ry;
  processCliActionN(freeClientW, getPtrClientW(&rx, &ry), selfC, (const void *)&freeLocFn_arg.argfn_);
}

void toggleFreePtrClientN(ActionAr freeLocFn_arg)  {
  int rx, ry;
  processCliActionN(toggleFreeClientW, getPtrClientW(&rx, &ry), selfC, (const void *)&freeLocFn_arg.argfn_);
}


//----------------------------------------------------------------------------------------------------------------------
// NEUROWM
//----------------------------------------------------------------------------------------------------------------------

int neurowm(const WMConfig *c) {
  // Init window manager
  initNeurowm(c);

  // Main loop
  XEvent ev;
  while (!stopWhile && !XNextEvent(display, &ev))
    if (eventsE[ ev.type ])
      eventsE[ ev.type ](&ev);

  // End window manager
  endNeurowm();

  return EXIT_SUCCESS;
}

