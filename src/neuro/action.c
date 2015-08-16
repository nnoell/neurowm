//----------------------------------------------------------------------------------------------------------------------
// Module      :  action
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "action.h"
#include "system.h"
#include "stackset.h"
#include "dzenpanel.h"
#include "neurowm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void processWSActionA(const GenericWSActionFn gwsaf, int ws) {
  assert(gwsaf);
  rmvEnterNotifyMaskW(ws);
  gwsaf(ws);
  addEnterNotifyMaskW(ws);
}

static void processCliActionA(const GenericCliActionFn gcaf, CliPtr c, const SelectCliFn scf, const void *data) {
  assert(gcaf);
  assert(scf);
  if (!c)
    return;
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  gcaf(c, scf, data);
  addEnterNotifyMaskW(CLIVAL(c).ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Window Manager
void quitNeurowmA(ActionAr no_arg) {
  (void)no_arg;
  quitNeurowmN();
}

void reloadNeurowmA(ActionAr no_arg) {
  (void)no_arg;
  reloadNeurowmN();
}

void changeNeurowmNameA(ActionAr string_arg) {
  assert(string_arg.string_);
  changeWMNameS(string_arg.string_);
}

void spawnA(ActionAr command_arg) {
  assert(command_arg.command_);
  spawnS(command_arg.command_, NULL);
}

void sleepA(ActionAr int_arg) {
  assert(int_arg.int_ >= 0);
  sleep(int_arg.int_);
}

void startCpuCalcA(ActionAr no_arg) {
  (void)no_arg;
  startCpuCalcDP();
}

void endCpuCalcA(ActionAr no_arg) {
  (void)no_arg;
  endCpuCalcDP();
}


// Layout
void changeLayoutA(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  changeLayoutL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void resetLayoutA(ActionAr no_arg) {
  (void)no_arg;
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resetLayoutL(ws);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutModA(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togModCurrLayoutL(ws, int_arg.uint_);
  addEnterNotifyMaskW(ws);
}

void toggleLayoutA(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  togLayoutL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void increaseMasterLayoutA(ActionAr int_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  increaseMasterL(ws, int_arg.int_);
  addEnterNotifyMaskW(ws);
}

void resizeMasterLayoutA(ActionAr float_arg) {
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  resizeMasterL(ws, float_arg.float_);
  addEnterNotifyMaskW(ws);
}


// Workspace
void changeToWorkspaceA(ActionAr int_arg) {
  processWSActionA(changeToWorkspaceW, int_arg.int_);
}

void changeToPrevWorkspaceA(ActionAr no_arg) {
  (void)no_arg;
  processWSActionA(changeToWorkspaceW, getPrevStackSS());
}

void changeToNextWorkspaceA(ActionAr no_arg) {
  (void)no_arg;
  processWSActionA(changeToWorkspaceW, getNextStackSS());
}

void changeToLastWorkspaceA(ActionAr no_arg) {
  (void)no_arg;
  processWSActionA(changeToWorkspaceW, getLastStackSS());
}

void moveClientToWorkspaceA(ActionAr int_arg) {
  CliPtr c = getCurrClientCurrStackSS();
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  moveClientToWorkspaceW(c, int_arg.int_);
  addEnterNotifyMaskW(CLIVAL(c).ws);
}

void moveClientToWorkspaceAndFollowA(ActionAr int_arg) {
  CliPtr c = getCurrClientCurrStackSS();
  rmvEnterNotifyMaskW(CLIVAL(c).ws);
  moveClientToWorkspaceAndFollowW(c, int_arg.int_);
  addEnterNotifyMaskW(CLIVAL(c).ws);
}

void restoreLastMinimizedA(ActionAr no_arg) {
  (void)no_arg;
  processWSActionA(restoreLastMinimizedW, getCurrStackSS());
}

void toggleNSPA(ActionAr command_arg) {
  assert(command_arg.command_);
  const int ws = getCurrStackSS();
  rmvEnterNotifyMaskW(ws);
  CliPtr c = findNSPClientSS();
  if (c && CLIVAL(c).ws == ws) {
    moveClientToWorkspaceW(c, getNSPStackSS());
  } else if (c) {
    moveClientToWorkspaceW(c, ws);
  } else {
    if (!getSizeNSPSS())
      spawnS(command_arg.command_, NULL);
    else
      moveClientToWorkspaceW(getCurrClientNSPStackSS(), ws);
  }
  addEnterNotifyMaskW(ws);
}


// Curr Client
void moveFocusCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(moveFocusClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void swapCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(swapClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void killCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(killClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void tileCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(tileClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void normalCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(normalClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void fullScreenCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(fullScreenClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void toggleFullScreenCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(toggleFullScreenClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void minimizeCurrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  processCliActionA(minimizeClientW, getCurrClientCurrStackSS(), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freeCurrClientA(ActionAr freeLocFn_arg) {
  processCliActionA(freeClientW, getCurrClientCurrStackSS(), selfC, (const void *)&freeLocFn_arg.argfn_);
}

void toggleFreeCurrClientA(ActionAr freeLocFn_arg) {
  processCliActionA(toggleFreeClientW, getCurrClientCurrStackSS(), selfC, (const void *)&freeLocFn_arg.argfn_);
}


// Ptr Client
void moveFocusPtrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(moveFocusClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freeMovePtrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(freeMoveClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freeResizePtrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(freeResizeClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void movePtrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(moveClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void resizePtrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(resizeClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void toggleFullScreenPtrClientA(ActionAr selectCliFn_arg) {
  assert(selectCliFn_arg.argfn_.selectCliFn);
  int rx, ry;
  processCliActionA(toggleFullScreenClientW, getPtrClientW(&rx, &ry), selectCliFn_arg.argfn_.selectCliFn, NULL);
}

void freePtrClientA(ActionAr freeLocFn_arg) {
  int rx, ry;
  processCliActionA(freeClientW, getPtrClientW(&rx, &ry), selfC, (const void *)&freeLocFn_arg.argfn_);
}

void toggleFreePtrClientA(ActionAr freeLocFn_arg)  {
  int rx, ry;
  processCliActionA(toggleFreeClientW, getPtrClientW(&rx, &ry), selfC, (const void *)&freeLocFn_arg.argfn_);
}

