//----------------------------------------------------------------------------------------------------------------------
// Module      :  personal
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "personal.h"
#include "stackset.h"
#include "layout.h"
#include "dzenpanel.h"
#include "workspace.h"

// Nnoell paths
#define NNOELL_dzenBoxLeftIconP  "/home/julian/.icons/xbm_icons/boxleft.xbm"
#define NNOELL_dzenBoxLeftIcon2P "/home/julian/.icons/xbm_icons/boxleft2.xbm"
#define NNOELL_dzenBoxRightIconP "/home/julian/.icons/xbm_icons/boxright.xbm"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Nnoell BoxPP
static const BoxPP WhiteBoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorWhiteAltP,
  NNOELL_colorGrayAltP,
  NNOELL_dzenBoxLeftIconP,
  NNOELL_dzenBoxRightIconP,
  14
};

static const BoxPP White2BBoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorBlackP,
  NNOELL_colorWhiteAltP,
  NNOELL_dzenBoxLeftIcon2P,
  NNOELL_dzenBoxRightIconP,
  14
};

static const BoxPP BlueBoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorBlueP,
  NNOELL_colorGrayAltP,
  NNOELL_dzenBoxLeftIconP,
  NNOELL_dzenBoxRightIconP,
  14
};

static const BoxPP Blue2BoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorBlueP,
  NNOELL_colorGrayAltP,
  NNOELL_dzenBoxLeftIcon2P,
  NNOELL_dzenBoxRightIconP,
  14
};

static const BoxPP Blue2BBoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorBlackP,
  NNOELL_colorBlueP,
  NNOELL_dzenBoxLeftIcon2P,
  NNOELL_dzenBoxRightIconP,
  14
};

static const BoxPP Gray2BoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorGrayP,
  NNOELL_colorGrayAltP,
  NNOELL_dzenBoxLeftIcon2P,
  NNOELL_dzenBoxRightIconP,
  14
};

static const BoxPP Green2BBoxPP = {
  NNOELL_colorBlackP,
  NNOELL_colorBlackP,
  NNOELL_colorGreenP,
  NNOELL_dzenBoxLeftIcon2P,
  NNOELL_dzenBoxRightIconP,
  14
};

// Nnoell clickable areas
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

// Nnoell Loggers
void nnoellCurrLayoutLoggerP(char *str) {
  assert(str);
  int ws = getCurrStackSS();
  int idx = getLayoutIdxStackSS(ws);
  const LayoutConf *lc = getCurrLayoutConfStackSS(ws);
  if (lc) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    if (isCurrTogLayoutStackSS(ws))
      snprintf(tmp2, LOGGER_MAX, "^fg(" NNOELL_colorRedP ")%i^fg(" NNOELL_colorGrayP ")|^fg()%s^fg()", idx + 1,
          lc->name);
    else
      snprintf(tmp2, LOGGER_MAX, "^fg(" NNOELL_colorGreenP ")%i^fg(" NNOELL_colorGrayP ")|^fg()%s^fg()", idx + 1,
          lc->name);
    wrapDzenBoxDP(tmp, tmp2, &WhiteBoxPP);
    wrapDzenBoxDP(str, "LAYOUT", &Blue2BoxPP);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrLayoutModLoggerP(char *str) {
  assert(str);
  int ws = getCurrStackSS();
  Layout *l = getCurrLayoutStackSS(ws);
  if (l) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    tmp[ 0 ] = '\0';
    if (l->mod == notModL) {
      strncpy(tmp, "Norm", LOGGER_MAX);
    } else {
      if (l->mod & mirrModL)
        strncat(tmp, "^fg(" NNOELL_colorGreenP ")M^fg()", LOGGER_MAX - strlen(tmp) - 1);
      if (l->mod & reflXModL)
        strncat(tmp, "^fg(" NNOELL_colorGreenP ")X^fg()", LOGGER_MAX - strlen(tmp) - 1);
      if (l->mod & reflYModL)
        strncat(tmp, "^fg(" NNOELL_colorGreenP ")Y^fg()", LOGGER_MAX - strlen(tmp) - 1);
    }
    wrapDzenBoxDP(tmp2, tmp, &WhiteBoxPP);
    wrapDzenBoxDP(str, "MOD", &Blue2BoxPP);
    strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrWSLoggerP(char *str) {
  assert(str);
  int ws = getCurrStackSS();
  const char *name = getNameStackSS(ws);
  if (name) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    snprintf(tmp2, LOGGER_MAX, "^fg(" NNOELL_colorGreenP ")%i^fg(" NNOELL_colorGrayP ")|^fg()%s^fg()",
        (ws + 1) % getSizeSS(), name);
    wrapDzenBoxDP(tmp, tmp2, &WhiteBoxPP);
    wrapDzenBoxDP(str, "WORKSPACE", &Blue2BoxPP);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrTitleLoggerP(char *str) {
  assert(str);
  CliPtr c = getCurrClientStackSS(getCurrStackSS());
  if (c) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    wrapDzenBoxDP(tmp, CLI_GET(c).title, &WhiteBoxPP);
    wrapDzenBoxDP(tmp2, "FOCUS", &White2BBoxPP);
    wrapDzenClickAreaDP(str, tmp2, &titleCA);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrWSListLoggerP(char *str) {
  assert(str);
  int size = getSizeSS();
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  int i;
  for (i = 0; i < size; ++i) {
    snprintf(tmp, LOGGER_MAX, "%i", (i+1) % size);
    snprintf(tmp3, LOGGER_MAX, "/usr/bin/xdotool key super+%s", tmp);
    static const CA wslstCA = { tmp3, tmp3, tmp3, tmp3, tmp3 };
    if (i == getCurrStackSS())
      wrapDzenBoxDP(tmp2, tmp, &Blue2BBoxPP);
    else if (findUrgentClientW(i))
      wrapDzenBoxDP(tmp2, tmp, &Green2BBoxPP);
    else if (!isEmptyStackSS(i))
      wrapDzenBoxDP(tmp2, tmp, &White2BBoxPP);
    else
      wrapDzenBoxDP(tmp2, tmp, &WhiteBoxPP);
    wrapDzenClickAreaDP(tmp4, tmp2, &wslstCA);
    strncat(str, tmp4, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellTimeLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, LOGGER_MAX,
      "%02d^fg("NNOELL_colorGrayP"):^fg()%02d^fg("NNOELL_colorGrayP"):^fg("NNOELL_colorGreenP")%02d^fg()",
      res.tm_hour, res.tm_min, res.tm_sec);
  wrapDzenBoxDP(str, tmp, &WhiteBoxPP);
}

void nnoellDateLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, LOGGER_MAX,
      "%d^fg("NNOELL_colorGrayP").^fg()%02d^fg("NNOELL_colorGrayP").^fg("NNOELL_colorBlueP")%02d^fg()",
      res.tm_year+1900, res.tm_mon+1, res.tm_mday);
  wrapDzenBoxDP(str, tmp, &WhiteBoxPP);
}

void nnoellDayLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  dayLoggerDP(tmp);
  wrapDzenBoxDP(str, tmp, &White2BBoxPP);
}

void nnoellCalendarLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  wrapDzenClickAreaDP(tmp, "CALENDAR", &calendarCA);
  wrapDzenBoxDP(str, tmp, &BlueBoxPP);
}

void nnoellDateTimeLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  nnoellCalendarLoggerP(tmp);
  nnoellTimeLoggerP(tmp2);
  nnoellDateLoggerP(tmp3);
  nnoellDayLoggerP(tmp4);
  snprintf(str, LOGGER_MAX, "%s%s%s%s", tmp4, tmp3, tmp2, tmp);
}

void nnoellUptimeLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  uptimeLoggerDP(tmp);
  wrapDzenBoxDP(tmp2, tmp, &WhiteBoxPP);
  wrapDzenBoxDP(str, "UPTIME", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellCurrSizeStackLoggerP(char *str) {
  assert(str);
  int size = getSizeStackSS(getCurrStackSS());
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  snprintf(tmp, LOGGER_MAX, "%i", size);
  wrapDzenBoxDP(tmp2, tmp, &WhiteBoxPP);
  wrapDzenBoxDP(str, "STACK", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellCurrMinimizedCountLoggerP(char *str) {
  assert(str);
  int count = getMinimizedNumStackSS(getCurrStackSS());
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  snprintf(tmp, LOGGER_MAX, "%i", count);
  wrapDzenBoxDP(tmp2, tmp, &WhiteBoxPP);
  wrapDzenBoxDP(str, "MIN", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellCPUUsageLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  cpuPercUsageLoggerDP(tmp);
  char *p = strchr(tmp, ' ');  // Skip general cpu usage
  if (!p)
    return;
  wrapDzenBoxDP(tmp2, p + 1, &BlueBoxPP);
  wrapDzenBoxDP(str, "CPU", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
  tmp[ 0 ] = '\0';
}

void nnoellMemPercLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  memPercLoggerDP(tmp);
  wrapDzenBoxDP(tmp2, tmp, &BlueBoxPP);
  wrapDzenBoxDP(str, "RAM", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellBatteryLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  if (-1 == readFileDP(tmp, "/sys/class/power_supply/BAT0/capacity"))
    strncpy(tmp, "N/A", LOGGER_MAX);
  else
    strncat(tmp, "%", LOGGER_MAX);
  wrapDzenBoxDP(tmp2, tmp, &BlueBoxPP);
  if (-1 == readFileDP(tmp3, "/sys/class/power_supply/BAT0/status"))
    strncpy(tmp3, "AC Conection", LOGGER_MAX);
  wrapDzenBoxDP(tmp4, tmp3, &WhiteBoxPP);
  wrapDzenBoxDP(str, "BATTERY", &Gray2BoxPP);
  strncat(tmp2, tmp4, LOGGER_MAX - strlen(tmp2) - 1);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellWifiStrengthLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  wifiStrengthDP(tmp);
  wrapDzenBoxDP(tmp2, tmp, &BlueBoxPP);
  wrapDzenBoxDP(str, "WIFI", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellTempLoggerP(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  if (-1 == readFileDP(tmp, "/sys/bus/acpi/devices/LNXTHERM:00/thermal_zone/temp"))
    strncpy(tmp, "N/A", LOGGER_MAX);
  else
    snprintf(tmp, LOGGER_MAX, "%i°C", atoi(tmp) / 1000);
  if (-1 == readFileDP(tmp2, "/sys/bus/acpi/devices/LNXTHERM:01/thermal_zone/temp"))
    strncpy(tmp2, "N/A", LOGGER_MAX);
  else
    snprintf(tmp2, LOGGER_MAX, "%i°C", atoi(tmp2) / 1000);
  snprintf(tmp3, LOGGER_MAX, "%s %s", tmp, tmp2);
  wrapDzenBoxDP(tmp4, tmp3, &BlueBoxPP);
  wrapDzenBoxDP(str, "TEMP", &Gray2BoxPP);
  strncat(str, tmp4, LOGGER_MAX - strlen(str) - 1);
}


