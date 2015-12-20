//----------------------------------------------------------------------------------------------------------------------
// Program     :  main
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "neuro/system.h"


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// FlagHandlerFn
typedef Bool (*const FlagHandlerFn)();

// Flag
typedef struct Flag Flag;
struct Flag {
  const char *const name;
  const FlagHandlerFn handler;
  const char *const desc;
};


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

static Bool run_cmd(const char *const *cmd, int *status);
static Bool help_handler();
static Bool version_handler();
static Bool recompile_handler();
// static Bool reload_handler();
static Bool run_neurowm(int argc, const char *const *argv, int *status);
static Bool run_flag(const char *flgname);
static Bool loop_run_neurowm(int argc, const char *const *argv);


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Flags                              NAME           HANDLER            DESC
static const Flag help_flag_      = { "--help",      help_handler,      "Print this message"           };
static const Flag version_flag_   = { "--version",   version_handler,   "Print the version number"     };
static const Flag recompile_flag_ = { "--recompile", recompile_handler, "Recompile your configuration" };
// static const Flag reload_flag_ = { "--reload",    reload_handler,    "Reload the window manager"    };

// Flags array
static const Flag* flag_list_[] = { &help_flag_, &version_flag_, &recompile_flag_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Bool run_cmd(const char *const *cmd, int *status) {
  assert(cmd);
  const pid_t pid = fork();
  if (pid == -1)
    return False;
  if (!pid) {  // Child
    execvp(cmd[ 0 ], (char *const *)cmd);
    NeuroSystemError("run_cmd - Could not execvp");
  }
  waitpid(pid, status, WUNTRACED);
  return True;
}

static Bool help_handler() {
  printf("Usage: neurowm [OPTION]\nOptions:\n");
  int i;
  for (i=0; flag_list_[ i ]; ++i)
    printf("  %s\t\t%s\n", flag_list_[ i ]->name, flag_list_[ i ]->desc);
  return True;
}

static Bool version_handler() {
  printf(PKG_NAME " " PKG_VERSION "\n");
  return True;
}

static Bool recompile_handler() {
  int status;
  return run_cmd(NeuroSystemGetRecompileCommand(NULL, NULL), &status);
}

//  static Bool reload_handler() {
//    return kill(NeuroSystemGetWmPid(), SIGUSR1) != -1;
//  }

static Bool run_neurowm(int argc, const char *const *argv, int *status) {
  assert(argv);
  const char *cmd[ argc + 1 ];
  NeuroSystemGetRecompileCommand(cmd + 0, NULL);
  cmd[ argc ] = NULL;
  int i;
  for (i = 1; i < argc; ++i)
    cmd[ i ] = argv[ i ];
  return run_cmd((const char *const *)cmd, status);
}

static Bool run_flag(const char *flgname) {
  assert(flgname);
  int i;
  for (i=0; flag_list_[ i ]; ++i)
    if (!strcmp(flgname, flag_list_[ i ]->name)) {
      Bool res = flag_list_[ i ]->handler();
      if (!res)
        perror(flgname);
      return True;
    }
  return False;
}

static Bool loop_run_neurowm(int argc, const char *const *argv) {
  assert(argv);
  int status;
  do {
    if (!run_neurowm(argc, argv, &status))
      return False;
  } while (WEXITSTATUS(status) == EXIT_RELOAD);
  return True;
}

//----------------------------------------------------------------------------------------------------------------------
// MAIN DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, const char *const *argv) {
  Bool runwm = True, reswm = True;
  if (argc > 1) {
    int i;
    for (i = 1; i < argc; ++i)
      runwm = !run_flag(argv[ i ]);
  }
  if (runwm)
    reswm = loop_run_neurowm(argc, argv);
  return reswm ? EXIT_SUCCESS : EXIT_FAILURE;
}
