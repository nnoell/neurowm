//----------------------------------------------------------------------------------------------------------------------
// Module      :  rule
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "rule.h"
#include "geometry.h"
#include "system.h"
#include "stackset.h"
#include "client.h"
#include "workspace.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int isFreeSizeHintsR(Client *c) {
  assert(c);
  int maxw = 0, maxh = 0, minw = 0, minh = 0;
  long msize;
  XSizeHints size;
  if (!XGetWMNormalHints(display, c->win, &size, &msize))
    size.flags = PSize;
  if (size.flags & PMaxSize) {
    maxw = size.max_width;
    maxh = size.max_height;
  }
  if (size.flags & PMinSize) {
    minw = size.min_width;
    minh = size.min_height;
  } else if (size.flags & PBaseSize) {
    minw = size.base_width;
    minh = size.base_height;
  }
  return maxw && minw && maxh && minh && maxw == minw && maxh == minh;
}

static Bool hasRuleR(const Client *c, const Rule *r) {
  assert(c);
  assert(r);
  if (!c)
    return False;
  if (!r->class && !r->name && !r->title)
    return False;
  Bool res_class = True, res_name = True, res_title = True;
  if (r->class)
    res_class = strcmp(c->class, r->class) == 0;
  if (r->name)
    res_name = strcmp(c->name, r->name) == 0;
  if (r->title)
    res_title = strcmp(c->title, r->title) == 0;
  return res_class && res_name && res_title;
}

static void setRuleR(Client *c, const Rule *r) {
  assert(c);
  assert(r);
  if (r->workspace != currWSR)
    c->ws = r->workspace % getSizeSS();
  c->isFullScreen = r->isFullScreen;
  c->freeLocFn = r->freeLocFn;
  c->fixPos = r->fixPos;
  Rectangle *reg = getRegionStackSS(c->ws);
  if (c->fixPos == upFixedR || c->fixPos == downFixedR)
    c->fixSize = r->fixSize * reg->h;
  else if (c->fixPos == leftFixedR || c->fixPos == rightFixedR)
    c->fixSize = r->fixSize * reg->w;
  if (r->goWorkspace)
    changeToWorkspaceW(c->ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

Client *allocCliAndSetRulesR(Window w, const XWindowAttributes *wa) {
  assert(wa);
  Client *c = allocClientT(w, wa);
  if (!c)
    return NULL;
  if (isFreeSizeHintsR(c))
    c->freeLocFn = defFreeR;
  c->ws = getCurrStackSS();
  updateClassAndNameC(&c, NULL);
  updateTitleC(&c, NULL);
  const Rule *r;
  int i;
  for (i = 0; ruleSetS[ i ]; ++i) {
    r = ruleSetS[ i ];
    if (hasRuleR(c, r)) {
      setRuleR(c, r);
      break;
    }
  }
  if (!strcmp(c->name, WM_SCRATCHPAD_NAME))
    c->isNSP = True;
  return c;
}

void applyRuleR(const CliPtr c) {
  assert(c);
  Rectangle *reg = getRegionStackSS(CLIVAL(c).ws);
  Rectangle *regc = getRegionClientSS(c);
  switch (CLIVAL(c).fixPos) {
    case upFixedR: {
      regc->x = reg->x;
      regc->y = reg->y;
      regc->w = reg->w;
      regc->h = CLIVAL(c).fixSize;
      if (CLIVAL(c).freeLocFn != notFreeR)
        break;
      reg->y += CLIVAL(c).fixSize;
      reg->h -= CLIVAL(c).fixSize;
      XMoveResizeWindow(display, CLIVAL(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case downFixedR: {
      regc->x = reg->x;
      regc->y = reg->h - CLIVAL(c).fixSize;
      regc->w = reg->w;
      regc->h = CLIVAL(c).fixSize;
      if (CLIVAL(c).freeLocFn != notFreeR)
        break;
      reg->h -= CLIVAL(c).fixSize;
      XMoveResizeWindow(display, CLIVAL(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case leftFixedR: {
      regc->x = reg->x;
      regc->y = reg->y;
      regc->w = CLIVAL(c).fixSize;
      regc->h = reg->h;
      if (CLIVAL(c).freeLocFn != notFreeR)
        break;
      reg->x += CLIVAL(c).fixSize;
      reg->w -= CLIVAL(c).fixSize;
      XMoveResizeWindow(display, CLIVAL(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case rightFixedR: {
      regc->x = reg->w - CLIVAL(c).fixSize;
      regc->y = reg->y;
      regc->w = CLIVAL(c).fixSize;
      regc->h = reg->h;
      if (CLIVAL(c).freeLocFn != notFreeR)
        break;
      reg->w -= CLIVAL(c).fixSize;
      XMoveResizeWindow(display, CLIVAL(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
  }
}

void unapplyRuleR(const CliPtr c) {
  assert(c);
  if (CLIVAL(c).freeLocFn != notFreeR)
    return;
  Rectangle *reg = getRegionStackSS(CLIVAL(c).ws);
  switch (CLIVAL(c).fixPos) {
    case upFixedR: {
      reg->y -= CLIVAL(c).fixSize;
      reg->h += CLIVAL(c).fixSize;
      break;
    }
    case downFixedR: {
      reg->h += CLIVAL(c).fixSize;
      break;
    }
    case leftFixedR: {
      reg->x -= CLIVAL(c).fixSize;
      reg->w += CLIVAL(c).fixSize;
      break;
    }
    case rightFixedR: {
      reg->w += CLIVAL(c).fixSize;
      break;
    }
  }
}

// Free Locations
void defFreeR(Rectangle *a, const Rectangle *r) {
  (void)a;
  (void)r;
}

void centerFreeR(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  centerRectangleInRegionG(a, r);
}

void bigCenterFreeR(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.05f, 0.05f, 0.9f, 0.9f };
  getRelativeRectangleG(a, r, size);
  centerRectangleInRegionG(a, r);
}

void scratchpadFreeR(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.00f, 0.00f, 1.00f, 0.75f };
  getRelativeRectangleG(a, r, size);
}

