//----------------------------------------------------------------------------------------------------------------------
// Module      :  event
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_EVENT_H_
#define NEURO_EVENT_H_

// Includes
#include "type.h"


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// NeuroEventHandler
typedef void (*NeuroEventHandlerFn)(XEvent *e);

// NeuroEventType
typedef unsigned int NeuroEventType;


//----------------------------------------------------------------------------------------------------------------------
// VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

NeuroEventHandlerFn NeuroEventGetHandler(NeuroEventType t);
void NeuroEventManageWindow(Window w);
void NeuroEventUnmanageClient(ClientPtrPtr c);
void NeuroEventLoadWindows();

#endif  // NEURO_EVENT_H_

