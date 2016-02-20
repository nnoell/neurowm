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

static bool is_free_size_hints(NeuroClient *c) {
  assert(c);
  int maxw = 0, maxh = 0, minw = 0, minh = 0;
  long msize = 0L;
  XSizeHints size = { 0 };
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

static bool has_rule(const NeuroClient *c, const NeuroRule *r) {
  assert(c);
  assert(r);
  if (!c)
    return false;
  if (!r->class && !r->name && !r->title)
    return false;
  bool res_class = true, res_name = true, res_title = true;
  if (r->class)
    res_class = strcmp(c->class, r->class) == 0;
  if (r->name)
    res_name = strcmp(c->name, r->name) == 0;
  if (r->title)
    res_title = strcmp(c->title, r->title) == 0;
  return res_class && res_name && res_title;
}

static void set_rule(NeuroClient *c, const NeuroRule *r) {
  assert(c);
  assert(r);
  c->ws = r->workspace_selector_fn() % NeuroCoreGetSize();
  c->is_fullscreen = r->is_fullscreen;
  c->free_setter_fn = r->free_setter_fn;
  c->fixed_pos = r->fixed_pos;
  c->fixed_size = r->fixed_size;
  if (r->follow)
    NeuroWorkspaceChange(c->ws);
}

static void apply_rules(NeuroClient *c) {
  assert(c);
  const NeuroRule *const *const rule_list = NeuroConfigGet()->rule_list;
  if (!rule_list)
    return;
  const NeuroRule *r;
  for (size_t i = 0U; rule_list[ i ]; ++i) {
    r = rule_list[ i ];
    if (has_rule(c, r)) {
      set_rule(c, r);
      break;
    }
  }
  if (!strcmp(c->name, RULE_SCRATCHPAD_NAME))
    c->is_nsp = true;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

NeuroClient *NeuroRuleNewClient(Window w, const XWindowAttributes *wa) {
  assert(wa);
  NeuroClient *c = NeuroTypeNewClient(w, wa);
  if (!c)
    return NULL;
  if (is_free_size_hints(c))
    c->free_setter_fn = NeuroRuleFreeSetterCenter;
  c->ws = NeuroCoreGetCurrStack();
  NeuroClientUpdateClassAndName(&c, NULL);
  NeuroClientUpdateTitle(&c, NULL);
  apply_rules(c);
  return c;
}

void NeuroRuleSetLayoutRegion(NeuroRectangle *r, const NeuroClientPtrPtr c) {
  if (!r || !c || CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_NULL)
    return;

  // Update the layout region
  if (CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_UP) {
    r->y += CLI_GET(c).fixed_size * r->h;
    r->h -= CLI_GET(c).fixed_size * r->h;
  } else if (CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_DOWN) {
    r->h -= CLI_GET(c).fixed_size * r->h;
  } else if (CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_LEFT) {
    r->x += CLI_GET(c).fixed_size * r->w;
    r->w -= CLI_GET(c).fixed_size * r->w;
  } else if (CLI_GET(c).fixed_pos ==  NEURO_FIXED_POSITION_RIGHT) {
    r->w -= CLI_GET(c).fixed_size * r->w;
  }
}

void NeuroRuleSetClientRegion(NeuroRectangle *r, const NeuroClientPtrPtr c) {
  if (!c || CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_NULL)
    return;

  // Get the stack region
  NeuroRectangle *const stack_reg = NeuroCoreStackGetRegion(CLI_GET(c).ws);

  // Update the client region
  if (CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_UP) {
    r->x = stack_reg->x;
    r->y = stack_reg->y;
    r->w = stack_reg->w;
    r->h = CLI_GET(c).fixed_size * stack_reg->h;
  } else if (CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_DOWN) {
    r->x = stack_reg->x;
    r->y = stack_reg->h - CLI_GET(c).fixed_size;
    r->w = stack_reg->w;
    r->h = CLI_GET(c).fixed_size * stack_reg->h;
  } else if (CLI_GET(c).fixed_pos == NEURO_FIXED_POSITION_LEFT) {
    r->x = stack_reg->x;
    r->y = stack_reg->y;
    r->w = CLI_GET(c).fixed_size * stack_reg->w;
    r->h = stack_reg->h;
  } else if (CLI_GET(c).fixed_pos ==  NEURO_FIXED_POSITION_RIGHT) {
    r->x = stack_reg->w - CLI_GET(c).fixed_size;
    r->y = stack_reg->y;
    r->w = CLI_GET(c).fixed_size * stack_reg->w;
    r->h = stack_reg->h;
  }
}

// Free Locations
void NeuroRuleFreeSetterDefault(NeuroRectangle *a, const NeuroRectangle *r) {
  (void)a;
  (void)r;
}

void NeuroRuleFreeSetterCenter(NeuroRectangle *a, const NeuroRectangle *r) {
  assert(a);
  assert(r);
  NeuroGeometryCenterRectangleInRegion(a, r);
}

void NeuroRuleFreeSetterBigCenter(NeuroRectangle *a, const NeuroRectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.05f, 0.05f, 0.9f, 0.9f };
  NeuroGeometryGetRelativeRectangle(a, r, size);
  NeuroGeometryCenterRectangleInRegion(a, r);
}

void NeuroRuleFreeSetterScratchpad(NeuroRectangle *a, const NeuroRectangle *r) {
  assert(a);
  assert(r);
  float size[ 4 ] = { 0.00f, 0.00f, 1.00f, 0.75f };
  NeuroGeometryGetRelativeRectangle(a, r, size);
}

