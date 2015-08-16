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


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void changeWMNameN(ActionAr arg) {
  assert(arg.com);
  changeWMNameS(arg.string_);
}

void spawnN(ActionAr arg) {
  assert(arg.com);
  spawnS(arg.command_, NULL);
}

void quitN(ActionAr arg) {
  (void)arg;
  stopWhile = True;
}

void reloadN(ActionAr arg) {
  (void)arg;
  signalHandler(SIGUSR1);
}

void killClientN(ActionAr arg) {
  assert(arg.afn.scf);
  killClientW(getCurrClientCurrStackSS(), arg.argfn_.selectCliFn);
}

void moveFocusClientN(ActionAr arg) {
  assert(arg.afn.scf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  moveFocusClientW(getCurrClientStackSS(ws), arg.argfn_.selectCliFn);
  addEnterNotifyMaskW(ws);
}

void swapClientN(ActionAr arg) {
  assert(arg.afn.scf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  swapClientW(getCurrClientStackSS(ws), arg.argfn_.selectCliFn);
  addEnterNotifyMaskW(ws);
}

void changeLayoutN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, arg.int_);
  addEnterNotifyMaskW(ws);
}

void resetLayoutN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetLayoutL(ws);
  addEnterNotifyMaskW(ws);
}

void increaseMasterN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, arg.int_);
  addEnterNotifyMaskW(ws);
}

void resizeMasterN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, arg.float_);
  addEnterNotifyMaskW(ws);
}

void changeToWorkspaceN(ActionAr arg) {
  rmvEnterNotifyMaskW(arg.int_);
  changeToWorkspaceW(arg.int_);
  addEnterNotifyMaskW(arg.int_);
}

void changeToPrevWorkspaceN(ActionAr arg) {
  (void)arg;
  const int ws = getPrevStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToNextWorkspaceN(ActionAr arg) {
  (void)arg;
  const int ws = getNextStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToLastWorkspaceN(ActionAr arg) {
  (void)arg;
  const int ws = getLastStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void toggleFreeCurrClientN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFreeClientW(getCurrClientStackSS(ws), selfC, (const void *)&arg.argfn_);
  addEnterNotifyMaskW(ws);
}

void tileClientN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  tileC(getCurrClientStackSS(ws), NULL);
  addEnterNotifyMaskW(ws);
}

void freeClientN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  freeClientW(getCurrClientStackSS(ws), selfC, (const void *)&arg.argfn_);
  addEnterNotifyMaskW(ws);
}

void normalClientN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  normalC(getCurrClientStackSS(ws), NULL);
  addEnterNotifyMaskW(ws);
}

void fullScreenClientN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  fullScreenC(getCurrClientStackSS(ws), NULL);
  addEnterNotifyMaskW(ws);
}

void toggleFullScreenClientN(ActionAr arg) {
  assert(arg.afn.scf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFullScreenClientW(getCurrClientStackSS(ws), arg.argfn_.selectCliFn);
  addEnterNotifyMaskW(ws);
}

void toggleFreePtrClientN(ActionAr arg) {
  moveFocusClientW(getCurrClientCurrStackSS(), pointerC);
  toggleFreeC(getCurrClientCurrStackSS(), (const void *)&arg.argfn_);
}

void toggleFullScreenPtrClientN(ActionAr arg) {
  (void)arg;
  moveFocusClientW(getCurrClientCurrStackSS(), pointerC);
  toggleFullScreenC(getCurrClientCurrStackSS(), NULL);
}

void freeMovePtrClientN(ActionAr arg) {
  (void)arg;
  freeMovePtrC();
}

void freeResizePtrClientN(ActionAr arg) {
  (void)arg;
  freeResizePtrC();
}

void movePtrClientN(ActionAr arg) {
  (void)arg;
  movePtrC();
}

void resizePtrClientN(ActionAr arg) {
  (void)arg;
  resizePtrC();
}

void toggleLayoutModN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, arg.uint_);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, arg.int_);
  addEnterNotifyMaskW(ws);
}

void moveClientToWorkspaceN(ActionAr arg) {
  moveClientToWorkspaceW(getCurrClientCurrStackSS(), arg.int_ % getSizeSS());
}

void moveClientToWorkspaceAndFollowN(ActionAr arg) {
  moveClientToWorkspaceAndFollowW(getCurrClientCurrStackSS(), arg.int_ % getSizeSS());
}

void toggleNSPN(ActionAr arg) {
  assert(arg.com);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  CliPtr c = findNSPClientSS();
  if (c && CLIVAL(c).ws == ws) {
    moveClientToWorkspaceW(c, getNSPStackSS());
  } else if (c) {
    moveClientToWorkspaceW(c, ws);
  } else {
    if (!getSizeNSPSS())
      spawnS(arg.command_, NULL);
    else
      moveClientToWorkspaceW(getCurrClientNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}

void minimizeClientN(ActionAr arg) {
  assert(arg.afn.scf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  minimizeClientW(getCurrClientStackSS(ws), arg.argfn_.selectCliFn);
  addEnterNotifyMaskW(ws);
}

void restoreLastMinimizedClientN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  restoreLastMinimizedW(ws);
  addEnterNotifyMaskW(ws);
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

