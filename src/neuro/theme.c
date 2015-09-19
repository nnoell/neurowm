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


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell theme BoxPP
static const BoxPP WhiteBoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP White2BBoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorWhiteAlt,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP BlueBoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlue,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP Blue2BoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlue,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP Blue2BBoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlue,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP Gray2BoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorGray,
  NeuroThemeNnoellColorGrayAlt,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

static const BoxPP Green2BBoxPP = {
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorBlack,
  NeuroThemeNnoellColorGreen,
  NeuroThemeNnoellDzenBoxLeftIcon2,
  NeuroThemeNnoellDzenBoxRightIcon,
  14
};

// Nnoell theme clickable areas
static const CA titleCA = {
  "/usr/bin/xdotool key super+j",
  "/usr/bin/xdotool key super+Tab",
  "/usr/bin/xdotool key super+k",
  "/usr/bin/xdotool key super+shift+j",
  "/usr/bin/xdotool key super+shift+k"
};

static const CA calendarCA = {
  "/home/julian/bin/dzencal.sh",
  "/home/julian/bin/dzencal.sh",
  "/home/julian/bin/dzencal.sh",
  "/home/julian/bin/dzencal.sh",
  "/home/julian/bin/dzencal.sh"
};


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell theme loggers
void NeuroThemeNnoellLoggerCurrLayout(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  int idx = NeuroCoreStackGetLayoutIdx(ws);
  const LayoutConf *lc = NeuroCoreStackGetCurrLayoutConf(ws);
  if (lc) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    if (NeuroCoreStackIsCurrToggledLayout(ws))
      snprintf(tmp2, LOGGER_MAX, "^fg(" NeuroThemeNnoellColorRed ")%i^fg(" NeuroThemeNnoellColorGray ")|^fg()%s^fg()", idx + 1,
          lc->name);
    else
      snprintf(tmp2, LOGGER_MAX, "^fg(" NeuroThemeNnoellColorGreen ")%i^fg(" NeuroThemeNnoellColorGray ")|^fg()%s^fg()", idx + 1,
          lc->name);
    NeuroDzenWrapDzenBox(tmp, tmp2, &WhiteBoxPP);
    NeuroDzenWrapDzenBox(str, "LAYOUT", &Blue2BoxPP);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerLayoutMod(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  Layout *l = NeuroCoreStackGetCurrLayout(ws);
  if (l) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    tmp[ 0 ] = '\0';
    if (l->mod == nullModL) {
      strncpy(tmp, "Norm", LOGGER_MAX);
    } else {
      if (l->mod & mirrModL)
        strncat(tmp, "^fg(" NeuroThemeNnoellColorGreen ")M^fg()", LOGGER_MAX - strlen(tmp) - 1);
      if (l->mod & reflXModL)
        strncat(tmp, "^fg(" NeuroThemeNnoellColorGreen ")X^fg()", LOGGER_MAX - strlen(tmp) - 1);
      if (l->mod & reflYModL)
        strncat(tmp, "^fg(" NeuroThemeNnoellColorGreen ")Y^fg()", LOGGER_MAX - strlen(tmp) - 1);
    }
    NeuroDzenWrapDzenBox(tmp2, tmp, &WhiteBoxPP);
    NeuroDzenWrapDzenBox(str, "MOD", &Blue2BoxPP);
    strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerCurrWorkspace(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  const char *name = NeuroCoreStackGetName(ws);
  if (name) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    snprintf(tmp2, LOGGER_MAX, "^fg(" NeuroThemeNnoellColorGreen ")%i^fg(" NeuroThemeNnoellColorGray ")|^fg()%s^fg()",
        (ws + 1) % NeuroCoreGetSize(), name);
    NeuroDzenWrapDzenBox(tmp, tmp2, &WhiteBoxPP);
    NeuroDzenWrapDzenBox(str, "WORKSPACE", &Blue2BoxPP);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerCurrTitle(char *str) {
  assert(str);
  ClientPtrPtr c = NeuroCoreStackGetCurrClient(NeuroCoreGetCurrStack());
  if (c) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    NeuroDzenWrapDzenBox(tmp, CLI_GET(c).title, &WhiteBoxPP);
    NeuroDzenWrapDzenBox(tmp2, "FOCUS", &White2BBoxPP);
    NeuroDzenWrapClickArea(str, tmp2, &titleCA);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerWorkspaceList(char *str) {
  assert(str);
  int size = NeuroCoreGetSize();
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  int i;
  for (i = 0; i < size; ++i) {
    snprintf(tmp, LOGGER_MAX, "%i", (i+1) % size);
    snprintf(tmp3, LOGGER_MAX, "/usr/bin/xdotool key super+%s", tmp);
    static const CA wslstCA = { tmp3, tmp3, tmp3, tmp3, tmp3 };
    if (i == NeuroCoreGetCurrStack())
      NeuroDzenWrapDzenBox(tmp2, tmp, &Blue2BBoxPP);
    else if (findUrgentClientW(i))
      NeuroDzenWrapDzenBox(tmp2, tmp, &Green2BBoxPP);
    else if (!NeuroCoreStackIsEmpty(i))
      NeuroDzenWrapDzenBox(tmp2, tmp, &White2BBoxPP);
    else
      NeuroDzenWrapDzenBox(tmp2, tmp, &WhiteBoxPP);
    NeuroDzenWrapClickArea(tmp4, tmp2, &wslstCA);
    strncat(str, tmp4, LOGGER_MAX - strlen(str) - 1);
  }
}

void NeuroThemeNnoellLoggerTime(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, LOGGER_MAX,
      "%02d^fg("NeuroThemeNnoellColorGray"):^fg()%02d^fg("NeuroThemeNnoellColorGray"):^fg("NeuroThemeNnoellColorGreen")%02d^fg()",
      res.tm_hour, res.tm_min, res.tm_sec);
  NeuroDzenWrapDzenBox(str, tmp, &WhiteBoxPP);
}

void NeuroThemeNnoellLoggerDate(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, LOGGER_MAX,
      "%d^fg("NeuroThemeNnoellColorGray").^fg()%02d^fg("NeuroThemeNnoellColorGray").^fg("NeuroThemeNnoellColorBlue")%02d^fg()",
      res.tm_year+1900, res.tm_mon+1, res.tm_mday);
  NeuroDzenWrapDzenBox(str, tmp, &WhiteBoxPP);
}

void NeuroThemeNnoellLoggerDay(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  NeuroDzenLoggerDay(tmp);
  NeuroDzenWrapDzenBox(str, tmp, &White2BBoxPP);
}

void NeuroThemeNnoellLoggerCalendar(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  NeuroDzenWrapClickArea(tmp, "CALENDAR", &calendarCA);
  NeuroDzenWrapDzenBox(str, tmp, &BlueBoxPP);
}

void NeuroThemeNnoellLoggerDateTime(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  NeuroThemeNnoellLoggerCalendar(tmp);
  NeuroThemeNnoellLoggerTime(tmp2);
  NeuroThemeNnoellLoggerDate(tmp3);
  NeuroThemeNnoellLoggerDay(tmp4);
  snprintf(str, LOGGER_MAX, "%s%s%s%s", tmp4, tmp3, tmp2, tmp);
}

void NeuroThemeNnoellLoggerUptime(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  NeuroDzenLoggerUptime(tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &WhiteBoxPP);
  NeuroDzenWrapDzenBox(str, "UPTIME", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerStackSize(char *str) {
  assert(str);
  int size = NeuroCoreStackGetSize(NeuroCoreGetCurrStack());
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  snprintf(tmp, LOGGER_MAX, "%i", size);
  NeuroDzenWrapDzenBox(tmp2, tmp, &WhiteBoxPP);
  NeuroDzenWrapDzenBox(str, "STACK", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerMinimizedCount(char *str) {
  assert(str);
  int count = NeuroCoreStackGetMinimizedNum(NeuroCoreGetCurrStack());
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  snprintf(tmp, LOGGER_MAX, "%i", count);
  NeuroDzenWrapDzenBox(tmp2, tmp, &WhiteBoxPP);
  NeuroDzenWrapDzenBox(str, "MIN", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerCpu(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  NeuroDzenLoggerCpu(tmp);
  char *p = strchr(tmp, ' ');  // Skip general cpu usage
  if (!p)
    return;
  NeuroDzenWrapDzenBox(tmp2, p + 1, &BlueBoxPP);
  NeuroDzenWrapDzenBox(str, "CPU", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
  tmp[ 0 ] = '\0';
}

void NeuroThemeNnoellLoggerRam(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  NeuroDzenLoggerRam(tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &BlueBoxPP);
  NeuroDzenWrapDzenBox(str, "RAM", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerBattery(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  if (-1 == NeuroDzenReadFile(tmp, "/sys/class/power_supply/BAT0/capacity"))
    strncpy(tmp, "N/A", LOGGER_MAX);
  else
    strncat(tmp, "%", LOGGER_MAX);
  NeuroDzenWrapDzenBox(tmp2, tmp, &BlueBoxPP);
  if (-1 == NeuroDzenReadFile(tmp3, "/sys/class/power_supply/BAT0/status"))
    strncpy(tmp3, "AC Conection", LOGGER_MAX);
  NeuroDzenWrapDzenBox(tmp4, tmp3, &WhiteBoxPP);
  NeuroDzenWrapDzenBox(str, "BATTERY", &Gray2BoxPP);
  strncat(tmp2, tmp4, LOGGER_MAX - strlen(tmp2) - 1);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerWifiStrength(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  NeuroDzenLoggerWifiStrength(tmp);
  NeuroDzenWrapDzenBox(tmp2, tmp, &BlueBoxPP);
  NeuroDzenWrapDzenBox(str, "WIFI", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void NeuroThemeNnoellLoggerTemperature(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  if (-1 == NeuroDzenReadFile(tmp, "/sys/bus/acpi/devices/LNXTHERM:00/thermal_zone/temp"))
    strncpy(tmp, "N/A", LOGGER_MAX);
  else
    snprintf(tmp, LOGGER_MAX, "%i°C", atoi(tmp) / 1000);
  if (-1 == NeuroDzenReadFile(tmp2, "/sys/bus/acpi/devices/LNXTHERM:01/thermal_zone/temp"))
    strncpy(tmp2, "N/A", LOGGER_MAX);
  else
    snprintf(tmp2, LOGGER_MAX, "%i°C", atoi(tmp2) / 1000);
  snprintf(tmp3, LOGGER_MAX, "%s %s", tmp, tmp2);
  NeuroDzenWrapDzenBox(tmp4, tmp3, &BlueBoxPP);
  NeuroDzenWrapDzenBox(str, "TEMP", &Gray2BoxPP);
  strncat(str, tmp4, LOGGER_MAX - strlen(str) - 1);
}


