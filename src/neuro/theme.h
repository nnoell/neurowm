//----------------------------------------------------------------------------------------------------------------------
// Module      :  theme
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------

#pragma once

// Includes
#include "type.h"

// Nnoell theme paths
#define NeuroThemeNnoellDzenBoxLeftIcon  "/usr/share/themes/nnoell/neurowm/xbm_icons/boxleft.xbm"
#define NeuroThemeNnoellDzenBoxLeftIcon2 "/usr/share/themes/nnoell/neurowm/xbm_icons/boxleft2.xbm"
#define NeuroThemeNnoellDzenBoxRightIcon "/usr/share/themes/nnoell/neurowm/xbm_icons/boxright.xbm"

// Nnoell theme colors
#define NeuroThemeNnoellColorBlack     "#020202"
#define NeuroThemeNnoellColorBlackAlt  "#1c1c1c"
#define NeuroThemeNnoellColorGray      "#444444"
#define NeuroThemeNnoellColorGrayAlt   "#101010"
#define NeuroThemeNnoellColorGrayAlt2  "#404040"
#define NeuroThemeNnoellColorGrayAlt3  "#252525"
#define NeuroThemeNnoellColorWhite     "#a9a6af"
#define NeuroThemeNnoellColorWhiteAlt  "#9d9d9d"
#define NeuroThemeNnoellColorWhiteAlt2 "#b5b3b3"
#define NeuroThemeNnoellColorWhiteAlt3 "#707070"
#define NeuroThemeNnoellColorMagenta   "#8e82a2"
#define NeuroThemeNnoellColorBlue      "#44aacc"
#define NeuroThemeNnoellColorBlueAlt   "#3955c4"
#define NeuroThemeNnoellColorRed       "#f7a16e"
#define NeuroThemeNnoellColorRedAlt    "#e0105f"
#define NeuroThemeNnoellColorGreen     "#66ff66"
#define NeuroThemeNnoellColorGreenAlt  "#558965"

// Nnoell theme fonts
#define NeuroThemeNnoellFontDzenPanel "-*-terminus-medium-r-normal-*-11-*-*-*-*-*-*-*"


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell theme loggers
void NeuroThemeNnoellLoggerCurrLayout(char *str);
void NeuroThemeNnoellLoggerLayoutMod(char *str);
void NeuroThemeNnoellLoggerCurrWorkspace(char *str);
void NeuroThemeNnoellLoggerCurrTitle(char *str);
void NeuroThemeNnoellLoggerWorkspaceList(char *str);
void NeuroThemeNnoellLoggerTime(char *str);
void NeuroThemeNnoellLoggerDate(char *str);
void NeuroThemeNnoellLoggerDay(char *str);
void NeuroThemeNnoellLoggerCalendar(char *str);
void NeuroThemeNnoellLoggerDateTime(char *str);
void NeuroThemeNnoellLoggerUptime(char *str);
void NeuroThemeNnoellLoggerStackSize(char *str);
void NeuroThemeNnoellLoggerMinimizedCount(char *str);
void NeuroThemeNnoellLoggerCpu(char *str);
void NeuroThemeNnoellLoggerRam(char *str);
void NeuroThemeNnoellLoggerBattery(char *str);
void NeuroThemeNnoellLoggerWifiStrength(char *str);
void NeuroThemeNnoellLoggerTemperature(char *str);

