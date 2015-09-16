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
void updateC(ClientPtrPtr c, const void *data);
void updateClassAndNameC(ClientPtrPtr c, const void *data);
void updateTitleC(ClientPtrPtr c, const void *data);
void hideC(ClientPtrPtr c, const void *doRules);
void showC(ClientPtrPtr c, const void *doRules);
void setUrgentC(ClientPtrPtr c, const void *data);
void unsetUrgentC(ClientPtrPtr c, const void *data);
void killC(ClientPtrPtr c, const void *data);
void minimizeC(ClientPtrPtr c, const void *data);
void tileC(ClientPtrPtr c, const void *data);
void freeC(ClientPtrPtr c, const void *freeSetterFn);
void toggleFreeC(ClientPtrPtr c, const void *freeSetterFn);
void normalC(ClientPtrPtr c, const void *data);
void fullScreenC(ClientPtrPtr c, const void *data);
void toggleFullScreenC(ClientPtrPtr c, const void *data);
void moveC(ClientPtrPtr c, const void *data);
void resizeC(ClientPtrPtr c, const void *data);
void freeMoveC(ClientPtrPtr c, const void *freeSetterFn);
void freeResizeC(ClientPtrPtr c, const void *freeSetterFn);

// Client getters
ClientPtrPtr getFocusedC();
ClientPtrPtr getPointerC(int *x, int *y);

// Client Selectors
ClientPtrPtr selfC(const ClientPtrPtr c);  // Client c itself
ClientPtrPtr nextC(const ClientPtrPtr c);  // The next client of c
ClientPtrPtr prevC(const ClientPtrPtr c);  // The previous client of c
ClientPtrPtr oldC(const ClientPtrPtr c);   // The previous selected client
ClientPtrPtr headC(const ClientPtrPtr c);  // The first client of the stack
ClientPtrPtr lastC(const ClientPtrPtr c);  // The last client of the stack
ClientPtrPtr upC(const ClientPtrPtr c);    // The upper client (layout position) of c
ClientPtrPtr downC(const ClientPtrPtr c);  // The lower client (layout position) of c
ClientPtrPtr leftC(const ClientPtrPtr c);  // The left client (layout position) of c
ClientPtrPtr rightC(const ClientPtrPtr c); // The right client (layout position) of c

// Client Testers
Bool testWindowC(const ClientPtrPtr c, const void *w);
Bool testIsUrgentC(const ClientPtrPtr c, const void *p);
Bool testIsFixedC(const ClientPtrPtr c, const void *p);

// Color Setters
Color onlyCurrBorderColorC(const ClientPtrPtr c);
Color allBorderColorC(const ClientPtrPtr c);
Color noBorderColorC(const ClientPtrPtr c);

// Border Width Setters
int alwaysBorderWidthC(const ClientPtrPtr c);
int neverBorderWidthC(const ClientPtrPtr c);
int smartBorderWidthC(const ClientPtrPtr c);
int onlyCurrBorderWidthC(const ClientPtrPtr c);

// Border Gap Setters
int alwaysBorderGapC(const ClientPtrPtr c);
int neverBorderGapC(const ClientPtrPtr c);
int smartBorderGapC(const ClientPtrPtr c);
int onlyCurrBorderGapC(const ClientPtrPtr c);

#endif  // NEURO_CLIENT_H_

