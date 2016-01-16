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

#pragma once

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

Rectangle *NeuroGeometryGetRelativeRectangle(Rectangle *dst, const Rectangle *src, const float *rel);
Rectangle *NeuroGeometryGetAbsoluteRelativeRectangle(Rectangle *dst, const Rectangle *src, const int *gaps);
Rectangle *NeuroGeometrySetRectangle(Rectangle *r, int x, int y, int w, int h);
Rectangle *NeuroGeometrySetRectangleBorderWidth(Rectangle *r, int width);
Rectangle *NeuroGeometrySetRectangleBorderGap(Rectangle *r, int gap);
Rectangle *NeuroGeometrySetRectangleBorderWidthAndGap(Rectangle *r, int width, int gap);
Rectangle *NeuroGeometryMirrorRectangle(Rectangle *rect, const Rectangle *reg);
Rectangle *NeuroGeometryTranspRectangle(Rectangle *rect);
Rectangle *NeuroGeometryNeuroGeometryTranspRectangle(Rectangle *rect, const Rectangle *reg);
Rectangle *NeuroGeometryCenterRectangleInRegion(Rectangle *rect, const Rectangle *reg);
bool NeuroGeometryIsPointInRectangle(const Rectangle *a, int x, int y);

