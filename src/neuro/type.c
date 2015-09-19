//----------------------------------------------------------------------------------------------------------------------
// Module      :  type
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "type.h"
#include "geometry.h"
#include "rule.h"
#include "system.h"


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

Client *allocClientT(Window w, const XWindowAttributes *wa) {
  Client *c = (Client *)malloc(sizeof(Client));
  if (!c)
    return NULL;
  *(Window *)&c->win = w;  // Pointer casted away because of const
  c->ws = 0;
  c->isNSP = False;
  c->class[ 0 ] = '\0';
  c->name[ 0 ] = '\0';
  c->title[ 0 ] = '\0';
  c->isHidden = False;
  c->freeSetterFn = NeuroRuleFreeSetterNull;
  c->fixPos = NeuroRuleFixedPositionNull;
  c->fixSize = 0;
  c->isUrgent = False;
  c->isFullScreen = False;
  NeuroGeometrySetRectangle(&(c->floatRegion), wa->x, wa->y, wa->width, wa->height);
  return c;
}

void freeClientT(Client *c) {
  if (!c)
    return;
  free(c);
  c = NULL;
}

size_t ptrArrayLengthT(const void *const *arrayPtr) {
  assert(arrayPtr);
  size_t size;
  for (size = 0; arrayPtr[ size ]; ++size)
    continue;
  return size;
}

