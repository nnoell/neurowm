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

#ifndef NEURO_GEOMETRY_H_
#define NEURO_GEOMETRY_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

Rectangle *getRelativeRectangleG(Rectangle *dst, const Rectangle *src, const float *rel);
Rectangle *getAbsoluteRelativeRectangleG(Rectangle *dst, const Rectangle *src, const int *gaps);
Rectangle *setRectangleG(Rectangle *r, int x, int y, int w, int h);
Rectangle *setRectangleBorderWidthG(Rectangle *r, int width);
Rectangle *setRectangleBorderGapG(Rectangle *r, int gap);
Rectangle *setRectangleBorderWidthAndGapG(Rectangle *r, int width, int gap);
Rectangle *mirrorRectangleG(Rectangle *rect, const Rectangle *reg);
Rectangle *transpRectangleG(Rectangle *rect);
Rectangle *fitRectangleInRegionG(Rectangle *rect, const Rectangle *reg);
Rectangle *centerRectangleInRegionG(Rectangle *rect, const Rectangle *reg);
Bool isPointInRectangleG(const Rectangle *a, int x, int y);

#endif  // NEURO_GEOMETRY_H_

