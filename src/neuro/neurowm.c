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

void changeWMNameN(GenericAr arg) {
  assert(arg.com);
  changeWMNameS(arg.str);
}

void spawnN(GenericAr arg) {
  assert(arg.com);
  spawnS(arg.com, NULL);
}

void quitN(GenericAr arg) {
  (void)arg;
  stopWhile = True;
}

void reloadN(GenericAr arg) {
  (void)arg;
  signalHandler(SIGUSR1);
}

void killCliN(GenericAr arg) {
  (void)arg;
  killC(getCurrCliCurrStackSS());
}

void moveFocusN(GenericAr arg) {
  assert(arg.sf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  moveFocusW(getCurrCliStackSS(ws), arg.sf);
  addEnterNotifyMaskW(ws);
}

void swapCliN(GenericAr arg) {
  assert(arg.sf);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  swapClientsW(getCurrCliStackSS(ws), arg.sf);
  addEnterNotifyMaskW(ws);
}

void changeLayoutN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void resetLayoutN(GenericAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetLayoutL(ws);
  addEnterNotifyMaskW(ws);
}

void increaseMasterN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void resizeMasterN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, arg.f);
  addEnterNotifyMaskW(ws);
}

void changeToWorkspaceN(GenericAr arg) {
  rmvEnterNotifyMaskW(arg.i);
  changeToWorkspaceW(arg.i);
  addEnterNotifyMaskW(arg.i);
}

void changeToPrevWorkspaceN(GenericAr arg) {
  (void)arg;
  const int ws = getPrevStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToNextWorkspaceN(GenericAr arg) {
  (void)arg;
  const int ws = getNextStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void changeToLastWorkspaceN(GenericAr arg) {
  (void)arg;
  const int ws = getLastStackSS();
  rmvEnterNotifyMaskW(ws);
  changeToWorkspaceW(ws);
  addEnterNotifyMaskW(ws);
}

void toggleFreeCliN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFreeC(getCurrCliStackSS(ws), arg.ff);
  addEnterNotifyMaskW(ws);
}

void tileCliN(GenericAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  tileC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void freeCliN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  freeC(getCurrCliStackSS(ws), arg.ff);
  addEnterNotifyMaskW(ws);
}

void normalCliN(GenericAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  normalC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void fullScreenCliN(GenericAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  fullScreenC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void toggleFullScreenCliN(GenericAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  toggleFullScreenC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void toggleFreePtrCliN(GenericAr arg) {
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFreeC(getCurrCliCurrStackSS(), arg.ff);
}

void toggleFullScreenPtrCliN(GenericAr arg) {
  (void)arg;
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFullScreenC(getCurrCliCurrStackSS());
}

void freeMovePointerCliN(GenericAr arg) {
  (void)arg;
  freeMovePointerC();
}

void freeResizePointerCliN(GenericAr arg) {
  (void)arg;
  freeResizePointerC();
}

void movePointerCliN(GenericAr arg) {
  (void)arg;
  movePointerC();
}

void resizePointerCliN(GenericAr arg) {
  (void)arg;
  resizePointerC();
}

void toggleLayoutModN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, arg.ui);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutN(GenericAr arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, arg.i);
  addEnterNotifyMaskW(ws);
}

void moveCliToWorkspaceN(GenericAr arg) {
  moveCliToWorkspaceW(getCurrCliCurrStackSS(), arg.i % getSizeSS());
}

void moveCliToWorkspaceAndFollowN(GenericAr arg) {
  moveCliToWorkspaceAndFollowW(getCurrCliCurrStackSS(), arg.i % getSizeSS());
}

void toggleNSPN(GenericAr arg) {
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
      spawnS(arg.com, NULL);
    else
      moveCliToWorkspaceW(getCurrCliNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}

void minimizeCliN(GenericAr arg) {
  (void)arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  minimizeC(getCurrCliStackSS(ws));
  addEnterNotifyMaskW(ws);
}

void restoreCliN(GenericAr arg) {
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

