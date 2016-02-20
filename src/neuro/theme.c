//----------------------------------------------------------------------------------------------------------------------
// Module      :  theme
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "theme.h"
#include "core.h"
#include "layout.h"
#include "dzen.h"
#include "workspace.h"
#include "system.h"
#include "monitor.h"
#include "geometry.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell theme BoxPP
static const BoxPP boxpp_nnoell_white_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_white2b_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_blue_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlue,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_blue2_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlue,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_blue2b_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlue,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_gray2_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorGray,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_green_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorGreen,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_green2_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorGreen,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP boxpp_nnoell_red2_ = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorRed,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

// Nnoell theme clickable areas
static const CA ca_nnoell_title_ = {
  "/usr/bin/xdotool key super+j",
  "/usr/bin/xdotool key super+Tab",
  "/usr/bin/xdotool key super+k",
  "/usr/bin/xdotool key super+shift+j",
  "/usr/bin/xdotool key super+shift+k"
};

static const CA ca_nnoell_calendar_ = {
  "/usr/share/themes/nnoell/neurowm/scripts/dzencal.sh",
  "/usr/share/themes/nnoell/neurowm/scripts/dzencal.sh",
  "/usr/share/themes/nnoell/neurowm/scripts/dzencal.sh",
  "/usr/share/themes/nnoell/neurowm/scripts/dzencal.sh",
  "/usr/share/themes/nnoell/neurowm/scripts/dzencal.sh",
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell theme loggers
void NeuroThemeNnoellLoggerMonitorCurrLayout(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  const size_t ws = NeuroCoreGetMonitorStack(m);
  const size_t idx = NeuroCoreStackGetLayoutIdx(ws);
  const LayoutConf *const lc = NeuroCoreStackGetCurrLayoutConf(ws);
  if (lc) {
    static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
    if (NeuroCoreStackIsCurrToggledLayout(ws))
      snprintf(tmp2, DZEN_LOGGER_MAX,
          "^fg(" NeuroThemeNnoellColorRed ")%zu^fg(" NeuroThemeNnoellColorGray ")|^fg()%s^fg()", idx + 1, lc->name);
    else
      snprintf(tmp2, DZEN_LOGGER_MAX,
          "^fg(" NeuroThemeNnoellColorGreen ")%zu^fg(" NeuroThemeNnoellColorGray ")|^fg()%s^fg()", idx + 1, lc->name);
    NeuroDzenWrapDzenBox(tmp, tmp2, &boxpp_nnoell_white_);
    NeuroDzenWrapDzenBox(str, "LAYOUT", &boxpp_nnoell_blue2_);
    strncat(str, tmp, DZEN_LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerMonitorCurrLayoutMod(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  const Layout *const l = NeuroCoreStackGetCurrLayout(NeuroCoreGetMonitorStack(m));
  if (l) {
    static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
    tmp[ 0 ] = '\0';
    if (l->mod == NEURO_LAYOUT_MOD_NULL) {
      strncpy(tmp, "Norm", DZEN_LOGGER_MAX);
    } else {
      if (l->mod & NEURO_LAYOUT_MOD_MIRROR)
        strncat(tmp, "^fg(" NeuroThemeNnoellColorGreen ")M^fg()", DZEN_LOGGER_MAX - strlen(tmp) - 1);
      if (l->mod & NEURO_LAYOUT_MOD_REFLECTX)
        strncat(tmp, "^fg(" NeuroThemeNnoellColorGreen ")X^fg()", DZEN_LOGGER_MAX - strlen(tmp) - 1);
      if (l->mod & NEURO_LAYOUT_MODE_REFLECTY)
        strncat(tmp, "^fg(" NeuroThemeNnoellColorGreen ")Y^fg()", DZEN_LOGGER_MAX - strlen(tmp) - 1);
    }
    NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
    NeuroDzenWrapDzenBox(str, "MOD", &boxpp_nnoell_blue2_);
    strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerMonitorWorkspace(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  const size_t ws = NeuroCoreGetMonitorStack(m);
  const char *const name = NeuroCoreStackGetName(ws);
  if (name) {
    static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
    snprintf(tmp2, DZEN_LOGGER_MAX,
        "^fg(" NeuroThemeNnoellColorGreen ")%zu^fg(" NeuroThemeNnoellColorGray ")|^fg()%s^fg()",
        (ws + 1) % NeuroCoreGetSize(), name);
    NeuroDzenWrapDzenBox(tmp, tmp2, &boxpp_nnoell_white_);
    NeuroDzenWrapDzenBox(str, "WORKSPACE", &boxpp_nnoell_blue2_);
    strncat(str, tmp, DZEN_LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerMonitorCurrTitle(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  const ClientPtrPtr c = NeuroCoreStackGetCurrClient(NeuroCoreGetMonitorStack(m));
  if (c) {
    static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
    NeuroDzenWrapDzenBox(tmp, CLI_GET(c).title, &boxpp_nnoell_white_);
    NeuroDzenWrapDzenBox(tmp2, "FOCUS", &boxpp_nnoell_white2b_);
    NeuroDzenWrapClickArea(str, tmp2, &ca_nnoell_title_);
    strncat(str, tmp, DZEN_LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerWorkspaceList(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const size_t size = NeuroCoreGetSize();
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ], tmp3[ DZEN_LOGGER_MAX ], tmp4[ DZEN_LOGGER_MAX ];
  for (size_t i = 0U; i < size; ++i) {
    snprintf(tmp, DZEN_LOGGER_MAX, "%zu", (i + 1) % size);
    snprintf(tmp3, DZEN_LOGGER_MAX, "/usr/bin/xdotool key super+%s", tmp);
    static const CA wslstCA = { tmp3, tmp3, tmp3, tmp3, tmp3 };
    if (NeuroCoreStackIsCurr(i))
      NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue2b_);
    else if (NeuroWorkspaceClientFindUrgent(i))
      NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_green2_);
    else if (NeuroCoreStackGetMonitor(i))
      NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_red2_);
    else if (!NeuroCoreStackIsEmpty(i))
      NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white2b_);
    else
      NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
    NeuroDzenWrapClickArea(tmp4, tmp2, &wslstCA);
    strncat(str, tmp4, DZEN_LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerTime(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ DZEN_LOGGER_MAX ];
  const time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, DZEN_LOGGER_MAX,
      "%02d^fg("NeuroThemeNnoellColorGray
      "):^fg()%02d^fg("NeuroThemeNnoellColorGray
      "):^fg("NeuroThemeNnoellColorGreen")%02d^fg()",
      res.tm_hour, res.tm_min, res.tm_sec);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_white_);
}

void NeuroThemeNnoellLoggerDate(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ DZEN_LOGGER_MAX ];
  const time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, DZEN_LOGGER_MAX,
      "%d^fg("NeuroThemeNnoellColorGray
      ").^fg()%02d^fg("NeuroThemeNnoellColorGray
      ").^fg("NeuroThemeNnoellColorBlue")%02d^fg()",
      res.tm_year+1900, res.tm_mon+1, res.tm_mday);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_white_);
}

void NeuroThemeNnoellLoggerDay(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ DZEN_LOGGER_MAX ];
  NeuroDzenLoggerDay(m, tmp);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_white2b_);
}

void NeuroThemeNnoellLoggerCalendar(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ DZEN_LOGGER_MAX ];
  NeuroDzenWrapClickArea(tmp, "CALENDAR", &ca_nnoell_calendar_);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_blue_);
}

void NeuroThemeNnoellLoggerDateTime(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ], tmp3[ DZEN_LOGGER_MAX ], tmp4[ DZEN_LOGGER_MAX ];
  NeuroThemeNnoellLoggerCalendar(m, tmp);
  NeuroThemeNnoellLoggerTime(m, tmp2);
  NeuroThemeNnoellLoggerDate(m, tmp3);
  NeuroThemeNnoellLoggerDay(m, tmp4);
  snprintf(str, DZEN_LOGGER_MAX, "%s%s%s%s", tmp4, tmp3, tmp2, tmp);
}

void NeuroThemeNnoellLoggerUptime(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  NeuroDzenLoggerUptime(m, tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "UPTIME", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorStackSize(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  const size_t size = NeuroCoreStackGetSize(NeuroCoreGetMonitorStack(m));
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  snprintf(tmp, DZEN_LOGGER_MAX, "%zu", size);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "STACK", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorNumMinimized(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  const size_t count = NeuroCoreStackGetMinimizedNum(NeuroCoreGetMonitorStack(m));
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  snprintf(tmp, DZEN_LOGGER_MAX, "%zu", count);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "MIN", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerCpu(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  NeuroDzenLoggerCpu(m, tmp);
  const char *const p = strchr(tmp, ' ');  // Skip general cpu usage
  if (!p)
    return;
  NeuroDzenWrapDzenBox(tmp2, p + 1, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "CPU", &boxpp_nnoell_gray2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
  tmp[ 0 ] = '\0';
}

void NeuroThemeNnoellLoggerRam(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  NeuroDzenLoggerRam(m, tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "RAM", &boxpp_nnoell_gray2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerBattery(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ], tmp3[ DZEN_LOGGER_MAX ], tmp4[ DZEN_LOGGER_MAX ];
  if (!NeuroDzenReadFirstLineFile(tmp, "/sys/class/power_supply/BAT0/capacity"))
    strncpy(tmp, "N/A", DZEN_LOGGER_MAX);
  else
    strncat(tmp, "%", DZEN_LOGGER_MAX);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue_);
  if (!NeuroDzenReadFirstLineFile(tmp3, "/sys/class/power_supply/BAT0/status"))
    strncpy(tmp3, "AC Conection", DZEN_LOGGER_MAX);
  NeuroDzenWrapDzenBox(tmp4, tmp3, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "BATTERY", &boxpp_nnoell_gray2_);
  strncat(tmp2, tmp4, DZEN_LOGGER_MAX - strlen(tmp2) - 1);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerWifiStrength(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  NeuroDzenLoggerWifiStrength(m, tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "WIFI", &boxpp_nnoell_gray2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerTemperature(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ], tmp3[ DZEN_LOGGER_MAX ], tmp4[ DZEN_LOGGER_MAX ];
  if (!NeuroDzenReadFirstLineFile(tmp, "/sys/bus/acpi/devices/LNXTHERM:00/thermal_zone/temp"))
    strncpy(tmp, "N/A", DZEN_LOGGER_MAX);
  else
    snprintf(tmp, DZEN_LOGGER_MAX, "%i°C", atoi(tmp) / 1000);
  if (!NeuroDzenReadFirstLineFile(tmp2, "/sys/bus/acpi/devices/LNXTHERM:01/thermal_zone/temp"))
    strncpy(tmp2, "N/A", DZEN_LOGGER_MAX);
  else
    snprintf(tmp2, DZEN_LOGGER_MAX, "%i°C", atoi(tmp2) / 1000);
  snprintf(tmp3, DZEN_LOGGER_MAX, "%s %s", tmp, tmp2);
  NeuroDzenWrapDzenBox(tmp4, tmp3, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "TEMP", &boxpp_nnoell_gray2_);
  strncat(str, tmp4, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerScreen(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const Rectangle *const r = NeuroSystemGetScreenRegion();
  static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ];
  snprintf(tmp, DZEN_LOGGER_MAX, "%ix%i", r->w, r->h);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "SCREEN", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorList(const Monitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const Monitor *const curr_m = NeuroCoreStackGetMonitor(NeuroCoreGetCurrStack());
  for (const Monitor * m = NeuroMonitorSelectorLast(NULL); m; m = NeuroMonitorSelectorPrev(m)) {
    static char tmp[ DZEN_LOGGER_MAX ], tmp2[ DZEN_LOGGER_MAX ], tmp3[ DZEN_LOGGER_MAX ];
    static Rectangle r = { 0 };
    NeuroGeometryUnsetRectangleGaps(&r, &m->region, m->gaps);
    snprintf(tmp, DZEN_LOGGER_MAX, "%ix%i", r.w, r.h);
    if (m == NeuroMonitorSelectorLast(NULL))
      NeuroDzenWrapDzenBox(tmp2, m->name ? m->name : "Unknown", &boxpp_nnoell_blue2_);
    else
      NeuroDzenWrapDzenBox(tmp2, m->name ? m->name : "Unknown", &boxpp_nnoell_blue_);
    strncat(str, tmp2, DZEN_LINE_MAX - strlen(str) - 1);
    if (m == curr_m)
      NeuroDzenWrapDzenBox(tmp3, tmp, &boxpp_nnoell_green_);
    else
      NeuroDzenWrapDzenBox(tmp3, tmp, &boxpp_nnoell_white_);
    strncat(str, tmp3, DZEN_LINE_MAX - strlen(str) - 1);
  }
}

