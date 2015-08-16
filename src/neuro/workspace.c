//----------------------------------------------------------------------------------------------------------------------
// Module      :  workspace
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "workspace.h"
#include "system.h"
#include "stackset.h"
#include "layout.h"
#include "client.h"
#include "rule.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Bool isAboveTiledClientW(const CliPtr c) {
  assert(c);
  return (CLIVAL(c).freeLocFn != notFreeR) || CLIVAL(c).isFullScreen;
}

static void focusClientW(CliPtr c) {
  assert(c);
  unsetUrgentC(c);
  ungrabButtonsS(CLIVAL(c).win);
  XSetInputFocus(display, CLIVAL(c).win, RevertToPointerRoot, CurrentTime);
  XChangeProperty(display, root, netatoms[ NET_ACTIVE ], XA_WINDOW, 32, PropModeReplace,
      (unsigned char *)&(CLIVAL(c).win), 1);
  updateC(c);
}

static void unfocusClientW(CliPtr c) {
  assert(c);
  grabButtonsS(CLIVAL(c).win);
  updateC(c);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void updateW(int ws) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    updateC(c);
}

void updateFocusW(int ws) {
  int n = getSizeStackSS(ws);
  if (n <= 0) {
    XDeleteProperty(display, root, netatoms[ NET_ACTIVE ]);
    return;
  }

  Window windows[ n ], d1, d2, *wins = NULL;
  unsigned int i, num;
  int atc = 0;
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    if (isAboveTiledClientW(c))
      ++atc;

  c = getCurrClientStackSS(ws);
  windows[ isAboveTiledClientW(c) ? 0 : atc ] = CLIVAL(c).win;
  focusClientW(c);

  if (n > 1) {
    if (!XQueryTree(display, root, &d1, &d2, &wins, &num))  // XQueryTree gets windows by stacking order
      exitErrorS("updateFocusW - could not get windows");
    int n2 = n;
    for (i = 0; i < num; ++i) {
      CliPtr c = findWindowClientW(ws, wins[ i ]);
      if (!c)
        continue;
      if (isCurrClientSS(c))
        continue;
      windows[ isAboveTiledClientW(c) ? --atc : --n2 ] = CLIVAL(c).win;
      unfocusClientW(c);
    }
    if (wins)
      XFree(wins);
  }

  XRestackWindows(display, windows, n);
}

void hideW(int ws, Bool doRules) {
  CliPtr c;
  for (c=getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    hideC(c, doRules);
}

void showW(int ws, Bool doRules) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    showC(c, doRules);
}

void tileW(int ws) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    tileC(c);
}

void freeW(int ws, const FreeLocFn flf) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    freeC(c, flf);
}

void changeToWorkspaceW(int ws) {
  int old = getCurrStackSS();
  setCurrStackSS(ws);
  int new = getCurrStackSS();
  if (old == new)
    return;
  hideW(old, True);
  showW(new, True);
  runCurrLayoutL(new);
  updateFocusW(new);
}

void moveClientToWorkspaceW(CliPtr c, int ws) {
  if (!c)
    return;
  if (ws < 0 || ws >= getSizeSS() + 1)
    return;
  int oldws = CLIVAL(c).ws, currws = getCurrStackSS();
  if (oldws == ws)
    return;
  Rectangle oldRegion = (Rectangle){ .x = 0, .y = 0, .h = 0, .w = 0 };
  Bool isFree = CLIVAL(c).freeLocFn != notFreeR;
  if (oldws == currws)
    hideC(c, True);
  if (isFree)
    memmove(&oldRegion, getRegionClientSS(c), sizeof(Rectangle));
  Client *cli = rmvClientSS(c);
  cli->ws = ws;
  CliPtr c2 = addClientStartSS(cli);
  if (!c2)
    exitErrorS("moveCliToWorkspaceW - could not add client");
  if (isFree)
    memmove(getRegionClientSS(c2), &oldRegion, sizeof(Rectangle));
  if (ws == currws)
    showC(c2, True);
  runCurrLayoutL(currws);
  updateFocusW(currws);
}

void moveClientToWorkspaceAndFollowW(CliPtr c, int ws) {
  if (!c)
    return;
  if (ws < 0 || ws >= getSizeSS() + 1)
    return;
  moveClientToWorkspaceW(c, ws);
  changeToWorkspaceW(ws);
}

void minimizeW(int ws) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    minimizeC(c);
}

void restoreLastMinimizedW(int ws) {
  if (getMinimizedNumStackSS(ws) <= 0)
    return;
  Client *cli = popMinimizedClientSS(ws);
  if (!cli)
    exitErrorS("restoreLastMinimizedW - could not restore last minimized client");
  CliPtr c = addClientStartSS(cli);
  if (!c)
    exitErrorS("restoreLastMinimizedW - could not add client");
  applyRuleR(c);
  setCurrClientSS(c);
  runCurrLayoutL(CLIVAL(c).ws);
  updateFocusW(CLIVAL(c).ws);
}

void addEnterNotifyMaskW(int ws) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    XSelectInput(display, CLIVAL(c).win, CLIENT_MASK);
}

void rmvEnterNotifyMaskW(int ws) {
  CliPtr c;
  for (c = getHeadClientStackSS(ws); c; c = getNextClientSS(c))
    XSelectInput(display, CLIVAL(c).win, CLIENT_MASK_NO_ENTER);
}


// Clients
void killClientW(const CliPtr c, const SelectCliFn scf) {
  if (!c || !scf)
    return;
  CliPtr dst = scf(c);
  if (!dst)
    return;
  killC(dst);
}

void moveFocusClientW(const CliPtr c, const SelectCliFn scf) {
  if (!c || !scf)
    return;
  if (getSizeStackSS(CLIVAL(c).ws) < 2)
    return;
  CliPtr dst = scf(c);
  if (!dst)
    return;
  setCurrClientSS(dst);
  updateFocusW(CLIVAL(dst).ws);
}

void swapClientW(const CliPtr c, const SelectCliFn scf) {
  if (!c || !scf)
    return;
  if (getSizeStackSS(CLIVAL(c).ws) < 2)
    return;
  CliPtr dst = scf(c);
  if (!dst)
    return;
  swpClientSS(c, dst);
  updateW(CLIVAL(c).ws);
  moveFocusClientW(c, scf);
}

void toggleFullScreenClientW(const CliPtr c, const SelectCliFn scf) {
  if (!c || !scf)
    return;
  CliPtr dst = scf(c);
  if (!dst)
    return;
  toggleFullScreenC(dst);
}

void minimizeClientW(const CliPtr c, const SelectCliFn scf) {
  if (!c || !scf)
    return;
  CliPtr dst = scf(c);
  if (!dst)
    return;
  minimizeC(dst);
}


// Find functions
CliPtr findWindowClientAllW(Window w) {
  return findClientSS(testWindowC, (const void *)&w);
}

CliPtr findWindowClientW(int ws, Window w) {
  return findClientStackSS(ws, testWindowC, (const void *)&w);
}

CliPtr findUrgentClientW(int ws) {
  return findClientStackSS(ws, testIsUrgentC, NULL);
}

CliPtr findFixedClientW(int ws) {
  return findClientStackSS(ws, testIsFixedC, NULL);
}

