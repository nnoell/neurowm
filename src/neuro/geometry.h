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

// Point Generators
NeuroPoint *NeuroGeometryGetUpperPoint(NeuroPoint *dst, const NeuroPoint *src, int distance);
NeuroPoint *NeuroGeometryGetLowerPoint(NeuroPoint *dst, const NeuroPoint *src, int distance);
NeuroPoint *NeuroGeometryGetLeftPoint(NeuroPoint *dst, const NeuroPoint *src, int distance);
NeuroPoint *NeuroGeometryGetRightPoint(NeuroPoint *dst, const NeuroPoint *src, int distance);

// Rectangle Generators
NeuroRectangle *NeuroGeometryGetRelativeRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const float *rel);
NeuroRectangle *NeuroGeometryGetReducedRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps);
NeuroRectangle *NeuroGeometryGetIncreasedRectangle(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps);

// Rectanlge Updaters
NeuroRectangle *NeuroGeometrySetRectangleBorderWidth(NeuroRectangle *r, int width);
NeuroRectangle *NeuroGeometrySetRectangleBorderGap(NeuroRectangle *r, int gap);
NeuroRectangle *NeuroGeometrySetRectangleBorderWidthAndGap(NeuroRectangle *r, int width, int gap);
NeuroRectangle *NeuroGeometryMirrorRectangle(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryReflectXRectangle(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryReflectYRectangle(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryTranspRectangle(NeuroRectangle *r);
NeuroRectangle *NeuroGeometryFitRectangleInRegion(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryCenterRectangleInRegion(NeuroRectangle *r, const NeuroRectangle *reg);

// Point-Rectangle Testers
bool NeuroGeometryIsPointInRectangle(const NeuroRectangle *r, const NeuroPoint *p);

