//----------------------------------------------------------------------------------------------------------------------
// Module      :  geometry
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "geometry.h"


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

Rectangle *getRelativeRectangleG(Rectangle *dst, const Rectangle *src, const float *rel) {
  assert(dst);
  assert(src);
  assert(rel);
  dst->x = src->x + (int)((float)src->w * rel[ 0 ]);
  dst->y = src->y + (int)((float)src->h * rel[ 1 ]);
  dst->w = (int)((float)src->w * rel[ 2 ]);
  dst->h = (int)((float)src->h * rel[ 3 ]);
  return dst;
}

Rectangle *getAbsoluteRelativeRectangleG(Rectangle *dst, const Rectangle *src, const int *gaps) {
  assert(dst);
  assert(src);
  assert(gaps);
  dst->x = src->x + gaps[ 2 ];
  dst->y = src->y + gaps[ 0 ];
  dst->w = src->w - (gaps[ 3 ] + gaps[ 2 ]);
  dst->h = src->h - (gaps[ 1 ] + gaps[ 0 ]);
  return dst;
}

Rectangle *setRectangleG(Rectangle *r, int x, int y, int w, int h) {
  assert(r);
  r->x = x; r->y = y; r->w = w; r->h = h;
  return r;
}

Rectangle *setRectangleBorderWidthG(Rectangle *r, int width) {
  assert(r);
  r->w -= width * 2;
  r->h -= width * 2;
  return r;
}

Rectangle *setRectangleBorderGapG(Rectangle *r, int gap) {
  assert(r);
  r->x += gap;
  r->y += gap;
  r->w -= gap * 2;
  r->h -= gap * 2;
  return r;
}

Rectangle *setRectangleBorderWidthAndGapG(Rectangle *r, int width, int gap) {
  assert(r);
  return setRectangleBorderGapG(setRectangleBorderWidthG(r, width), gap);
}

Rectangle *mirrorRectangleG(Rectangle *rect, const Rectangle *reg) {
  assert(rect);
  assert(reg);
  const int oldx = rect->x, oldy = rect->y, oldw = rect->w, oldh = rect->h;
  rect->y = (int)(((float)oldx-reg->x) / ((float)reg->w) * ((float)reg->h) + 0.5f) + reg->y;
  rect->x = (int)(((float)oldy-reg->y) / ((float)reg->h) * ((float)reg->w) + 0.5f) + reg->x;
  rect->h = (int)(((float)oldw) / ((float)reg->w) * ((float)reg->h) + 0.5f);
  rect->w = (int)(((float)oldh) / ((float)reg->h) * ((float)reg->w) + 0.5f);
  return rect;
}

Rectangle *transpRectangleG(Rectangle *rect) {
  int tmp = rect->x;
  rect->x = rect->y;
  rect->y = tmp;
  tmp = rect->w;
  rect->w = rect->h;
  rect->h = tmp;
  return rect;
}

Rectangle *fitRectangleInRegionG(Rectangle *rect, const Rectangle *reg) {
  assert(rect);
  assert(reg);
  if (rect->x < reg->x)
    rect->x = reg->x;
  if (rect->y < reg->y)
    rect->y = reg->y;
  if (rect->w > reg->w)
    rect->w = reg->w;
  if (rect->h > reg->h)
    rect->h = reg->h;
  if (rect->x + rect->w > reg->w)
    rect->x = reg->x + reg->w - rect->w;
  if (rect->y + rect->h > reg->h)
    rect->y = reg->y + reg->h - rect->h;
  return rect;
}

Rectangle *centerRectangleInRegionG(Rectangle *rect, const Rectangle *reg) {
  assert(rect);
  assert(reg);
  rect->x = reg->x + (reg->w - rect->w) / 2;
  rect->y = reg->y + (reg->h - rect->h) / 2;
  return rect;
}

Bool isPointInRectangleG(const Rectangle *a, int x, int y) {
  assert(a);
  if (x < a->x)
    return False;
  if (y < a->y)
    return False;
  if (x > a->x + a->w)
    return False;
  if (y > a->y + a->h)
    return False;
  return True;
}

