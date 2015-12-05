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
#include "neuro/neurowm.h"

// Defines
#define SNPRINTF_SIZE 256


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// FlagHandler
typedef int (*const FlagHandlerFn)();

// FlagHandlers
typedef struct FlagHandlers FlagHandlers;
struct FlagHandlers {
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

// Flags                                        NAME           HANDLER           DESC
static const FlagHandlers help_flag[]      = { { "--help",      help_handler,      "Print this message"           } };
static const FlagHandlers version_flag[]   = { { "--version",   version_handler,   "Print the version number"     } };
static const FlagHandlers recompile_flag[] = { { "--recompile", recompile_handler, "Recompile your configuration" } };
// static const FlagHandlers reload_flag[]   = { { "--reload",    reload_handler,    "Reload the window manager"    } };

// Flags array
static const FlagHandlers *flag_set[] = { help_flag, version_flag, recompile_flag, NULL };  // Removed reload_flag


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int run_cmd(const char *const *cmd) {
  assert(cmd);
  int res = 0, status;
  const pid_t pid = fork();
  if (pid == -1) {
    perror("run_cmd - Could not fork");
    exit(EXIT_FAILURE);
  }
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
  for (i=0; flag_set[ i ]; ++i)
    printf("  %s\t\t%s\n", flag_set[ i ]->name, flag_set[ i ]->desc);
  return 0;
}

static int version_handler() {
  printf(WM_NAME " " VERSION "\n");
  return 0;
}

static int recompile_handler() {
  char out[ NAME_MAX ] = "", src[ NAME_MAX ] = "";
  snprintf(out, SNPRINTF_SIZE, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  snprintf(src, SNPRINTF_SIZE, "%s/." WM_NAME "/" WM_NAME ".c", getenv("HOME"));
  char lib[ NAME_MAX ] = "/usr/lib/neuro/lib" WM_NAME ".a";
  const char *const cmd[] = { "/usr/bin/cc", "-O3", "-o", out, src, lib, "-lX11", "-pthread", NULL };
  return run_cmd(cmd);
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
  char bin[ NAME_MAX ] = "";
  snprintf(bin, SNPRINTF_SIZE, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  cmd[ 0 ] = bin;
  cmd[ argc ] = NULL;
  int i;
  for (i = 1; i < argc; ++i)
    cmd[ i ] = argv[ i ];
  return run_cmd((const char *const *)cmd);
}

static int run_flag(const char *flgname) {
  assert(flgname);
  int i;
  for (i=0; flag_set[ i ]; ++i)
    if (!strcmp(flgname, flag_set[ i ]->name)) {
      int res = flag_set[ i ]->handler();
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
