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

static int recompile_wm(pid_t *pid) {
  char out[ NAME_MAX ] = "", src[ NAME_MAX ] = "";
  snprintf(out, NAME_MAX, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  snprintf(src, NAME_MAX, "%s/." WM_NAME "/" WM_NAME ".c", getenv("HOME"));
  char lib[ NAME_MAX ] = "/usr/lib/neuro/lib" WM_NAME ".a";
  const char *const cmd[] = { "/usr/bin/cc", "-O3", "-o", out, src, lib, "-lX11", "-pthread", NULL };
  return spawnS(cmd, pid);
}

static void stop_wm() {
  NeuroActionUtilRunActionChain(endUpHookS);
  NeuroDzenStop();
  NeuroCoreStop();
  stopS();
}

static void wm_signal_handler(int signo) {
  if (signo == SIGUSR1) {
    stop_wm();
    pid_t pid;
    if (recompile_wm(&pid) == -1)
      perror("wm_signal_handler - Could not recompile neurowm");
    waitpid(pid, NULL, WUNTRACED);
    exit(EXIT_RELOAD);
  }
}

static void init_wm(const Configuration *c) {
  if (!c)
    exitErrorS("init_wm - could not set configuration");

  // Set configuration and init base, stackset and panels
  setConfigS(c);
  if (!initS())
    exitErrorS("init_wm - could not init Base");
  if (!NeuroCoreInit())
    exitErrorS("init_wm - could not init StackSet");
  if (!NeuroDzenInit())
    exitErrorS("init_wm - could not init Panels");

  NeuroActionUtilRunActionChain(startUpHookS);

  // Catch asynchronously SIGUSR1
  // if (SIG_ERR == signal(SIGUSR1, signalHandler))
  //   exitErrorS("init_wm - could not set SIGHUP handler");

  // Load existing windows if Xsesion was not closed
  NeuroEventLoadWindows();
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int NeuroNeurowmRun(const Configuration *c) {
  // Init window manager
  init_wm(c);

  // Main loop
  XEvent ev;
  while (!stopWhile && !XNextEvent(display, &ev))
    if (NeuroEventEventArray[ ev.type ])
      NeuroEventEventArray[ ev.type ](&ev);

  // Stop window manager
  stop_wm();

  return EXIT_SUCCESS;
}

void NeuroNeurowmQuit() {
  stopWhile = True;
}

void NeuroNeurowmReload() {
  wm_signal_handler(SIGUSR1);
}

