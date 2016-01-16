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
#define DZEN_ON_EVENT 0
#define DZEN_LINE_MAX 16384
#define DZEN_ARGS_MAX 64
#define DZEN_LOGGER_MAX 8192


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
Bool NeuroDzenInit();
void NeuroDzenStop();
void NeuroDzenRefresh(Bool on_event_only);
void NeuroDzenInitCpuCalc();
void NeuroDzenStopCpuCalc();
void NeuroDzenWrapDzenBox(char *dst, const char *src, const BoxPP *b);
void NeuroDzenWrapClickArea(char *dst, const char *src, const CA *ca);
Bool NeuroDzenReadFirstLineFile(char *buf, const char *path);

// Loggers
void NeuroDzenLoggerTime(char *str);
void NeuroDzenLoggerDate(char *str);
void NeuroDzenLoggerDay(char *str);
void NeuroDzenLoggerUptime(char *str);
void NeuroDzenLoggerCpu(char *str);
void NeuroDzenLoggerRam(char *str);
void NeuroDzenLoggerWifiStrength(char *str);
void NeuroDzenLoggerCurrWorkspace(char *str);
void NeuroDzenLoggerCurrLayout(char *str);
void NeuroDzenLoggerCurrTitle(char *str);

