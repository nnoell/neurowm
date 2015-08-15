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
  changeWMNameS(arg.str);
}

void spawnN(ActionAr arg) {
  assert(arg.com);
  spawnS(arg.cmd, NULL);
}

void quitN(ActionAr arg) {
  (void)arg;
  stopWhile = True;
}

void reloadN(ActionAr arg) {
  (void)arg;
  signalHandler(SIGUSR1);
}

void killCliN(ActionAr arg) {
  (void)arg;
  killC(getCurrCliCurrStackSS());
}

void moveFocusN(ActionAr arg) {
  assert(arg.scf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  moveFocusW(getCurrCliStackSS(ws), arg.scf);
  addEnterNotifyMaskW(ws);
}

void swapCliN(ActionAr arg) {
  assert(arg.scf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  swapClientsW(getCurrCliStackSS(ws), arg.scf);
  addEnterNotifyMaskW(ws);
}

void changeLayoutN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, arg.i);
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
  increaseMasterL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void resizeMasterN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, arg.f);
  addEnterNotifyMaskW(ws);
}

void changeToWorkspaceN(ActionAr arg) {
  rmvEnterNotifyMaskW(arg.i);
  changeToWorkspaceW(arg.i);
  addEnterNotifyMaskW(arg.i);
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

void toggleFreeCliN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFreeC(getCurrCliStackSS(ws), arg.flf);
  addEnterNotifyMaskW(ws);
}

void tileCliN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  tileC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void freeCliN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  freeC(getCurrCliStackSS(ws), arg.flf);
  addEnterNotifyMaskW(ws);
}

void normalCliN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  normalC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void fullScreenCliN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  fullScreenC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void toggleFullScreenCliN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFullScreenC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void toggleFreePtrCliN(ActionAr arg) {
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFreeC(getCurrCliCurrStackSS(), arg.flf);
}

void toggleFullScreenPtrCliN(ActionAr arg) {
  (void)arg;
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFullScreenC(getCurrCliCurrStackSS());
}

void freeMovePointerCliN(ActionAr arg) {
  (void)arg;
  freeMovePointerC();
}

void freeResizePointerCliN(ActionAr arg) {
  (void)arg;
  freeResizePointerC();
}

void movePointerCliN(ActionAr arg) {
  (void)arg;
  movePointerC();
}

void resizePointerCliN(ActionAr arg) {
  (void)arg;
  resizePointerC();
}

void toggleLayoutModN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, arg.ui);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutN(ActionAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void moveCliToWorkspaceN(ActionAr arg) {
  moveCliToWorkspaceW(getCurrCliCurrStackSS(), arg.i % getSizeSS());
}

void moveCliToWorkspaceAndFollowN(ActionAr arg) {
  moveCliToWorkspaceAndFollowW(getCurrCliCurrStackSS(), arg.i % getSizeSS());
}

void toggleNSPN(ActionAr arg) {
  assert(arg.com);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  CliPtr c = findNSPCliSS();
  if (c && CLIVAL(c).ws == ws) {
    moveCliToWorkspaceW(c, getNSPStackSS());
  } else if (c) {
    moveCliToWorkspaceW(c, ws);
  } else {
    if (!getSizeNSPSS())
      spawnS(arg.cmd, NULL);
    else
      moveCliToWorkspaceW(getCurrCliNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}

void minimizeCliN(ActionAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  minimizeC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void restoreCliN(ActionAr arg) {
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

