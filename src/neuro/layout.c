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

static Arrange *allocArrange(int ws, Layout *l) {
  assert(l);
  Rectangle **rs = NULL, **frs = NULL;
  int i = 0, size = 0;
  ClientPtrPtr c;
  for (c=NeuroCoreStackGetHeadClient(ws); c; c=NeuroCoreClientGetNext(c)) {
    if (CLI_GET(c).freeSetterFn == notFreeR && !CLI_GET(c).fixPos && !CLI_GET(c).isHidden && !CLI_GET(c).isFullScreen) {
      if (i >= size || i <= 0) {  // Realloc if memory is needed
        size += STEP_SIZE_REALLOC;
        rs = (Rectangle **)realloc(rs, size*sizeof(void *));
        frs = (Rectangle **)realloc(frs, size*sizeof(void *));
        if (!rs || !frs)
          return NULL;
      }
      rs[ i ] = NeuroCoreClientGetRegion(c);
      frs[ i ] = &(CLI_GET(c).floatRegion);
      ++i;
    }
  }
  Arrange *a = (Arrange *)malloc(sizeof(Arrange));
  if (!a)
    return NULL;
  a->size = i;
  getRelativeRectangleG(&a->region, NeuroCoreStackGetRegion(ws), l->region);
  a->cliRegions = rs;
  a->cliFloatRegions = frs;
  a->arrangeSettings = l->arrangeSettings;
  return a;
}

static void freeArrange(Arrange *a) {
  if (!a)
    return;
  free(a->cliRegions);
  a->cliRegions = NULL;
  free(a->cliFloatRegions);
  a->cliFloatRegions = NULL;
  free(a);
  a = NULL;
}

static void getBestPositionsAndSizes(int n, int total, int *positions, int *sizes) {
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
static Arrange *normalArrange(Arrange *a, ArrangerFn af) {
  assert(a);
  assert(af);
  af(a);
  return a;
}

static Arrange *mirrorArrange(Arrange *a, ArrangerFn af) {
  assert(a);
  assert(af);
  transpRectangleG(&a->region);
  af(a);
  int i;
  for (i = 0; i < a->size; ++i)
    transpRectangleG(a->cliRegions[ i ]);
  transpRectangleG(&a->region);
  return a;
}

// Mods
static Arrange *reflectXMod(Arrange *a) {
  assert(a);
  int i;
  for (i = 0; i < a->size; ++i)
    a->cliRegions[ i ]->x = 2*(a->region.x) + a->region.w - (a->cliRegions[ i ]->x + a->cliRegions[ i ]->w);
  return a;
}

static Arrange *reflectYMod(Arrange *a) {
  assert(a);
  int i;
  for (i = 0; i < a->size; ++i)
    a->cliRegions[ i ]->y = 2*(a->region.y) + a->region.h - (a->cliRegions[ i ]->y + a->cliRegions[ i ]->h);
  return a;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void runL(int ws, int i) {
  Layout *l = NeuroCoreStackGetLayout(ws, i);
  Arrange *a = allocArrange(ws, l);
  if (!a)
    exitErrorS("runLayoutL - could not run layout");
  if (a->size) {  // Then run layout
    if (l->mod & mirrModL)
      mirrorArrange(a, l->arrangerFn);
    else
      normalArrange(a, l->arrangerFn);
    if (l->mod & reflXModL)
      reflectXMod(a);
    if (l->mod & reflYModL)
      reflectYMod(a);
  }
  freeArrange(a);
}

void runCurrL(int ws) {
  runL(ws, NeuroCoreStackGetLayoutIdx(ws));
}

void toggleModL(int ws, int i, unsigned int mod) {
  Layout *l = NeuroCoreStackGetLayout(ws, i);
  l->mod ^= mod;
  runL(ws, i);
  updateW(ws);
}

void toggleModCurrL(int ws, unsigned int mod) {
  toggleModL(ws, NeuroCoreStackGetLayoutIdx(ws), mod);
}

void toggleL(int ws, int i) {
  int tl = i;
  if (NeuroCoreStackIsCurrToggledLayout(ws))
    tl = -1;
  NeuroCoreStackSetToggledLayout(ws, tl);
  runCurrL(ws);
  updateFocusW(ws);
}

void changeL(int ws, int s) {
  int val = NeuroCoreStackGetLayoutIdx(ws) + s;
  NeuroCoreStackSetLayoutIdx(ws, val);
  runCurrL(ws);
  updateFocusW(ws);
}

void resetL(int ws) {
  Layout *l;
  const LayoutConf *lc;
  int i;
  for (i = 0; i < NeuroCoreStackGetNumLayouts(ws); ++i) {
    l = NeuroCoreStackGetLayout(ws, i);
    lc = NeuroCoreStackGetLayoutConf(ws, i);
    l->mod = lc->mod;
    l->followMouse = lc->followMouse;
    memmove(l->arrangeSettings, lc->arrangeSettings, sizeof(GenericArg)*ARRSET_MAX);
  }
  tileW(ws);
  NeuroCoreStackSetLayoutIdx(ws, 0);
  runCurrL(ws);
  updateFocusW(ws);
}

void increaseMasterL(int ws, int size) {
  GenericArg *as = NeuroCoreStackGetCurrLayout(ws)->arrangeSettings;
  const int res = as[ 0 ].int_ + size;
  if (res < 1)
    return;
  *(int *)&(as[ 0 ].int_) = res;
  runCurrL(ws);
  updateFocusW(ws);
}

void resizeMasterL(int ws, float factor) {
  GenericArg *as = NeuroCoreStackGetCurrLayout(ws)->arrangeSettings;
  const float newmsize = factor * as[ 2 ].float_ + as[ 1 ].float_;
  if (newmsize <= 0.0f || newmsize >= 1.0f)
    return;
  *(float *)&(as[ 1 ].float_) = newmsize;
  runCurrL(ws);
  updateFocusW(ws);
}

// Layout Arrangers
Arrange *tallArrangerL(Arrange *a) {
  assert(a);
  int n = a->size;
  int mn = a->arrangeSettings[ 0 ].int_, ms = (int)(a->arrangeSettings[ 1 ].float_ * a->region.w);
  int nwindows = n <= mn ? n : mn;
  int ys[ n ], hs[ n ];
  memset(ys, 0, sizeof(ys));
  memset(hs, 0, sizeof(hs));
  getBestPositionsAndSizes(nwindows, a->region.h, ys, hs);
  int i;
  for (i = 0; i < nwindows; ++i)  // Master area
    setRectangleG(a->cliRegions[ i ], a->region.x, a->region.y + ys[ i ] , n > mn ? ms : a->region.w, hs[ i ]);
  if (n - nwindows == 0)
    return a;
  getBestPositionsAndSizes(n - nwindows, a->region.h, ys, hs);
  for (; i < n; ++i)  // Stacking area
    setRectangleG(a->cliRegions[ i ], a->region.x + ms, a->region.y + ys[ i-nwindows ], a->region.w - ms,
        hs[ i-nwindows ]);
  return a;
}

Arrange *gridArrangerL(Arrange *a) {
  assert(a);
  int n = a->size, cols, rows;
  for (cols = 0; cols <= n/2; ++cols)
    if (cols*cols >= n)
      break;
  rows = n/cols;
  int xs[ cols ], ws[ cols ];
  memset(xs, 0, sizeof(xs));
  memset(ws, 0, sizeof(ws));
  getBestPositionsAndSizes(cols, a->region.w, xs, ws);
  int i, cn = 0, rn = 0;
  for (i = 0; i < n; ++i) {
    if (i/rows + 1 > cols - n%cols)
      rows = n/cols + 1;
    int ys[ rows ], hs[ rows ];
    memset(ys, 0, sizeof(ys));
    memset(hs, 0, sizeof(hs));
    getBestPositionsAndSizes(rows, a->region.h, ys, hs);
    setRectangleG(a->cliRegions[ i ], a->region.x + xs[ cn ], a->region.y + ys[ rn ], ws[ cn ], hs[ rn ]);
    if (++rn >= rows) {
      rn = 0;
      ++cn;
    }
  }
  return a;
}

Arrange *fullArrangerL(Arrange *a) {
  assert(a);
  int i;
  for (i = 0; i < a->size; ++i)
    setRectangleG(a->cliRegions[ i ], a->region.x, a->region.y, a->region.w, a->region.h);
  return a;
}

Arrange *floatArrangerL(Arrange *a) {
  assert(a);
  Rectangle *fr;
  int i;
  for (i = 0; i < a->size; ++i) {
    fr = a->cliFloatRegions[ i ];
    setRectangleG(a->cliRegions[ i ], fr->x, fr->y, fr->w, fr->h);
    fitRectangleInRegionG(a->cliRegions[ i ], &a->region);
  }
  return a;
}

