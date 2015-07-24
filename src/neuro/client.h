//----------------------------------------------------------------------------------------------------------------------
// Module      :  client
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_CLIENT_H_
#define NEURO_CLIENT_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
void updateC(const CliPtr c);
void killC(const CliPtr c);
void updateClassAndNameC(CliPtr c);
void updateTitleC(CliPtr c);
void hideC(CliPtr c, int doRules);
void showC(CliPtr c, int doRules);
void setUrgentC(CliPtr c);
void unsetUrgentC(CliPtr c);
void minimizeC(CliPtr c);
void tileC(CliPtr c);
void freeC(CliPtr c, const FreeLocFn ff);
void toggleFreeC(CliPtr c, const FreeLocFn ff);
void normalC(CliPtr c);
void fullScreenC(CliPtr c);
void toggleFullScreenC(CliPtr c);
void movePointerC();
void resizePointerC();
void freeMovePointerC();
void freeResizePointerC();

// Select Functions
CliPtr selfC(const CliPtr c);     // Client c itself
CliPtr nextC(const CliPtr c);     // Next client of c
CliPtr prevC(const CliPtr c);     // Previous client of c
CliPtr oldC(const CliPtr c);      // Previous selected client
CliPtr headC(const CliPtr c);     // First client of the stack
CliPtr lastC(const CliPtr c);     // Last client of the stack
CliPtr upC(const CliPtr c);       // Upper client (layout position) of c
CliPtr downC(const CliPtr c);     // Lower client (layout position) of c
CliPtr leftC(const CliPtr c);     // Left client (layout position) of c
CliPtr rightC(const CliPtr c);    // Right client (layout position) of c
CliPtr pointerC(const CliPtr c);  // Client under the pointer

// Test Functions
Bool testWindowC(const CliPtr c, const void *w);
Bool testIsUrgentC(const CliPtr c, const void *p);
Bool testIsFixedC(const CliPtr c, const void *p);

// Border Color
Color onlyCurrBorderColorC(const CliPtr c);
Color allBorderColorC(const CliPtr c);
Color noBorderColorC(const CliPtr c);

// Border Width
int alwaysBorderWidthC(const CliPtr c);
int smartBorderWidthC(const CliPtr c);
int onlyCurrBorderWidthC(const CliPtr c);

// Border Gap
int alwaysBorderGapC(const CliPtr c);
int smartBorderGapC(const CliPtr c);
int onlyCurrBorderGapC(const CliPtr c);

#endif  // NEURO_CLIENT_H_

