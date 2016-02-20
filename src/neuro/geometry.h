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

NeuroRectangle *NeuroGeometryGetRelativeRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const float *rel);
NeuroRectangle *NeuroGeometrySetRectangleGaps(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps);
NeuroRectangle *NeuroGeometryUnsetRectangleGaps(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps);
NeuroRectangle *NeuroGeometrySetRectangle(NeuroRectangle *r, int x, int y, int w, int h);
NeuroRectangle *NeuroGeometrySetRectangleBorderWidth(NeuroRectangle *r, int width);
NeuroRectangle *NeuroGeometrySetRectangleBorderGap(NeuroRectangle *r, int gap);
NeuroRectangle *NeuroGeometrySetRectangleBorderWidthAndGap(NeuroRectangle *r, int width, int gap);
NeuroRectangle *NeuroGeometryMirrorRectangle(NeuroRectangle *rect, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryTranspRectangle(NeuroRectangle *rect);
NeuroRectangle *NeuroGeometryNeuroGeometryTranspRectangle(NeuroRectangle *rect, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryCenterRectangleInRegion(NeuroRectangle *rect, const NeuroRectangle *reg);
bool NeuroGeometryIsPointInRectangle(const NeuroRectangle *a, int x, int y);

