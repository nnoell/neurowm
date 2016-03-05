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
bool NeuroMonitorInit(void);
void NeuroMonitorStop(void);
NeuroIndex NeuroMonitorGetSize(void);

// Getters
const NeuroMonitor *NeuroMonitorGet(NeuroIndex m);
const NeuroMonitor *NeuroMonitorFindDefault(NeuroIndex ws);
const NeuroMonitor *NeuroMonitorFindPointed(const NeuroPoint *p);

// Selector Functions
const NeuroMonitor *NeuroMonitorSelectorHead(const NeuroMonitor *m);  // The first monitor
const NeuroMonitor *NeuroMonitorSelectorLast(const NeuroMonitor *m);  // The last monitor
const NeuroMonitor *NeuroMonitorSelectorNext(const NeuroMonitor *m);  // The next monitor of m
const NeuroMonitor *NeuroMonitorSelectorPrev(const NeuroMonitor *m);  // The previous monitor of m
const NeuroMonitor *NeuroMonitorSelector0(const NeuroMonitor *m);     // The monitor with index 0
const NeuroMonitor *NeuroMonitorSelector1(const NeuroMonitor *m);     // The monitor with index 1
const NeuroMonitor *NeuroMonitorSelector2(const NeuroMonitor *m);     // The monitor with index 2
const NeuroMonitor *NeuroMonitorSelector3(const NeuroMonitor *m);     // The monitor with index 3
const NeuroMonitor *NeuroMonitorSelector4(const NeuroMonitor *m);     // The monitor with index 4
const NeuroMonitor *NeuroMonitorSelector5(const NeuroMonitor *m);     // The monitor with index 5
const NeuroMonitor *NeuroMonitorSelector6(const NeuroMonitor *m);     // The monitor with index 6
const NeuroMonitor *NeuroMonitorSelector7(const NeuroMonitor *m);     // The monitor with index 7

