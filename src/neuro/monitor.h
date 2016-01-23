//----------------------------------------------------------------------------------------------------------------------
// Module      :  monitor
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

// Basic Functions
bool NeuroMonitorInit();
void NeuroMonitorStop();
size_t NeuroMonitorGetSize();

// Getters
const Monitor *NeuroMonitorGet(size_t m);
const Monitor *NeuroMonitorFindDefault(size_t ws);
const Monitor *NeuroMonitorFindPtr(int x, int y);

// Selector Functions
const Monitor *NeuroMonitorSelectorHead(const Monitor *m);  // The first monitor
const Monitor *NeuroMonitorSelectorLast(const Monitor *m);  // The last monitor
const Monitor *NeuroMonitorSelectorNext(const Monitor *m);  // The next monitor of m
const Monitor *NeuroMonitorSelectorPrev(const Monitor *m);  // The previous monitor of m
const Monitor *NeuroMonitorSelector0(const Monitor *m);     // The monitor with index 0
const Monitor *NeuroMonitorSelector1(const Monitor *m);     // The monitor with index 1
const Monitor *NeuroMonitorSelector2(const Monitor *m);     // The monitor with index 2
const Monitor *NeuroMonitorSelector3(const Monitor *m);     // The monitor with index 3
const Monitor *NeuroMonitorSelector4(const Monitor *m);     // The monitor with index 4
const Monitor *NeuroMonitorSelector5(const Monitor *m);     // The monitor with index 5
const Monitor *NeuroMonitorSelector6(const Monitor *m);     // The monitor with index 6
const Monitor *NeuroMonitorSelector7(const Monitor *m);     // The monitor with index 7

