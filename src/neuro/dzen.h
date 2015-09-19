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
Bool initD();
void stopD();
void updateD(int onlyEvent);

// CPU Calculation
void startCpuCalcD();
void endCpuCalcD();

// Loggers
void timeLoggerD(char *str);
void dateLoggerD(char *str);
void dayLoggerD(char *str);
void uptimeLoggerD(char *str);
void ramPercLoggerD(char *str);
void wifiStrengthD(char *str);
void currWSLoggerD(char *str);
void currLayoutLoggerD(char *str);
void currTitleLoggerD(char *str);
void cpuPercUsageLoggerD(char *str);

// Logger Utils
int readFileD(char *buf, const char *fileName);
void wrapDzenBoxD(char *dst, const char *src, const BoxPP *b);
void wrapDzenClickAreaD(char *dst, const char *src, const CA *ca);

#endif  // NEURO_DZEN_H_

