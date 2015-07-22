//----------------------------------------------------------------------------------------------------------------------
// Module      :  general
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "general.h"
#include "area.h"
#include "rule.h"
#include "base.h"


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

Client *allocClientG(Window w, const XWindowAttributes *wa) {
  Client *c = (Client *)malloc(sizeof(Client));
  if (!c)
    return NULL;
  *(Window *)&c->win = w;  // Pointer casted away because of const
  c->ws = 0;
  c->isNSP = False;
  c->class[ 0 ] = '\0';
  c->name[ 0 ] = '\0';
  c->title[ 0 ] = '\0';
  c->isHidden = False;
  c->freeLocFunc = notFreeR;
  c->fixPos = notFixedR;
  c->fixSize = 0;
  c->isUrgent = False;
  c->isFullScreen = False;
  setAreaA(&(c->floatRegion), wa->x, wa->y, wa->width, wa->height);
  return c;
}

void freeClientG(Client *c) {
  free(c);
  c = NULL;
}

size_t ptrArrayLengthG(const void *const *arrayPtr) {
  size_t size;
  for (size = 0; arrayPtr[ size ]; ++size)
    continue;
  return size;
}

void changeProcName(const char *newname) {
  prctl(PR_SET_NAME, (unsigned long)newname, 0, 0, 0);
}

int spawnG(char **cmd, pid_t *p) {
  pid_t pid = fork();
  if (pid == -1)
    return -1;
  if (!pid) {  // Child
    if (display)
      close(ConnectionNumber(display));
    setsid();
    execvp(cmd[ 0 ], cmd);
    exitErrorG("spawnG - Could not execvp");
  }
  if (p)
    *p = pid;
  return 0;
}

int spawnPipeG(char **cmd, pid_t *p) {
  int filedes[ 2 ];
  if (pipe(filedes))
    return -1;
  pid_t pid = fork();
  if (pid < 0)
    return -1;
  if (pid == 0) {  // Child
    if (display)
      close(ConnectionNumber(display));
    setsid();
    dup2(filedes[ 0 ], STDIN_FILENO);
    close(filedes[ 0 ]);
    execvp(cmd[ 0 ], cmd);
    exit(EXIT_FAILURE);
  }
  if (p)
    *p = pid;
  return filedes[ 1 ];
}

void exitErrorG(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

