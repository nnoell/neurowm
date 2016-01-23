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
void NeuroDzenWrapDzenBox(char *dst, const char *src, const BoxPP *b);
void NeuroDzenWrapClickArea(char *dst, const char *src, const CA *ca);
bool NeuroDzenReadFirstLineFile(char *buf, const char *path);

// Loggers
void NeuroDzenLoggerTime(const Monitor *m, char *str);
void NeuroDzenLoggerDate(const Monitor *m, char *str);
void NeuroDzenLoggerDay(const Monitor *m, char *str);
void NeuroDzenLoggerUptime(const Monitor *m, char *str);
void NeuroDzenLoggerCpu(const Monitor *m, char *str);
void NeuroDzenLoggerRam(const Monitor *m, char *str);
void NeuroDzenLoggerWifiStrength(const Monitor *m, char *str);
void NeuroDzenLoggerMonitorWorkspace(const Monitor *m, char *str);
void NeuroDzenLoggerMonitorCurrLayout(const Monitor *m, char *str);
void NeuroDzenLoggerMonitorCurrTitle(const Monitor *m, char *str);
void NeuroDzenLoggerScreen(const Monitor *m, char *str);
void NeuroDzenLoggerMonitorList(const Monitor *m, char *str);

