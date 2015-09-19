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

#ifndef NEURO_DZEN_H_
#define NEURO_DZEN_H_

// Includes
#include "type.h"

// Defines
#define DZEN_LINE_MAX 16384
#define DZEN_ARGS_MAX 64
#define LOGGER_MAX    8192


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Basic Functions
Bool NeuroDzenInit();
void NeuroDzenStop();
void NeuroDzenUpdate(int onlyEvent);
void NeuroDzenStartCpuCalc();
void NeuroDzenStopCpuCalc();
void NeuroDzenWrapDzenBox(char *dst, const char *src, const BoxPP *b);
void NeuroDzenWrapClickArea(char *dst, const char *src, const CA *ca);
int NeuroDzenReadFile(char *buf, const char *fileName);

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

#endif  // NEURO_DZEN_H_

