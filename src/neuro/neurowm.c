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
#include "core.h"
#include "event.h"
#include "dzen.h"


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
  NeuroActionUtilRunActionChain(endUpHookS);
  NeuroDzenStop();
  NeuroCoreStop();
  stopS();
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

static void initNeurowm(const Configuration *c) {
  if (!c)
    exitErrorS("initNeurowm - could not set configuration");

  // Set configuration and init base, stackset and panels
  setConfigS(c);
  if (!initS())
    exitErrorS("initNeurowm - could not init Base");
  if (!NeuroCoreInit())
    exitErrorS("initNeurowm - could not init StackSet");
  if (!NeuroDzenInit())
    exitErrorS("initNeurowm - could not init Panels");

  NeuroActionUtilRunActionChain(startUpHookS);

  // Catch asynchronously SIGUSR1
  // if (SIG_ERR == signal(SIGUSR1, signalHandler))
  //   exitErrorG("initNeurowmN - could not set SIGHUP handler");

  // Load existing windows if Xsesion was not closed
  loadWindowsE();
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int runN(const Configuration *c) {
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

void quitN() {
  stopWhile = True;
}

void reloadN() {
  signalHandler(SIGUSR1);
}

