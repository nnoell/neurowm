//----------------------------------------------------------------------------------------------------------------------
// Module      :  theme
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_THEME_H_
#define NEURO_THEME_H_

// Includes
#include "type.h"

// Nnoell colors
#define NNOELL_colorBlackP       "#020202"
#define NNOELL_colorBlackAltP    "#1c1c1c"
#define NNOELL_colorGrayP        "#444444"
#define NNOELL_colorGrayAltP     "#101010"
#define NNOELL_colorGrayAlt2P    "#404040"
#define NNOELL_colorGrayAlt3P    "#252525"
#define NNOELL_colorWhiteP       "#a9a6af"
#define NNOELL_colorWhiteAltP    "#9d9d9d"
#define NNOELL_colorWhiteAlt2P   "#b5b3b3"
#define NNOELL_colorWhiteAlt3P   "#707070"
#define NNOELL_colorMagentaP     "#8e82a2"
#define NNOELL_colorBlueP        "#44aacc"
#define NNOELL_colorBlueAltP     "#3955c4"
#define NNOELL_colorRedP         "#f7a16e"
#define NNOELL_colorRedAltP      "#e0105f"
#define NNOELL_colorGreenP       "#66ff66"
#define NNOELL_colorGreenAltP    "#558965"


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell loggers
void nnoellCurrLayoutLoggerP(char *str);
void nnoellCurrLayoutModLoggerP(char *str);
void nnoellCurrWSLoggerP(char *str);
void nnoellCurrTitleLoggerP(char *str);
void nnoellCurrWSListLoggerP(char *str);
void nnoellTimeLoggerP(char *str);
void nnoellDateLoggerP(char *str);
void nnoellDayLoggerP(char *str);
void nnoellCalendarLoggerP(char *str);
void nnoellDateTimeLoggerP(char *str);
void nnoellUptimeLoggerP(char *str);
void nnoellCurrSizeStackLoggerP(char *str);
void nnoellCurrMinimizedCountLoggerP(char *str);
void nnoellCPUUsageLoggerP(char *str);
void nnoellRamPercLoggerP(char *str);
void nnoellBatteryLoggerP(char *str);
void nnoellWifiStrengthLoggerP(char *str);
void nnoellTempLoggerP(char *str);


#endif  // NEURO_THEME_H_

