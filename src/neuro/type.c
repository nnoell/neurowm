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

// Creation and Destruction
Client *NeuroTypeNewClient(Window w, const XWindowAttributes *wa) {
  Client *c = (Client *)malloc(sizeof(Client));
  if (!c)
    return NULL;
  *(Window *)&c->win = w;  // Pointer casted away because of const
  c->ws = 0;
  c->is_nsp = False;
  c->class[ 0 ] = '\0';
  c->name[ 0 ] = '\0';
  c->title[ 0 ] = '\0';
  c->is_hidden = False;
  c->free_setter_fn = NeuroRuleFreeSetterNull;
  c->fixed_pos = RuleFixedPositionNull;
  c->fixed_size = 0;
  c->is_urgent = False;
  c->is_fullscreen = False;
  NeuroGeometrySetRectangle(&(c->float_region), wa->x, wa->y, wa->width, wa->height);
  return c;
}

void NeuroTypeDeleteClient(Client *c) {
  if (!c)
    return;
  free(c);
  c = NULL;
}

// Basic Functions
size_t NeuroTypeArrayLength(const void *const *arrayPtr) {
  assert(arrayPtr);
  size_t size;
  for (size = 0; arrayPtr[ size ]; ++size)
    continue;
  return size;
}

