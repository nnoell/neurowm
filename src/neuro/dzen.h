//----------------------------------------------------------------------------------------------------------------------
// Module      :  dzen
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

// Defines
#define DZEN_ON_EVENT 0U
#define DZEN_LINE_MAX 16384
#define DZEN_ARGS_MAX 64
#define DZEN_LOGGER_MAX 8192


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
bool NeuroDzenInit();
void NeuroDzenStop();
void NeuroDzenRefresh(bool on_event_only);
void NeuroDzenInitCpuCalc();
void NeuroDzenStopCpuCalc();
void NeuroDzenWrapDzenBox(char *dst, const char *src, const NeuroDzenBox *b);
void NeuroDzenWrapClickArea(char *dst, const char *src, const NeuroDzenClickableArea *ca);
bool NeuroDzenReadFirstLineFile(char *buf, const char *path);

// Loggers
void NeuroDzenLoggerTime(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerDate(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerDay(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerUptime(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerCpu(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerRam(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerWifiStrength(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerMonitorWorkspace(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerMonitorCurrLayout(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerMonitorCurrTitle(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerScreen(const NeuroMonitor *m, char *str);
void NeuroDzenLoggerMonitorList(const NeuroMonitor *m, char *str);

