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
#include "core.h"
#include "client.h"
#include "workspace.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int isFreeSizeHints(Client *c) {
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

static Bool hasRule(const Client *c, const Rule *r) {
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

static void setRule(Client *c, const Rule *r) {
  assert(c);
  assert(r);
  if (r->workspace != currWSR)
    c->ws = r->workspace % NeuroCoreGetSize();
  c->isFullScreen = r->isFullScreen;
  c->freeSetterFn = r->freeSetterFn;
  c->fixPos = r->fixPos;
  Rectangle *reg = NeuroCoreStackGetRegion(c->ws);
  if (c->fixPos == upFixedR || c->fixPos == downFixedR)
    c->fixSize = r->fixSize * reg->h;
  else if (c->fixPos == leftFixedR || c->fixPos == rightFixedR)
    c->fixSize = r->fixSize * reg->w;
  if (r->goWorkspace)
    changeW(c->ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

Client *allocCliAndSetRulesR(Window w, const XWindowAttributes *wa) {
  assert(wa);
  Client *c = allocClientT(w, wa);
  if (!c)
    return NULL;
  if (isFreeSizeHints(c))
    c->freeSetterFn = defFreeR;
  c->ws = NeuroCoreGetCurrStack();
  NeuroClientUpdateClassAndName(&c, NULL);
  NeuroClientUpdateTitle(&c, NULL);
  const Rule *r;
  int i;
  for (i = 0; ruleSetS[ i ]; ++i) {
    r = ruleSetS[ i ];
    if (hasRule(c, r)) {
      setRule(c, r);
      break;
    }
  }
  if (!strcmp(c->name, WM_SCRATCHPAD_NAME))
    c->isNSP = True;
  return c;
}

void applyRuleR(const ClientPtrPtr c) {
  assert(c);
  Rectangle *reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  switch (CLI_GET(c).fixPos) {
    case upFixedR: {
      regc->x = reg->x;
      regc->y = reg->y;
      regc->w = reg->w;
      regc->h = CLI_GET(c).fixSize;
      if (CLI_GET(c).freeSetterFn != notFreeR)
        break;
      reg->y += CLI_GET(c).fixSize;
      reg->h -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case downFixedR: {
      regc->x = reg->x;
      regc->y = reg->h - CLI_GET(c).fixSize;
      regc->w = reg->w;
      regc->h = CLI_GET(c).fixSize;
      if (CLI_GET(c).freeSetterFn != notFreeR)
        break;
      reg->h -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case leftFixedR: {
      regc->x = reg->x;
      regc->y = reg->y;
      regc->w = CLI_GET(c).fixSize;
      regc->h = reg->h;
      if (CLI_GET(c).freeSetterFn != notFreeR)
        break;
      reg->x += CLI_GET(c).fixSize;
      reg->w -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case rightFixedR: {
      regc->x = reg->w - CLI_GET(c).fixSize;
      regc->y = reg->y;
      regc->w = CLI_GET(c).fixSize;
      regc->h = reg->h;
      if (CLI_GET(c).freeSetterFn != notFreeR)
        break;
      reg->w -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
  }
}

void unapplyRuleR(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return;
  Rectangle *reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  switch (CLI_GET(c).fixPos) {
    case upFixedR: {
      reg->y -= CLI_GET(c).fixSize;
      reg->h += CLI_GET(c).fixSize;
      break;
    }
    case downFixedR: {
      reg->h += CLI_GET(c).fixSize;
      break;
    }
    case leftFixedR: {
      reg->x -= CLI_GET(c).fixSize;
      reg->w += CLI_GET(c).fixSize;
      break;
    }
    case rightFixedR: {
      reg->w += CLI_GET(c).fixSize;
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
  NeuroGeometryCenterRectangleInRegion(a, r);
}

void bigCenterFreeR(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.05f, 0.05f, 0.9f, 0.9f };
  NeuroGeometryGetRelativeRectangle(a, r, size);
  NeuroGeometryCenterRectangleInRegion(a, r);
}

void scratchpadFreeR(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.00f, 0.00f, 1.00f, 0.75f };
  NeuroGeometryGetRelativeRectangle(a, r, size);
}

