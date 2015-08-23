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
void updateC(CliPtr c, const void *data);
void updateClassAndNameC(CliPtr c, const void *data);
void updateTitleC(CliPtr c, const void *data);
void hideC(CliPtr c, const void *doRules);
void showC(CliPtr c, const void *doRules);
void setUrgentC(CliPtr c, const void *data);
void unsetUrgentC(CliPtr c, const void *data);
void killC(CliPtr c, const void *data);
void minimizeC(CliPtr c, const void *data);
void tileC(CliPtr c, const void *data);
void freeC(CliPtr c, const void *freeSetterFn);
void toggleFreeC(CliPtr c, const void *freeSetterFn);
void normalC(CliPtr c, const void *data);
void fullScreenC(CliPtr c, const void *data);
void toggleFullScreenC(CliPtr c, const void *data);
void moveC(CliPtr c, const void *data);
void resizeC(CliPtr c, const void *data);
void freeMoveC(CliPtr c, const void *data);
void freeResizeC(CliPtr c, const void *data);

// Client Selectors
CliPtr selfC(const CliPtr c);     // Client c itself
CliPtr nextC(const CliPtr c);     // The next client of c
CliPtr prevC(const CliPtr c);     // The previous client of c
CliPtr oldC(const CliPtr c);      // The previous selected client
CliPtr headC(const CliPtr c);     // The first client of the stack
CliPtr lastC(const CliPtr c);     // The last client of the stack
CliPtr upC(const CliPtr c);       // The upper client (layout position) of c
CliPtr downC(const CliPtr c);     // The lower client (layout position) of c
CliPtr leftC(const CliPtr c);     // The left client (layout position) of c
CliPtr rightC(const CliPtr c);    // The right client (layout position) of c

// Client Testers
Bool testWindowC(const CliPtr c, const void *w);
Bool testIsUrgentC(const CliPtr c, const void *p);
Bool testIsFixedC(const CliPtr c, const void *p);

// Color Setters
Color onlyCurrBorderColorC(const CliPtr c);
Color allBorderColorC(const CliPtr c);
Color noBorderColorC(const CliPtr c);

// Border Width Setters
int alwaysBorderWidthC(const CliPtr c);
int neverBorderWidthC(const CliPtr c);
int smartBorderWidthC(const CliPtr c);
int onlyCurrBorderWidthC(const CliPtr c);

// Border Gap Setters
int alwaysBorderGapC(const CliPtr c);
int neverBorderGapC(const CliPtr c);
int smartBorderGapC(const CliPtr c);
int onlyCurrBorderGapC(const CliPtr c);

#endif  // NEURO_CLIENT_H_

