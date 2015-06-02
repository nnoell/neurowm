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
typedef int (*FlagHandler)();

// FlagHandlers
typedef struct FlagHandlers FlagHandlers;
struct FlagHandlers {
  char *name;
  FlagHandler handler;
  char *desc;
};


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

static int runCmd(char **cmd);
// static pid_t getNeurowmPid();
static int helpHandler();
static int versionHandler();
static int recompileHandler();
// static int reloadHandler();
static int runNeurowm(int argc, char **argv);
static int runFlag(char *flgname);
static int loopRunNeurowm(int argc, char **argv);


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Flags                                         NAME           HANDLER           DESC
static const FlagHandlers helpFlag[]      = { { "--help",      helpHandler,      "Print this message"           } };
static const FlagHandlers versionFlag[]   = { { "--version",   versionHandler,   "Print the version number"     } };
static const FlagHandlers recompileFlag[] = { { "--recompile", recompileHandler, "Recompile your configuration" } };
// static const FlagHandlers reloadFlag[]    = { { "--reload",    reloadHandler,    "Reload the window manager"    } };

// Flags array
static const FlagHandlers *allFlags[] = { helpFlag, versionFlag, recompileFlag, NULL };  // Removed reloadFlag


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int runCmd(char **cmd) {
  int res = 0, status;
  pid_t pid = fork();
  if (pid == -1) {
    perror("runCmd - Could not fork");
    exit(EXIT_FAILURE);
  }
  if (!pid) {  // Child
    res = execvp(cmd[ 0 ], cmd);
    exit(EXIT_FAILURE);
  }
  waitpid(pid, &status, WUNTRACED);
  return res == -1 ? res : status;
}

// static pid_t getNeurowmPid() {
//   char pidstr[ NAME_MAX ];
//   FILE *cmd = popen("/usr/bin/pidof -s " WM_MYNAME, "r");
//   fgets(pidstr, NAME_MAX, cmd);
//   pid_t wmpid = (pid_t)strtoul(pidstr, NULL, 10);
//   if (wmpid <= 0) // WM not running
//   return 0;
//   return wmpid;
// }

static int helpHandler() {
  printf("Usage: neurowm [OPTION]\nOptions:\n");
  int i;
  for (i=0; allFlags[ i ]; ++i)
    printf("  %s\t\t%s\n", allFlags[ i ]->name, allFlags[ i ]->desc);
  return 0;
}

static int versionHandler() {
  printf(WM_NAME " " VERSION "\n");
  return 0;
}

static int recompileHandler() {
  char out[ NAME_MAX ] = "", src[ NAME_MAX ] = "";
  snprintf(out, SNPRINTF_SIZE, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  snprintf(src, SNPRINTF_SIZE, "%s/." WM_NAME "/" WM_NAME ".c", getenv("HOME"));
  char lib[ NAME_MAX ] = "/usr/lib/neuro/lib" WM_NAME ".a";
  char *cmd[] = { "/usr/bin/cc", "-O3", "-o", out, src, lib, "-lX11", "-pthread", NULL };
  return runCmd(cmd);
}

// static int reloadHandler() {
//   pid_t wmpid = getNeurowmPid();
//   if (wmpid == 0)
//     return 0;
//   if (kill(wmpid, SIGUSR1) == -1)
//     return -1;
//   return 0;
// }

static int runNeurowm(int argc, char **argv) {
  char bin[ NAME_MAX ] = "", *cmd[ argc + 1 ];
  snprintf(bin, SNPRINTF_SIZE, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  cmd[ 0 ] = bin;
  cmd[ argc ] = NULL;
  int i;
  for (i = 1; i < argc; ++i)
    cmd[ i ] = argv[ i ];
  return runCmd(cmd);
}

static int runFlag(char *flgname) {
  int i;
  for (i=0; allFlags[ i ]; ++i)
    if (!strcmp(flgname, allFlags[ i ]->name)) {
      int res = allFlags[ i ]->handler();
      if (res == -1)
        perror(flgname);
      return res;
    }
  return 1;
}

static int loopRunNeurowm(int argc, char **argv) {
  int res;
  do {
    res = runNeurowm(argc, argv);
  } while (WEXITSTATUS(res) == EXIT_RELOAD);
  return res;
}

//----------------------------------------------------------------------------------------------------------------------
// MAIN DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {
  int runwm = 1, reswm = 0;
  if (argc > 1) {
    int i, res;
    for (i = 1; i < argc; ++i) {
      res = runFlag(argv[ i ]);
      if (res == -1 || res == 0)  // -1 error, 0 dont run, 1 run wm
        runwm = 0;
    }
  }
  if (runwm)
    reswm = loopRunNeurowm(argc, argv);
  return reswm == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}
