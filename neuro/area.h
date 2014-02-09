//----------------------------------------------------------------------------------------------------------------------
// Module      :  area
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_AREA_H_
#define NEURO_AREA_H_

// Includes
#include "general.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

Area *getRelativeAreaA(Area *dst, const Area *src, const float *rel);
Area *getGapsAreaA(Area *dst, const Area *src, const int *gaps);
Area *setAreaA(Area *r, int x, int y, int w, int h);
Area *setAreaBorderA(Area *r, int bs);
Area *setAreaSpaceA(Area *r, int sp);
Area *setAreaBorderAndSpaceA(Area *r, int bs, int s);
Area *mirrorAreaA(Area *rect, const Area *reg);
Area *transpAreaA(Area *rect);
Area *fitAreaInRegA(Area *rect, const Area *reg);
Area *centerAreaInRegA(Area *rect, const Area *reg);
Bool isPointInArea(const Area *a, int x, int y);

#endif  // NEURO_AREA_H_

