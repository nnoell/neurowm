//----------------------------------------------------------------------------------------------------------------------
// Module      :  wm
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "wm.h"
#include "system.h"
#include "config.h"
#include "core.h"
#include "event.h"
#include "dzen.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

static bool stop_main_while_ = false;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static bool recompile_wm(pid_t *pid) {
  return NeuroSystemSpawn(NeuroSystemGetRecompileCommand(NULL, NULL), pid);
}

static void stop_wm(void) {
  NeuroActionRunActionChain(&NeuroConfigGet()->stop_action_chain);
  NeuroDzenStop();
  NeuroCoreStop();
  NeuroMonitorStop();
  NeuroSystemStop();
}

static void wm_signal_handler(int signo) {
  if (signo == SIGUSR1) {
    stop_wm();
    pid_t pid;
    if (!recompile_wm(&pid))
      perror("wm_signal_handler - Could not recompile neurowm");
    waitpid(pid, NULL, WUNTRACED);
    exit(NEURO_EXIT_RELOAD);
  }
}

static void init_wm(const NeuroConfiguration *c) {
  // Set the configuration
  NeuroConfigSet(c);

  // Init System, NeuroMonitor, Core and Panels
  if (!NeuroSystemInit())
    NeuroSystemError(__func__, "Could not init System module");
  if (!NeuroMonitorInit())
    NeuroSystemError(__func__, "Could not init Monitor module");
  if (!NeuroCoreInit())
    NeuroSystemError(__func__, "Could not init Core module");
  if (!NeuroDzenInit())
    NeuroSystemError(__func__, "Could not init Dzen module");

  // Change to the default workspace
  NeuroWorkspaceChange(NeuroMonitorSelectorHead(NULL)->default_ws);

  // Run the init action chain
  NeuroActionRunActionChain(&NeuroConfigGet()->init_action_chain);

  // Catch asynchronously SIGUSR1
  // if (SIG_ERR == signal(SIGUSR1, wm_signal_handler))
  //   NeuroSystemError("init_wm - Could not set SIGHUP handler");

  // Load existing windows if Xsesion was not closed
  NeuroEventLoadWindows();
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int NeuroWmRun(const NeuroConfiguration *c) {
  // Init window manager
  init_wm(c);

  // Main loop
  XEvent ev;
  while (!stop_main_while_ && !XNextEvent(NeuroSystemGetDisplay(), &ev)) {
    const NeuroEventHandlerFn eh = NeuroEventGetHandler(ev.type);
    if (eh)
      eh(&ev);
  }

  // Stop window manager
  stop_wm();

  return EXIT_SUCCESS;
}

void NeuroWmQuit(void) {
  stop_main_while_ = true;
}

void NeuroWmReload(void) {
  wm_signal_handler(SIGUSR1);
}

