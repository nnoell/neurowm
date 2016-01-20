//----------------------------------------------------------------------------------------------------------------------
// Module      :  layout
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "system.h"
#include "layout.h"
#include "geometry.h"
#include "core.h"
#include "workspace.h"
#include "rule.h"

// Defines
#define STEP_SIZE_REALLOC 32


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Arrange *new_arrange(int ws, Layout *l) {
  assert(l);
  Rectangle **rs = NULL, **frs = NULL;
  int i = 0, size = 0;
  ClientPtrPtr c;
  for (c=NeuroCoreStackGetHeadClient(ws); c; c=NeuroCoreClientGetNext(c)) {
    if (CLI_GET(c).free_setter_fn == NeuroRuleFreeSetterNull && !CLI_GET(c).fixed_pos && !CLI_GET(c).is_hidden &&
        !CLI_GET(c).is_fullscreen) {
      if (i >= size || i <= 0) {  // Realloc if memory is needed
        size += STEP_SIZE_REALLOC;
        rs = (Rectangle **)realloc(rs, size*sizeof(void *));
        frs = (Rectangle **)realloc(frs, size*sizeof(void *));
        if (!rs || !frs)
          return NULL;
      }
      rs[ i ] = NeuroCoreClientGetRegion(c);
      frs[ i ] = &(CLI_GET(c).float_region);
      ++i;
    }
  }
  Arrange *a = (Arrange *)malloc(sizeof(Arrange));
  if (!a)
    return NULL;
  a->size = i;
  NeuroGeometryGetRelativeRectangle(&a->region, NeuroCoreStackGetRegion(ws), l->region);
  a->client_regions = rs;
  a->client_float_regions = frs;
  a->arrange_settings = l->arrange_settings;
  return a;
}

static void delete_arrange(Arrange *a) {
  if (!a)
    return;
  free(a->client_regions);
  a->client_regions = NULL;
  free(a->client_float_regions);
  a->client_float_regions = NULL;
  free(a);
  a = NULL;
}

static void get_best_positions_and_sizes(int n, int total, int *positions, int *sizes) {
  assert(positions);
  assert(sizes);
  assert(n > 0);
  int position = 0, size, i;
  for (i = 0; i < n; ++i) {
    size = total / (n - i);
    positions[ i ] = position;
    sizes[ i ] = size;
    position += size;
    total -= size;
  }
}

// Arrange runners
static Arrange *normal_arrange(Arrange *a, ArrangerFn af) {
  assert(a);
  assert(af);
  af(a);
  return a;
}

static Arrange *mirror_arrange(Arrange *a, ArrangerFn af) {
  assert(a);
  assert(af);
  NeuroGeometryTranspRectangle(&a->region);
  af(a);
  int i;
  for (i = 0; i < a->size; ++i)
    NeuroGeometryTranspRectangle(a->client_regions[ i ]);
  NeuroGeometryTranspRectangle(&a->region);
  return a;
}

// Mods
static Arrange *reflect_x_mod(Arrange *a) {
  assert(a);
  int i;
  for (i = 0; i < a->size; ++i)
    a->client_regions[ i ]->x = 2*(a->region.x) + a->region.w - (a->client_regions[ i ]->x + a->client_regions[ i ]->w);
  return a;
}

static Arrange *reflect_y_mod(Arrange *a) {
  assert(a);
  int i;
  for (i = 0; i < a->size; ++i)
    a->client_regions[ i ]->y = 2*(a->region.y) + a->region.h - (a->client_regions[ i ]->y + a->client_regions[ i ]->h);
  return a;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void NeuroLayoutRun(int ws, int i) {
  Layout *l = NeuroCoreStackGetLayout(ws, i);
  Arrange *a = new_arrange(ws, l);
  if (!a)
    NeuroSystemError("NeuroLayoutRun - Could not run layout");
  if (a->size) {  // Then run layout
    if (l->mod & LayoutModMirror)
      mirror_arrange(a, l->arranger_fn);
    else
      normal_arrange(a, l->arranger_fn);
    if (l->mod & LayoutModReflectX)
      reflect_x_mod(a);
    if (l->mod & LayoutModReflectY)
      reflect_y_mod(a);
  }
  delete_arrange(a);
}

void NeuroLayoutRunCurr(int ws) {
  NeuroLayoutRun(ws, NeuroCoreStackGetLayoutIdx(ws));
}

void NeuroLayoutToggleMod(int ws, int i, unsigned int mod) {
  Layout *l = NeuroCoreStackGetLayout(ws, i);
  l->mod ^= mod;
  NeuroLayoutRun(ws, i);
  NeuroWorkspaceUpdate(ws);
}

void NeuroLayoutToggleModCurr(int ws, unsigned int mod) {
  NeuroLayoutToggleMod(ws, NeuroCoreStackGetLayoutIdx(ws), mod);
}

void NeuroLayoutToggle(int ws, int i) {
  if (NeuroCoreStackIsCurrToggledLayout(ws))
    NeuroCoreStackSetToggledLayout(ws, NULL);
  else
    NeuroCoreStackSetToggledLayout(ws, &i);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroLayoutChange(int ws, int s) {
  int val = NeuroCoreStackGetLayoutIdx(ws) + s;
  NeuroCoreStackSetLayoutIdx(ws, val);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroLayoutReset(int ws) {
  Layout *l;
  const LayoutConf *lc;
  int i;
  for (i = 0; i < NeuroCoreStackGetNumLayouts(ws); ++i) {
    l = NeuroCoreStackGetLayout(ws, i);
    lc = NeuroCoreStackGetLayoutConf(ws, i);
    l->mod = lc->mod;
    l->follow_mouse = lc->follow_mouse;
    memmove(l->arrange_settings, lc->arrange_settings, sizeof(GenericArg)*ARRSET_MAX);
  }
  NeuroWorkspaceTile(ws);
  NeuroCoreStackSetLayoutIdx(ws, 0);
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroLayoutIncreaseMaster(int ws, int size) {
  GenericArg *as = NeuroCoreStackGetCurrLayout(ws)->arrange_settings;
  const int res = as[ 0 ].int_ + size;
  if (res < 1)
    return;
  *(int *)&(as[ 0 ].int_) = res;
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

void NeuroLayoutResizeMaster(int ws, float factor) {
  GenericArg *as = NeuroCoreStackGetCurrLayout(ws)->arrange_settings;
  const float newmsize = factor * as[ 2 ].float_ + as[ 1 ].float_;
  if (newmsize <= 0.0f || newmsize >= 1.0f)
    return;
  *(float *)&(as[ 1 ].float_) = newmsize;
  NeuroLayoutRunCurr(ws);
  NeuroWorkspaceFocus(ws);
}

// Layout Arrangers
Arrange *NeuroLayoutArrangerTall(Arrange *a) {
  assert(a);
  int n = a->size;
  int mn = a->arrange_settings[ 0 ].int_, ms = (int)(a->arrange_settings[ 1 ].float_ * a->region.w);
  int nwindows = n <= mn ? n : mn;
  int ys[ n ], hs[ n ];
  memset(ys, 0, sizeof(ys));
  memset(hs, 0, sizeof(hs));
  get_best_positions_and_sizes(nwindows, a->region.h, ys, hs);
  int i;
  for (i = 0; i < nwindows; ++i)  // Master area
    NeuroGeometrySetRectangle(a->client_regions[ i ], a->region.x, a->region.y + ys[ i ],
        n > mn ? ms : a->region.w, hs[ i ]);
  if (n - nwindows == 0)
    return a;
  get_best_positions_and_sizes(n - nwindows, a->region.h, ys, hs);
  for (; i < n; ++i)  // Stacking area
    NeuroGeometrySetRectangle(a->client_regions[ i ], a->region.x + ms, a->region.y + ys[ i-nwindows ],
        a->region.w - ms, hs[ i-nwindows ]);
  return a;
}

Arrange *NeuroLayoutArrangerGrid(Arrange *a) {
  assert(a);
  int n = a->size, cols, rows;
  for (cols = 0; cols <= n/2; ++cols)
    if (cols*cols >= n)
      break;
  rows = n/cols;
  int xs[ cols ], ws[ cols ];
  memset(xs, 0, sizeof(xs));
  memset(ws, 0, sizeof(ws));
  get_best_positions_and_sizes(cols, a->region.w, xs, ws);
  int i, cn = 0, rn = 0;
  for (i = 0; i < n; ++i) {
    if (i/rows + 1 > cols - n%cols)
      rows = n/cols + 1;
    int ys[ rows ], hs[ rows ];
    memset(ys, 0, sizeof(ys));
    memset(hs, 0, sizeof(hs));
    get_best_positions_and_sizes(rows, a->region.h, ys, hs);
    NeuroGeometrySetRectangle(a->client_regions[ i ], a->region.x + xs[ cn ], a->region.y + ys[ rn ], ws[ cn ],
        hs[ rn ]);
    if (++rn >= rows) {
      rn = 0;
      ++cn;
    }
  }
  return a;
}

Arrange *NeuroLayoutArrangerFull(Arrange *a) {
  assert(a);
  int i;
  for (i = 0; i < a->size; ++i)
    NeuroGeometrySetRectangle(a->client_regions[ i ], a->region.x, a->region.y, a->region.w, a->region.h);
  return a;
}

Arrange *NeuroLayoutArrangerFloat(Arrange *a) {
  assert(a);
  Rectangle *fr;
  int i;
  for (i = 0; i < a->size; ++i) {
    fr = a->client_float_regions[ i ];
    NeuroGeometrySetRectangle(a->client_regions[ i ], fr->x, fr->y, fr->w, fr->h);
    NeuroGeometryNeuroGeometryTranspRectangle(a->client_regions[ i ], &a->region);
  }
  return a;
}

