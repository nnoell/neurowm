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
#include "base.h"
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
  return spawnG(cmd, pid);
}

static void endNeurowm() {
  // End endup hook
  int i;
  for (i = 0; endUpHookB[ i ]; ++i)
    endUpHookB[ i ]->func(endUpHookB[ i ]->arg);

  // End panels
  endDP();

  // End stackset
  endSS();

  // End base
  endB();
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
  assert(c);

  // Set configuration
  setConfigB(c);

  // Init base (must free with endB at some point)
  if (!initB())
    exitErrorG("initNeurowm - could not init Base");

  // Init stackset (must free with endSS at some point)
  if (!initSS())
    exitErrorG("initNeurowm - could not init StackSet");

  // Init panels (must free with endDP at some point)
  if (!initDP())
    exitErrorG("initNeurowm - could not init Panels");

  // Init startup hook
  int i;
  for (i = 0; startUpHookB[ i ]; ++i)
    startUpHookB[ i ]->func(startUpHookB[ i ]->arg);

  // Catch asynchronously SIGUSR1
  // if (SIG_ERR == signal(SIGUSR1, signalHandler))
  //   exitErrorG("initNeurowm - could not set SIGHUP handler");

  // Load existing windows if Xsesion was not closed
  loadWindowsE();
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void spawnN(Arg arg) {
  spawnG(arg.com, NULL);
}

void quitN(Arg arg) {
  (void)arg;
  stopWhile = True;
}

void reloadN(Arg arg) {
  (void)arg;
  signalHandler(SIGUSR1);
}

void killCliN(Arg arg) {
  (void)arg;
  killC(getCurrCliCurrStackSS());
}

void moveFocusN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  moveFocusW(getCurrCliStackSS(ws), arg.sf);
  addEnterNotifyMaskW(ws);
}

void swapCliN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  swapClientsW(getCurrCliStackSS(ws), arg.sf);
  addEnterNotifyMaskW(ws);
}

void changeLayoutN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void resetLayoutN(Arg arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetLayoutL(ws);
  addEnterNotifyMaskW(ws);
}

void increaseMasterN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void resizeMasterN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void changeToWorkspaceN(Arg arg) {
  rmvEnterNotifyMaskW(arg.i);
  changeToWorkspaceW(arg.i);
  addEnterNotifyMaskW(arg.i);
}

void changeToPrevWorkspaceN(Arg arg) {
  (void)arg;
  const int ws = getPrevStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToNextWorkspaceN(Arg arg) {
  (void)arg;
  const int ws = getNextStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToLastWorkspaceN(Arg arg) {
  (void)arg;
  const int ws = getLastStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void toggleFreeCliN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFreeC(getCurrCliStackSS(ws), arg.ff);
  addEnterNotifyMaskW(ws);
}

void tileCliN(Arg arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  tileC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void freeCliN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  freeC(getCurrCliStackSS(ws), arg.ff);
  addEnterNotifyMaskW(ws);
}

void normalCliN(Arg arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  normalC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void fullScreenCliN(Arg arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  fullScreenC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void toggleFullScreenCliN(Arg arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFullScreenC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void toggleFreePtrCliN(Arg arg) {
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFreeC(getCurrCliCurrStackSS(), arg.ff);
}

void toggleFullScreenPtrCliN(Arg arg) {
  (void)arg;
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFullScreenC(getCurrCliCurrStackSS());
}

void freeMovePointerCliN(Arg arg) {
  (void)arg;
  freeMovePointerC();
}

void freeResizePointerCliN(Arg arg) {
  (void)arg;
  freeResizePointerC();
}

void movePointerCliN(Arg arg) {
  (void)arg;
  movePointerC();
}

void resizePointerCliN(Arg arg) {
  (void)arg;
  resizePointerC();
}

void toggleLayoutModN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, arg.ui);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void moveCliToWorkspaceN(Arg arg) {
  moveCliToWorkspaceW(getCurrCliCurrStackSS(), arg.i % getSizeSS());
}

void moveCliToWorkspaceAndFollowN(Arg arg) {
  moveCliToWorkspaceAndFollowW(getCurrCliCurrStackSS(), arg.i % getSizeSS());
}

void toggleNSPN(Arg arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  CliPtr c = findNSPCliSS();
  if (c && CLIVAL(c).ws == ws) {
    moveCliToWorkspaceW(c, getNSPStackSS());
  } else if (c) {
    moveCliToWorkspaceW(c, ws);
  } else {
    if (!getSizeNSPSS())
      spawnG(arg.com, NULL);
    else
      moveCliToWorkspaceW(getCurrCliNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}

void minimizeCliN(Arg arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  minimizeC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void restoreCliN(Arg arg) {
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

