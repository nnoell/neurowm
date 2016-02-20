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
void NeuroThemeNnoellLoggerMonitorCurrLayout(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerMonitorCurrLayoutMod(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerMonitorWorkspace(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerMonitorCurrTitle(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerWorkspaceList(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerTime(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerDate(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerDay(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerCalendar(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerDateTime(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerUptime(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerMonitorStackSize(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerMonitorNumMinimized(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerCpu(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerRam(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerBattery(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerWifiStrength(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerTemperature(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerScreen(const NeuroMonitor *m, char *str);
void NeuroThemeNnoellLoggerMonitorList(const NeuroMonitor *m, char *str);

