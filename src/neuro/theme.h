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
#define NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON  "/usr/share/themes/nnoell/neurowm/xbm_icons/boxleft.xbm"
#define NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2 "/usr/share/themes/nnoell/neurowm/xbm_icons/boxleft2.xbm"
#define NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON "/usr/share/themes/nnoell/neurowm/xbm_icons/boxright.xbm"

// Nnoell theme colors
#define NEURO_THEME_NNOELL_COLOR_BLACK      "#020202"
#define NEURO_THEME_NNOELL_COLOR_BLACK_ALT  "#1c1c1c"
#define NEURO_THEME_NNOELL_COLOR_GRAY       "#444444"
#define NEURO_THEME_NNOELL_COLOR_GRAY_ALT   "#101010"
#define NEURO_THEME_NNOELL_COLOR_GRAY_ALT2  "#404040"
#define NEURO_THEME_NNOELL_COLOR_GRAY_ALT3  "#252525"
#define NEURO_THEME_NNOELL_COLOR_WHITE      "#a9a6af"
#define NEURO_THEME_NNOELL_COLOR_WHITE_ALT  "#9d9d9d"
#define NEURO_THEME_NNOELL_COLOR_WHITE_ALT2 "#b5b3b3"
#define NEURO_THEME_NNOELL_COLOR_WHITE_ALT3 "#707070"
#define NEURO_THEME_NNOELL_COLOR_MAGENTA    "#8e82a2"
#define NEURO_THEME_NNOELL_COLOR_BLUE       "#44aacc"
#define NEURO_THEME_NNOELL_COLOR_BLUE_ALT   "#3955c4"
#define NEURO_THEME_NNOELL_COLOR_RED        "#f7a16e"
#define NEURO_THEME_NNOELL_COLOR_RED_ALT    "#e0105f"
#define NEURO_THEME_NNOELL_COLOR_GREEN      "#66ff66"
#define NEURO_THEME_NNOELL_COLOR_GREEN_ALT  "#558965"

// Nnoell theme fonts
#define NEURO_THEME_NNOELL_DZEN_FONT "-*-terminus-medium-r-normal-*-11-*-*-*-*-*-*-*"


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

