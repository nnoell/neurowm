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

// Point Getters
NeuroPoint *NeuroGeometryPointGetUpper(NeuroPoint *dst, const NeuroPoint *src, int distance);
NeuroPoint *NeuroGeometryPointGetLower(NeuroPoint *dst, const NeuroPoint *src, int distance);
NeuroPoint *NeuroGeometryPointGetLeft(NeuroPoint *dst, const NeuroPoint *src, int distance);
NeuroPoint *NeuroGeometryPointGetRight(NeuroPoint *dst, const NeuroPoint *src, int distance);

// Rectangle Getters
NeuroRectangle *NeuroGeometryRectangleGetRelative(NeuroRectangle *dst, const NeuroRectangle *src, const float *rel);
NeuroRectangle *NeuroGeometryRectangleGetReduced(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps);
NeuroRectangle *NeuroGeometryRectangleGetIncreased(NeuroRectangle *dst, const NeuroRectangle *src, const int *gaps);

// Rectanlge Modifiers
NeuroRectangle *NeuroGeometryRectangleSetBorderWidth(NeuroRectangle *r, int width);
NeuroRectangle *NeuroGeometryRectangleSetBorderGap(NeuroRectangle *r, int gap);
NeuroRectangle *NeuroGeometryRectangleSetBorderWidthAndGap(NeuroRectangle *r, int width, int gap);
NeuroRectangle *NeuroGeometryRectangleMirror(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryRectangleReflectX(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryRectangleReflectY(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryRectangleTranspose(NeuroRectangle *r);
NeuroRectangle *NeuroGeometryRectangleFit(NeuroRectangle *r, const NeuroRectangle *reg);
NeuroRectangle *NeuroGeometryRectangleCenter(NeuroRectangle *r, const NeuroRectangle *reg);

// Point-Rectangle Testers
bool NeuroGeometryIsPointInRectangle(const NeuroRectangle *r, const NeuroPoint *p);

