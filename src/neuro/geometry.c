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

// Point Generators
NeuroPoint *NeuroGeometryGetUpperPoint(NeuroPoint *dst, const NeuroPoint *src, int distance) {
  assert(dst);
  assert(src);
  memmove(dst, src, sizeof(NeuroPoint));
  dst->y = src->y - distance;
  return dst;
}

NeuroPoint *NeuroGeometryGetLowerPoint(NeuroPoint *dst, const NeuroPoint *src, int distance) {
  assert(dst);
  assert(src);
  memmove(dst, src, sizeof(NeuroPoint));
  dst->y = src->y + distance;
  return dst;
}

NeuroPoint *NeuroGeometryGetLeftPoint(NeuroPoint *dst, const NeuroPoint *src, int distance) {
  assert(dst);
  assert(src);
  memmove(dst, src, sizeof(NeuroPoint));
  dst->x = src->x - distance;
  return dst;
}

NeuroPoint *NeuroGeometryGetRightPoint(NeuroPoint *dst, const NeuroPoint *src, int distance) {
  assert(dst);
  assert(src);
  memmove(dst, src, sizeof(NeuroPoint));
  dst->x = src->x + distance;
  return dst;
}

// Rectangle Generators
NeuroRectangle *NeuroGeometryGetRelativeRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const float *rel) {
  assert(dst);
  assert(src);
  assert(rel);
  dst->p.x = src->p.x + (int)((float)src->w * rel[ 0 ]);
  dst->p.y = src->p.y + (int)((float)src->h * rel[ 1 ]);
  dst->w = (int)((float)src->w * rel[ 2 ]);
  dst->h = (int)((float)src->h * rel[ 3 ]);
  return dst;
}

NeuroRectangle *NeuroGeometryGetReducedRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps) {
  assert(dst);
  assert(src);
  assert(gaps);
  dst->p.x = src->p.x + (gaps[ 2 ]);
  dst->p.y = src->p.y + (gaps[ 0 ]);
  dst->w = src->w - (gaps[ 2 ] + gaps[ 3 ]);
  dst->h = src->h - (gaps[ 0 ] + gaps[ 1 ]);
  return dst;
}

NeuroRectangle *NeuroGeometryGetIncreasedRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps) {
  assert(dst);
  assert(src);
  assert(gaps);
  dst->p.x = src->p.x - (gaps[ 2 ]);
  dst->p.y = src->p.y - (gaps[ 0 ]);
  dst->w = src->w + (gaps[ 2 ] + gaps[ 3 ]);
  dst->h = src->h + (gaps[ 0 ] + gaps[ 1 ]);
  return dst;
}

// Rectangle Updaters
NeuroRectangle *NeuroGeometrySetRectangleBorderWidth(NeuroRectangle *r, int width) {
  assert(r);
  r->w -= width * 2;
  r->h -= width * 2;
  return r;
}

NeuroRectangle *NeuroGeometrySetRectangleBorderGap(NeuroRectangle *r, int gap) {
  assert(r);
  r->p.x += gap;
  r->p.y += gap;
  r->w -= gap * 2;
  r->h -= gap * 2;
  return r;
}

NeuroRectangle *NeuroGeometrySetRectangleBorderWidthAndGap(NeuroRectangle *r, int width, int gap) {
  assert(r);
  return NeuroGeometrySetRectangleBorderGap(NeuroGeometrySetRectangleBorderWidth(r, width), gap);
}

NeuroRectangle *NeuroGeometryMirrorRectangle(NeuroRectangle *r, const NeuroRectangle *reg) {
  assert(r);
  assert(reg);
  const int oldx = r->p.x, oldy = r->p.y, oldw = r->w, oldh = r->h;
  r->p.y = (int)(((float)oldx-reg->p.x) / ((float)reg->w) * ((float)reg->h) + 0.5f) + reg->p.y;
  r->p.x = (int)(((float)oldy-reg->p.y) / ((float)reg->h) * ((float)reg->w) + 0.5f) + reg->p.x;
  r->h = (int)(((float)oldw) / ((float)reg->w) * ((float)reg->h) + 0.5f);
  r->w = (int)(((float)oldh) / ((float)reg->h) * ((float)reg->w) + 0.5f);
  return r;
}

NeuroRectangle *NeuroGeometryTranspRectangle(NeuroRectangle *r) {
  assert(r);
  int tmp = r->p.x;
  r->p.x = r->p.y;
  r->p.y = tmp;
  tmp = r->w;
  r->w = r->h;
  r->h = tmp;
  return r;
}

NeuroRectangle *NeuroGeometryFitRectangleInRegion(NeuroRectangle *r, const NeuroRectangle *reg) {
  assert(r);
  assert(reg);
  if (r->p.x < reg->p.x)
    r->p.x = reg->p.x;
  if (r->p.y < reg->p.y)
    r->p.y = reg->p.y;
  if (r->w > reg->w)
    r->w = reg->w;
  if (r->h > reg->h)
    r->h = reg->h;
  if (r->p.x + r->w > reg->w)
    r->p.x = reg->p.x + reg->w - r->w;
  if (r->p.y + r->h > reg->h)
    r->p.y = reg->p.y + reg->h - r->h;
  return r;
}

NeuroRectangle *NeuroGeometryCenterRectangleInRegion(NeuroRectangle *r, const NeuroRectangle *reg) {
  assert(r);
  assert(reg);
  r->p.x = reg->p.x + (reg->w - r->w) / 2;
  r->p.y = reg->p.y + (reg->h - r->h) / 2;
  return r;
}

// Point-Rectangle Testers
bool NeuroGeometryIsPointInRectangle(const NeuroRectangle *r, const NeuroPoint *p) {
  assert(r);
  if (p->x < r->p.x)
    return false;
  if (p->y < r->p.y)
    return false;
  if (p->x > r->p.x + r->w)
    return false;
  if (p->y > r->p.y + r->h)
    return false;
  return true;
}

