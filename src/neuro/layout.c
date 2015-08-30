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
#include "stackset.h"
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
  for (c=getHeadClientStackSS(ws); c; c=getNextClientSS(c)) {
    if (CLI_GET(c).freeSetterFn == notFreeR && !CLI_GET(c).fixPos && !CLI_GET(c).isHidden && !CLI_GET(c).isFullScreen) {
      if (i >= size || i <= 0) {  // Realloc if memory is needed
        size += STEP_SIZE_REALLOC;
        rs = (Rectangle **)realloc(rs, size*sizeof(void *));
        frs = (Rectangle **)realloc(frs, size*sizeof(void *));
        if (!rs || !frs)
          return NULL;
      }
      rs[ i ] = getRegionClientSS(c);
      frs[ i ] = &(CLI_GET(c).floatRegion);
      ++i;
    }
  }
  Arrange *a = (Arrange *)malloc(sizeof(Arrange));
  if (!a)
    return NULL;
  a->size = i;
  getRelativeRectangleG(&a->region, getRegionStackSS(ws), l->region);
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

static void getLengthSizes(int n, int length, int *xs, int *ws) {
  assert(xs);
  assert(ws);
  assert(n > 0);
  int a = (float)length / (float)n + 0.5f;
  int countx = 0;
  int i;
  for (i = 0; i < n; ++i) {
    xs[ i ] = countx;
    ws[ i ] = a;
    countx += a;
  }
  if ((ws[ n-1 ] + xs[ n-1 ]) != length)
    ws[ n-1 ] = length - xs[ n-1 ];
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

void runLayoutL(int ws, int i) {
  Layout *l = getLayoutStackSS(ws, i);
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

void runCurrLayoutL(int ws) {
  runLayoutL(ws, getLayoutIdxStackSS(ws));
}

void togModLayoutL(int ws, int i, unsigned int mod) {
  Layout *l = getLayoutStackSS(ws, i);
  l->mod ^= mod;
  runLayoutL(ws, i);
  updateW(ws);
}

void togModCurrLayoutL(int ws, unsigned int mod) {
  togModLayoutL(ws, getLayoutIdxStackSS(ws), mod);
}

void togLayoutL(int ws, int i) {
  int tl = i;
  if (isCurrTogLayoutStackSS(ws))
    tl = -1;
  setTogLayoutStackSS(ws, tl);
  runCurrLayoutL(ws);
  updateFocusW(ws);
}

void changeLayoutL(int ws, int s) {
  int val = getLayoutIdxStackSS(ws) + s;
  setLayoutStackSS(ws, val);
  runCurrLayoutL(ws);
  updateFocusW(ws);
}

void resetLayoutL(int ws) {
  Layout *l;
  const LayoutConf *lc;
  int i;
  for (i = 0; i < getNumLayoutStackSS(ws); ++i) {
    l = getLayoutStackSS(ws, i);
    lc = getLayoutConfStackSS(ws, i);
    l->mod = lc->mod;
    l->followMouse = lc->followMouse;
    memmove(l->arrangeSettings, lc->arrangeSettings, sizeof(GenericArg)*ARRSET_MAX);
  }
  tileW(ws);
  setLayoutStackSS(ws, 0);
  runCurrLayoutL(ws);
  updateFocusW(ws);
}

void increaseMasterL(int ws, int size) {
  GenericArg *as = getCurrLayoutStackSS(ws)->arrangeSettings;
  const int res = as[ 0 ].int_ + size;
  if (res < 1)
    return;
  *(int *)&(as[ 0 ].int_) = res;
  runCurrLayoutL(ws);
  updateFocusW(ws);
}

void resizeMasterL(int ws, float factor) {
  GenericArg *as = getCurrLayoutStackSS(ws)->arrangeSettings;
  const float newmsize = factor * as[ 2 ].float_ + as[ 1 ].float_;
  if (newmsize <= 0.0f || newmsize >= 1.0f)
    return;
  *(float *)&(as[ 1 ].float_) = newmsize;
  runCurrLayoutL(ws);
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
  getLengthSizes(nwindows, a->region.h, ys, hs);
  int i;
  for (i = 0; i < nwindows; ++i)  // Master area
    setRectangleG(a->cliRegions[ i ], a->region.x, a->region.y + ys[ i ] , n > mn ? ms : a->region.w, hs[ i ]);
  if (n - nwindows == 0)
    return a;
  getLengthSizes(n - nwindows, a->region.h, ys, hs);
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
  getLengthSizes(cols, a->region.w, xs, ws);
  int i, cn = 0, rn = 0;
  for (i = 0; i < n; ++i) {
    if (i/rows + 1 > cols - n%cols)
      rows = n/cols + 1;
    int ys[ rows ], hs[ rows ];
    memset(ys, 0, sizeof(ys));
    memset(hs, 0, sizeof(hs));
    getLengthSizes(rows, a->region.h, ys, hs);
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

