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

// Nnoell theme NeuroDzenBox
static const NeuroDzenBox boxpp_nnoell_white_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_COLOR_GRAY_ALT,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_white2b_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_WHITE_ALT,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_blue_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLUE,
  NEURO_THEME_NNOELL_COLOR_GRAY_ALT,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_blue2_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLUE,
  NEURO_THEME_NNOELL_COLOR_GRAY_ALT,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_blue2b_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLUE,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_gray2_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_GRAY,
  NEURO_THEME_NNOELL_COLOR_GRAY_ALT,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_green_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_GREEN,
  NEURO_THEME_NNOELL_COLOR_GRAY_ALT,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_green2_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_GREEN,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

static const NeuroDzenBox boxpp_nnoell_red2_ = {
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_BLACK,
  NEURO_THEME_NNOELL_COLOR_RED,
  NEURO_THEME_NNOELL_DZEN_BOX_LEFT_ICON2,
  NEURO_THEME_NNOELL_DZEN_BOX_RIGHT_ICON,
  14
};

// Nnoell theme clickable areas
static const NeuroDzenClickableArea ca_nnoell_title_ = {
  "/usr/bin/xdotool key super+j",
  "/usr/bin/xdotool key super+Tab",
  "/usr/bin/xdotool key super+k",
  "/usr/bin/xdotool key super+shift+j",
  "/usr/bin/xdotool key super+shift+k"
};

static const NeuroDzenClickableArea ca_nnoell_calendar_ = {
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
void NeuroThemeNnoellLoggerMonitorCurrLayout(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroIndex ws = NeuroCoreGetMonitorStack(m);
  const NeuroLayoutConf *const lc = NeuroCoreStackGetCurrLayoutConf(ws);
  if (!lc)
    return;

  const NeuroIndex idx = NeuroCoreStackGetLayoutIdx(ws);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  if (NeuroCoreStackIsCurrToggledLayout(ws))
    snprintf(tmp2, NEURO_DZEN_LOGGER_MAX,
        "^fg(" NEURO_THEME_NNOELL_COLOR_RED ")%zu^fg(" NEURO_THEME_NNOELL_COLOR_GRAY ")|^fg()%s^fg()",
        idx + 1, lc->name);
  else
    snprintf(tmp2, NEURO_DZEN_LOGGER_MAX,
        "^fg(" NEURO_THEME_NNOELL_COLOR_GREEN ")%zu^fg(" NEURO_THEME_NNOELL_COLOR_GRAY ")|^fg()%s^fg()",
        idx + 1, lc->name);
  NeuroDzenWrapDzenBox(tmp, tmp2, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "LAYOUT", &boxpp_nnoell_blue2_);
  strncat(str, tmp, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorCurrLayoutMod(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroLayout *const l = NeuroCoreStackGetCurrLayout(NeuroCoreGetMonitorStack(m));
  if (!l)
    return;

  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  tmp[ 0 ] = '\0';
  if (l->mod == NEURO_LAYOUT_MOD_NULL) {
    strncpy(tmp, "Norm", NEURO_DZEN_LOGGER_MAX);
  } else {
    if (l->mod & NEURO_LAYOUT_MOD_MIRROR)
      strncat(tmp, "^fg(" NEURO_THEME_NNOELL_COLOR_GREEN ")M^fg()", NEURO_DZEN_LOGGER_MAX - strlen(tmp) - 1);
    if (l->mod & NEURO_LAYOUT_MOD_REFLECTX)
      strncat(tmp, "^fg(" NEURO_THEME_NNOELL_COLOR_GREEN ")X^fg()", NEURO_DZEN_LOGGER_MAX - strlen(tmp) - 1);
    if (l->mod & NEURO_LAYOUT_MODE_REFLECTY)
      strncat(tmp, "^fg(" NEURO_THEME_NNOELL_COLOR_GREEN ")Y^fg()", NEURO_DZEN_LOGGER_MAX - strlen(tmp) - 1);
  }
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "MOD", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorWorkspace(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroIndex ws = NeuroCoreGetMonitorStack(m);
  const char *const name = NeuroCoreStackGetName(ws);
  if (!name)
    return;

  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  snprintf(tmp2, NEURO_DZEN_LOGGER_MAX,
      "^fg(" NEURO_THEME_NNOELL_COLOR_GREEN ")%zu^fg(" NEURO_THEME_NNOELL_COLOR_GRAY ")|^fg()%s^fg()",
      (ws + 1) % NeuroCoreGetSize(), name);
  NeuroDzenWrapDzenBox(tmp, tmp2, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "WORKSPACE", &boxpp_nnoell_blue2_);
  strncat(str, tmp, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorCurrTitle(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroClientPtrPtr c = NeuroCoreStackGetCurrClient(NeuroCoreGetMonitorStack(m));
  if (!c)
    return;

  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenWrapDzenBox(tmp, NEURO_CLIENT_PTR(c)->title, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(tmp2, "FOCUS", &boxpp_nnoell_white2b_);
  NeuroDzenWrapClickArea(str, tmp2, &ca_nnoell_title_);
  strncat(str, tmp, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerWorkspaceList(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const NeuroIndex size = NeuroCoreGetSize();
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ], tmp3[ NEURO_DZEN_LOGGER_MAX ],
      tmp4[ NEURO_DZEN_LOGGER_MAX ];
  for (NeuroIndex i = 0U; i < size; ++i) {
    snprintf(tmp, NEURO_DZEN_LOGGER_MAX, "%zu", (i + 1) % size);
    snprintf(tmp3, NEURO_DZEN_LOGGER_MAX, "/usr/bin/xdotool key super+%s", tmp);
    static const NeuroDzenClickableArea wslstCA = { tmp3, tmp3, tmp3, tmp3, tmp3 };
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
    strncat(str, tmp4, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerTime(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ NEURO_DZEN_LOGGER_MAX ];
  const time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, NEURO_DZEN_LOGGER_MAX,
      "%02d^fg("NEURO_THEME_NNOELL_COLOR_GRAY
      "):^fg()%02d^fg("NEURO_THEME_NNOELL_COLOR_GRAY
      "):^fg("NEURO_THEME_NNOELL_COLOR_GREEN")%02d^fg()",
      res.tm_hour, res.tm_min, res.tm_sec);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_white_);
}

void NeuroThemeNnoellLoggerDate(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ NEURO_DZEN_LOGGER_MAX ];
  const time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, NEURO_DZEN_LOGGER_MAX,
      "%d^fg("NEURO_THEME_NNOELL_COLOR_GRAY
      ").^fg()%02d^fg("NEURO_THEME_NNOELL_COLOR_GRAY
      ").^fg("NEURO_THEME_NNOELL_COLOR_BLUE")%02d^fg()",
      res.tm_year+1900, res.tm_mon+1, res.tm_mday);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_white_);
}

void NeuroThemeNnoellLoggerDay(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenLoggerDay(m, tmp);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_white2b_);
}

void NeuroThemeNnoellLoggerCalendar(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenWrapClickArea(tmp, "CALENDAR", &ca_nnoell_calendar_);
  NeuroDzenWrapDzenBox(str, tmp, &boxpp_nnoell_blue_);
}

void NeuroThemeNnoellLoggerDateTime(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ], tmp3[ NEURO_DZEN_LOGGER_MAX ],
      tmp4[ NEURO_DZEN_LOGGER_MAX ];
  NeuroThemeNnoellLoggerCalendar(m, tmp);
  NeuroThemeNnoellLoggerTime(m, tmp2);
  NeuroThemeNnoellLoggerDate(m, tmp3);
  NeuroThemeNnoellLoggerDay(m, tmp4);
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "%s%s%s%s", tmp4, tmp3, tmp2, tmp);
}

void NeuroThemeNnoellLoggerUptime(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenLoggerUptime(m, tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "UPTIME", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorStackSize(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroIndex size = NeuroCoreStackGetSize(NeuroCoreGetMonitorStack(m));
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  snprintf(tmp, NEURO_DZEN_LOGGER_MAX, "%zu", size);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "STACK", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorNumMinimized(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroIndex count = NeuroCoreStackGetMinimizedNum(NeuroCoreGetMonitorStack(m));
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  snprintf(tmp, NEURO_DZEN_LOGGER_MAX, "%zu", count);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "MIN", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerCpu(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenLoggerCpu(m, tmp);
  const char *const p = strchr(tmp, ' ');  // Skip general cpu usage
  if (!p)
    return;
  NeuroDzenWrapDzenBox(tmp2, p + 1, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "CPU", &boxpp_nnoell_gray2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
  tmp[ 0 ] = '\0';
}

void NeuroThemeNnoellLoggerRam(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenLoggerRam(m, tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "RAM", &boxpp_nnoell_gray2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerBattery(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ], tmp3[ NEURO_DZEN_LOGGER_MAX ],
      tmp4[ NEURO_DZEN_LOGGER_MAX ];
  if (!NeuroDzenReadFirstLineFile(tmp, "/sys/class/power_supply/BAT0/capacity"))
    strncpy(tmp, "N/A", NEURO_DZEN_LOGGER_MAX);
  else
    strncat(tmp, "%", NEURO_DZEN_LOGGER_MAX);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue_);
  if (!NeuroDzenReadFirstLineFile(tmp3, "/sys/class/power_supply/BAT0/status"))
    strncpy(tmp3, "AC Conection", NEURO_DZEN_LOGGER_MAX);
  NeuroDzenWrapDzenBox(tmp4, tmp3, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "BATTERY", &boxpp_nnoell_gray2_);
  strncat(tmp2, tmp4, NEURO_DZEN_LOGGER_MAX - strlen(tmp2) - 1);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerWifiStrength(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  NeuroDzenLoggerWifiStrength(m, tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "WIFI", &boxpp_nnoell_gray2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerTemperature(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ], tmp3[ NEURO_DZEN_LOGGER_MAX ],
      tmp4[ NEURO_DZEN_LOGGER_MAX ];
  if (!NeuroDzenReadFirstLineFile(tmp, "/sys/bus/acpi/devices/LNXTHERM:00/thermal_zone/temp"))
    strncpy(tmp, "N/A", NEURO_DZEN_LOGGER_MAX);
  else
    snprintf(tmp, NEURO_DZEN_LOGGER_MAX, "%i°C", atoi(tmp) / 1000);
  if (!NeuroDzenReadFirstLineFile(tmp2, "/sys/bus/acpi/devices/LNXTHERM:01/thermal_zone/temp"))
    strncpy(tmp2, "N/A", NEURO_DZEN_LOGGER_MAX);
  else
    snprintf(tmp2, NEURO_DZEN_LOGGER_MAX, "%i°C", atoi(tmp2) / 1000);
  snprintf(tmp3, NEURO_DZEN_LOGGER_MAX, "%s %s", tmp, tmp2);
  NeuroDzenWrapDzenBox(tmp4, tmp3, &boxpp_nnoell_blue_);
  NeuroDzenWrapDzenBox(str, "TEMP", &boxpp_nnoell_gray2_);
  strncat(str, tmp4, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerScreen(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const NeuroRectangle *const r = NeuroSystemGetScreenRegion();
  static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ];
  snprintf(tmp, NEURO_DZEN_LOGGER_MAX, "%ix%i", r->w, r->h);
  NeuroDzenWrapDzenBox(tmp2, tmp, &boxpp_nnoell_white_);
  NeuroDzenWrapDzenBox(str, "SCREEN", &boxpp_nnoell_blue2_);
  strncat(str, tmp2, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMonitorList(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const NeuroMonitor *const curr_m = NeuroCoreStackGetMonitor(NeuroCoreGetCurrStack());
  for (const NeuroMonitor * m = NeuroMonitorSelectorLast(NULL); m; m = NeuroMonitorSelectorPrev(m)) {
    static char tmp[ NEURO_DZEN_LOGGER_MAX ], tmp2[ NEURO_DZEN_LOGGER_MAX ], tmp3[ NEURO_DZEN_LOGGER_MAX ];
    static NeuroRectangle r = { 0 };
    NeuroGeometryGetIncreasedRectangle(&r, &m->region, m->gaps);
    snprintf(tmp, NEURO_DZEN_LOGGER_MAX, "%ix%i", r.w, r.h);
    if (m == NeuroMonitorSelectorLast(NULL))
      NeuroDzenWrapDzenBox(tmp2, m->name ? m->name : "Unknown", &boxpp_nnoell_blue2_);
    else
      NeuroDzenWrapDzenBox(tmp2, m->name ? m->name : "Unknown", &boxpp_nnoell_blue_);
    strncat(str, tmp2, NEURO_DZEN_LINE_MAX - strlen(str) - 1);
    if (m == curr_m)
      NeuroDzenWrapDzenBox(tmp3, tmp, &boxpp_nnoell_green_);
    else
      NeuroDzenWrapDzenBox(tmp3, tmp, &boxpp_nnoell_white_);
    strncat(str, tmp3, NEURO_DZEN_LINE_MAX - strlen(str) - 1);
  }
}

