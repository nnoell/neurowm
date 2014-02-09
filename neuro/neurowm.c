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
  char *cmd[] = { "/usr/bin/cc", "-O3", "-o", out, src, lib, "-lX11", "-pthread", NULL };
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
  // Init base (must free with endB at some point)
  if (!initB(c))
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
  spawnG((char **)arg.com, NULL);
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
  moveFocusW(getCurrCliCurrStackSS(), arg.sf);
  XSync(display, True);
}

void swapCliN(Arg arg) {
  swapClientsW(getCurrCliCurrStackSS(), arg.sf);
  XSync(display, True);
}

void changeLayoutN(Arg arg) {
  changeLayoutL(getCurrStackSS(), arg.i);
  XSync(display, True);
}

void resetLayoutN(Arg arg) {
  (void)arg;
  resetLayoutL(getCurrStackSS());
  XSync(display, True);
}

void increaseMasterN(Arg arg) {
  increaseMasterL(getCurrStackSS(), arg.i);
  XSync(display, True);
}

void resizeMasterN(Arg arg) {
  resizeMasterL(getCurrStackSS(), arg.i);
  XSync(display, True);
}

void changeToWorkspaceN(Arg arg) {
  changeToWorkspaceW(arg.i);
  XSync(display, True);
}

void changeToPrevWorkspaceN(Arg arg) {
  (void)arg;
  changeToPrevWorkspaceW();
  XSync(display, True);
}

void changeToNextWorkspaceN(Arg arg) {
  (void)arg;
  changeToNextWorkspaceW();
  XSync(display, True);
}

void changeToLastWorkspaceN(Arg arg) {
  (void)arg;
  changeToLastWorkspaceW();
  XSync(display, True);
}

void toggleFreeCliN(Arg arg) {
  toggleFreeC(getCurrCliCurrStackSS(), arg.ff);
  XSync(display, True);
}

void tileCliN(Arg arg) {
  (void)arg;
  tileC(getCurrCliCurrStackSS());
  XSync(display, True);
}

void freeCliN(Arg arg) {
  freeC(getCurrCliCurrStackSS(), arg.ff);
  XSync(display, True);
}

void normalCliN(Arg arg) {
  (void)arg;
  normalC(getCurrCliCurrStackSS());
  XSync(display, True);
}

void fullScreenCliN(Arg arg) {
  (void)arg;
  fullScreenC(getCurrCliCurrStackSS());
  XSync(display, True);
}

void toggleFullScreenCliN(Arg arg) {
  (void)arg;
  toggleFullScreenC(getCurrCliCurrStackSS());
  XSync(display, True);
}

void toggleFreePtrCliN(Arg arg) {
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFreeC(getCurrCliCurrStackSS(), arg.ff);
  XSync(display, True);
}

void toggleFullScreenPtrCliN(Arg arg) {
  (void)arg;
  moveFocusW(getCurrCliCurrStackSS(), pointerC);
  toggleFullScreenC(getCurrCliCurrStackSS());
  XSync(display, True);
}

void freeMovePointerCliN(Arg arg) {
  (void)arg;
  freeMovePointerC();
  XSync(display, True);
}

void freeResizePointerCliN(Arg arg) {
  (void)arg;
  freeResizePointerC();
  XSync(display, True);
}

void movePointerCliN(Arg arg) {
  (void)arg;
  movePointerC();
  XSync(display, True);
}

void resizePointerCliN(Arg arg) {
  (void)arg;
  resizePointerC();
  XSync(display, True);
}

void toggleLayoutModN(Arg arg) {
  togModCurrLayoutL(getCurrStackSS(), arg.ui);
  XSync(display, True);
}

void toggleLayoutN(Arg arg) {
  togLayoutL(getCurrStackSS(), arg.i);
  XSync(display, True);
}

void moveCliToWorkspaceN(Arg arg) {
  moveCliToWorkspaceW(getCurrCliCurrStackSS(), arg.i);
  XSync(display, True);
}

void moveCliToWorkspaceAndFollowN(Arg arg) {
  moveCliToWorkspaceAndFollowW(getCurrCliCurrStackSS(), arg.i);
  XSync(display, True);
}

void toggleNSPN(Arg arg) {
  int currWS = getCurrStackSS();
  CliPtr c = findNSPCliSS();
  if (c && CLIVAL(c).ws == currWS) {
    moveCliToNSPWorkspaceW(c);
  } else if (c) {
    moveCliToWorkspaceW(c, currWS);
  } else {
    if (!getSizeNSPSS())
      spawnG((char **)arg.com, NULL);
    else
      moveNSPCliToWorkspaceW(currWS);
  }
  XSync(display, True);
}

void minimizeCliN(Arg arg) {
  (void)arg;
  minimizeC(getCurrCliCurrStackSS());
  XSync(display, True);
}

void restoreCliN(Arg arg) {
  (void)arg;
  restoreLastMinimizedC();
  XSync(display, True);
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

