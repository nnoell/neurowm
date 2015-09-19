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
Bool initDP();
void endDP();
void updateDP(int onlyEvent);

// CPU Calculation
void startCpuCalcDP();
void endCpuCalcDP();

// Loggers
void timeLoggerDP(char *str);
void dateLoggerDP(char *str);
void dayLoggerDP(char *str);
void uptimeLoggerDP(char *str);
void memPercLoggerDP(char *str);
void wifiStrengthDP(char *str);
void currWSLoggerDP(char *str);
void currLayoutLoggerDP(char *str);
void currTitleLoggerDP(char *str);
void cpuPercUsageLoggerDP(char *str);

// Logger Utils
int readFileDP(char *buf, const char *fileName);
void wrapDzenBoxDP(char *dst, const char *src, const BoxPP *b);
void wrapDzenClickAreaDP(char *dst, const char *src, const CA *ca);

#endif  // NEURO_DZEN_H_

