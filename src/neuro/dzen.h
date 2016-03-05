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
#define NEURO_DZEN_REFRESH_ON_EVENT 0U
#define NEURO_DZEN_LINE_MAX 16384
#define NEURO_DZEN_ARGS_MAX 64
#define NEURO_DZEN_LOGGER_MAX 8192


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
bool NeuroDzenInit(void);
void NeuroDzenStop(void);
void NeuroDzenRefresh(bool on_event_only);
void NeuroDzenInitCpuCalc(void);
void NeuroDzenStopCpuCalc(void);
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

