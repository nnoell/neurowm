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
typedef int (*const FlagHandlerFn)();

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

static int run_cmd(const char *const *cmd);
// static pid_t get_neurowm_pid();
static int help_handler();
static int version_handler();
static int recompile_handler();
// static int reload_handler();
static int run_neurowm(int argc, const char *const *argv);
static int run_flag(const char *flgname);
static int loop_run_neurowm(int argc, const char *const *argv);


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Flags                                  NAME           HANDLER            DESC
static const Flag help_flag_[]      = { { "--help",      help_handler,      "Print this message"           } };
static const Flag version_flag_[]   = { { "--version",   version_handler,   "Print the version number"     } };
static const Flag recompile_flag_[] = { { "--recompile", recompile_handler, "Recompile your configuration" } };
// static const Flag reload_flag_[] = { { "--reload",    reload_handler,    "Reload the window manager"    } };

// Flags array
static const Flag *flag_set_[] = { help_flag_, version_flag_, recompile_flag_, NULL };


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int run_cmd(const char *const *cmd) {
  assert(cmd);
  int res = 0, status;
  const pid_t pid = fork();
  if (pid == -1)
    NeuroSystemError("run_cmd - Could not fork");
  if (!pid) {  // Child
    res = execvp(cmd[ 0 ], (char *const *)cmd);
    exit(EXIT_FAILURE);
  }
  waitpid(pid, &status, WUNTRACED);
  return res == -1 ? res : status;
}

//  static pid_t get_neurowm_pid() {
//    char pidstr[ NAME_MAX ];
//    FILE *cmd = popen("/usr/bin/pidof -s " WM_MYNAME, "r");
//    fgets(pidstr, NAME_MAX, cmd);
//    return (pid_t)strtoul(pidstr, NULL, 10);
//  }

static int help_handler() {
  printf("Usage: neurowm [OPTION]\nOptions:\n");
  int i;
  for (i=0; flag_set_[ i ]; ++i)
    printf("  %s\t\t%s\n", flag_set_[ i ]->name, flag_set_[ i ]->desc);
  return 0;
}

static int version_handler() {
  printf(WM_NAME " " VERSION "\n");
  return 0;
}

static int recompile_handler() {
  return run_cmd(NeuroSystemGetRecompileCommand(NULL, NULL));
}

//  static int reload_handler() {
//    const pid_t wmpid = get_neurowm_pid();
//    if (wmpid <= 0)
//      return 0;
//    return kill(wmpid, SIGUSR1) == -1 ? -1 : 0;
//  }

static int run_neurowm(int argc, const char *const *argv) {
  assert(argv);
  const char *cmd[ argc + 1 ];
  NeuroSystemGetRecompileCommand(cmd + 0, NULL);
  cmd[ argc ] = NULL;
  int i;
  for (i = 1; i < argc; ++i)
    cmd[ i ] = argv[ i ];
  return run_cmd((const char *const *)cmd);
}

static int run_flag(const char *flgname) {
  assert(flgname);
  int i;
  for (i=0; flag_set_[ i ]; ++i)
    if (!strcmp(flgname, flag_set_[ i ]->name)) {
      int res = flag_set_[ i ]->handler();
      if (res == -1)
        perror(flgname);
      return res;
    }
  return 1;
}

static int loop_run_neurowm(int argc, const char *const *argv) {
  assert(argv);
  int res;
  do {
    res = run_neurowm(argc, argv);
  } while (WEXITSTATUS(res) == EXIT_RELOAD);
  return res;
}

//----------------------------------------------------------------------------------------------------------------------
// MAIN DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, const char *const *argv) {
  Bool runwm = True;
  if (argc > 1) {
    int i, res;
    for (i = 1; i < argc; ++i) {
      res = run_flag(argv[ i ]);
      runwm = res != -1 && res != 0;  // res: -1 error, 0 dont run, 1 run wm
    }
  }
  int reswm = 0;
  if (runwm)
    reswm = loop_run_neurowm(argc, argv);
  return reswm == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}
