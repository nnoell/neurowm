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
#include "config.h"
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
  if (!XGetWMNormalHints(NeuroSystemGetDisplay(), c->win, &size, &msize))
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
  c->ws = r->workspace_selector_fn() % NeuroCoreGetSize();
  c->is_fullscreen = r->is_fullscreen;
  c->free_setter_fn = r->free_setter_fn;
  c->fixed_pos = r->fixed_pos;
  Rectangle *reg = NeuroCoreStackGetRegion(c->ws);
  if (c->fixed_pos == RuleFixedPositionUp || c->fixed_pos == RuleFixedPositionDown)
    c->fixed_size = r->fixed_size * reg->h;
  else if (c->fixed_pos == RuleFixedPositionLeft || c->fixed_pos == RuleFixedPositionRigth)
    c->fixed_size = r->fixed_size * reg->w;
  if (r->follow)
    NeuroWorkspaceChange(c->ws);
}

static void apply_rules(Client *c) {
  assert(c);
  const Rule *const *const rule_set = NeuroConfigGet()->rule_set;
  if (!rule_set)
    return;
  const Rule *r;
  int i;
  for (i = 0; rule_set[ i ]; ++i) {
    r = rule_set[ i ];
    if (has_rule(c, r)) {
      set_rule(c, r);
      break;
    }
  }
  if (!strcmp(c->name, RULE_SCRATCHPAD_NAME))
    c->is_nsp = True;
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
    c->free_setter_fn = NeuroRuleFreeSetterDefault;
  c->ws = NeuroCoreGetCurrStack();
  NeuroClientUpdateClassAndName(&c, NULL);
  NeuroClientUpdateTitle(&c, NULL);
  apply_rules(c);
  return c;
}

void NeuroRuleApply(const ClientPtrPtr c) {
  assert(c);
  Rectangle *reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  Rectangle *regc = NeuroCoreClientGetRegion(c);
  if (CLI_GET(c).fixed_pos == RuleFixedPositionUp) {
    regc->x = reg->x;
    regc->y = reg->y;
    regc->w = reg->w;
    regc->h = CLI_GET(c).fixed_size;
    if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
      return;
    reg->y += CLI_GET(c).fixed_size;
    reg->h -= CLI_GET(c).fixed_size;
    XMoveResizeWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
  } else if (CLI_GET(c).fixed_pos == RuleFixedPositionDown) {
    regc->x = reg->x;
    regc->y = reg->h - CLI_GET(c).fixed_size;
    regc->w = reg->w;
    regc->h = CLI_GET(c).fixed_size;
    if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
      return;
    reg->h -= CLI_GET(c).fixed_size;
    XMoveResizeWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
  } else if (CLI_GET(c).fixed_pos == RuleFixedPositionLeft) {
    regc->x = reg->x;
    regc->y = reg->y;
    regc->w = CLI_GET(c).fixed_size;
    regc->h = reg->h;
    if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
      return;
    reg->x += CLI_GET(c).fixed_size;
    reg->w -= CLI_GET(c).fixed_size;
    XMoveResizeWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
  } else if (CLI_GET(c).fixed_pos ==  RuleFixedPositionRigth) {
    regc->x = reg->w - CLI_GET(c).fixed_size;
    regc->y = reg->y;
    regc->w = CLI_GET(c).fixed_size;
    regc->h = reg->h;
    if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
      return;
    reg->w -= CLI_GET(c).fixed_size;
    XMoveResizeWindow(NeuroSystemGetDisplay(), CLI_GET(c).win, regc->x, regc->y, regc->w, regc->h);
  }
}

void NeuroRuleUnapply(const ClientPtrPtr c) {
  assert(c);
  if (CLI_GET(c).free_setter_fn != NeuroRuleFreeSetterNull)
    return;
  Rectangle *reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);
  if (CLI_GET(c).fixed_pos == RuleFixedPositionUp) {
    reg->y -= CLI_GET(c).fixed_size;
    reg->h += CLI_GET(c).fixed_size;
  } else if (CLI_GET(c).fixed_pos == RuleFixedPositionDown) {
    reg->h += CLI_GET(c).fixed_size;
  } else if (CLI_GET(c).fixed_pos == RuleFixedPositionLeft) {
    reg->x -= CLI_GET(c).fixed_size;
    reg->w += CLI_GET(c).fixed_size;
  } else if (CLI_GET(c).fixed_pos == RuleFixedPositionRigth) {
    reg->w += CLI_GET(c).fixed_size;
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

