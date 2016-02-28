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
NeuroClient *NeuroTypeNewClient(Window w, const XWindowAttributes *wa) {
  NeuroClient *const c = (NeuroClient *)malloc(sizeof(NeuroClient));
  if (!c)
    return NULL;

  // Set region
  c->float_region.p.x = wa ? wa->x : 0;
  c->float_region.p.y = wa ? wa->y : 0;
  c->float_region.w = wa ? wa->width : 500;
  c->float_region.h = wa ? wa->height : 500;

  // Set the window
  *(Window *)&c->win = w;

  // Set the properties
  c->ws = 0;
  c->is_nsp = false;
  c->class[ 0 ] = '\0';
  c->name[ 0 ] = '\0';
  c->title[ 0 ] = '\0';
  c->is_fullscreen = false;
  c->free_setter_fn = NeuroRuleFreeSetterNull;
  c->fixed_pos = NEURO_FIXED_POSITION_NULL;
  c->fixed_size = 0;
  c->is_urgent = false;

  return c;
}

void NeuroTypeDeleteClient(NeuroClient *c) {
  if (!c)
    return;
  free(c);
  c = NULL;
}

// Basic Functions
NeuroIndex NeuroTypeArrayLength(const void *const *array_ptr) {
  if (!array_ptr)
    return 0U;

  NeuroIndex i = 0U;
  while (array_ptr[ i ])
    ++i;

  return i;
}

