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

static int is_free_size_hints(Client *c) {
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

static Bool has_rule(const Client *c, const Rule *r) {
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

static void set_rule(Client *c, const Rule *r) {
  assert(c);
  assert(r);
  c->ws = r->workspaceSelectorFn() % NeuroCoreGetSize();
  c->isFullScreen = r->isFullScreen;
  c->freeSetterFn = r->freeSetterFn;
  c->fixPos = r->fixPos;
  Rectangle *reg = NeuroCoreStackGetRegion(c->ws);
  if (c->fixPos == NeuroRuleFixedPositionUp || c->fixPos == NeuroRuleFixedPositionDown)
    c->fixSize = r->fixSize * reg->h;
  else if (c->fixPos == NeuroRuleFixedPositionLeft || c->fixPos == NeuroRuleFixedPositionRigth)
    c->fixSize = r->fixSize * reg->w;
  if (r->goWorkspace)
    changeW(c->ws);
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

Client *NeuroRuleNewClient(Window w, const XWindowAttributes *wa) {
  assert(wa);
  Client *c = NeuroTypeNewClient(w, wa);
  if (!c)
    return NULL;
  if (is_free_size_hints(c))
    c->freeSetterFn = NeuroRuleFreeSetterDefault;
  c->ws = NeuroCoreGetCurrStack();
  NeuroClientUpdateClassAndName(&c, NULL);
  NeuroClientUpdateTitle(&c, NULL);
  const Rule *r;
  int i;
  for (i = 0; ruleSetS[ i ]; ++i) {
    r = ruleSetS[ i ];
    if (has_rule(c, r)) {
      set_rule(c, r);
      break;
    }
  }
  if (!strcmp(c->name, WM_SCRATCHPAD_NAME))
    c->isNSP = True;
  return c;
}

void NeuroRuleApply(const ClientPtrPtr c) {
  assert(c);
  Rectangle *reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  switch (CLI_GET(c).fixPos) {
    case NeuroRuleFixedPositionUp: {
      regc->x = reg->x;
      regc->y = reg->y;
      regc->w = reg->w;
      regc->h = CLI_GET(c).fixSize;
      if (CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull)
        break;
      reg->y += CLI_GET(c).fixSize;
      reg->h -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case NeuroRuleFixedPositionDown: {
      regc->x = reg->x;
      regc->y = reg->h - CLI_GET(c).fixSize;
      regc->w = reg->w;
      regc->h = CLI_GET(c).fixSize;
      if (CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull)
        break;
      reg->h -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case NeuroRuleFixedPositionLeft: {
      regc->x = reg->x;
      regc->y = reg->y;
      regc->w = CLI_GET(c).fixSize;
      regc->h = reg->h;
      if (CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull)
        break;
      reg->x += CLI_GET(c).fixSize;
      reg->w -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
    case NeuroRuleFixedPositionRigth: {
      regc->x = reg->w - CLI_GET(c).fixSize;
      regc->y = reg->y;
      regc->w = CLI_GET(c).fixSize;
      regc->h = reg->h;
      if (CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull)
        break;
      reg->w -= CLI_GET(c).fixSize;
      XMoveResizeWindow(display, CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
      break;
    }
  }
}

void NeuroRuleUnapply(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != NeuroRuleFreeSetterNull)
    return;
  Rectangle *reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  switch (CLI_GET(c).fixPos) {
    case NeuroRuleFixedPositionUp: {
      reg->y -= CLI_GET(c).fixSize;
      reg->h += CLI_GET(c).fixSize;
      break;
    }
    case NeuroRuleFixedPositionDown: {
      reg->h += CLI_GET(c).fixSize;
      break;
    }
    case NeuroRuleFixedPositionLeft: {
      reg->x -= CLI_GET(c).fixSize;
      reg->w += CLI_GET(c).fixSize;
      break;
    }
    case NeuroRuleFixedPositionRigth: {
      reg->w += CLI_GET(c).fixSize;
      break;
    }
  }
}

// Free Locations
void NeuroRuleFreeSetterDefault(Rectangle *a, const Rectangle *r) {
  (void)a;
  (void)r;
}

void NeuroRuleFreeSetterCenter(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  NeuroGeometryCenterRectangleInRegion(a, r);
}

void NeuroRuleFreeSetterBigCenter(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.05f, 0.05f, 0.9f, 0.9f };
  NeuroGeometryGetRelativeRectangle(a, r, size);
  NeuroGeometryCenterRectangleInRegion(a, r);
}

void NeuroRuleFreeSetterScratchpad(Rectangle *a, const Rectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.00f, 0.00f, 1.00f, 0.75f };
  NeuroGeometryGetRelativeRectangle(a, r, size);
}

