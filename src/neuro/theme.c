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
void nnoellCurrLayoutLoggerT(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  int idx = NeuroCoreStackGetLayoutIdx(ws);
  const LayoutConf *lc = NeuroCoreStackGetCurrLayoutConf(ws);
  if (lc) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    if (NeuroCoreStackIsCurrToggledLayout(ws))
      snprintf(tmp2, LOGGER_MAX, "^fg(" NNOELL_colorRedP ")%i^fg(" NNOELL_colorGrayP ")|^fg()%s^fg()", idx + 1,
          lc->name);
    else
      snprintf(tmp2, LOGGER_MAX, "^fg(" NNOELL_colorGreenP ")%i^fg(" NNOELL_colorGrayP ")|^fg()%s^fg()", idx + 1,
          lc->name);
    wrapDzenBoxD(tmp, tmp2, &WhiteBoxPP);
    wrapDzenBoxD(str, "LAYOUT", &Blue2BoxPP);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrLayoutModLoggerT(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  Layout *l = NeuroCoreStackGetCurrLayout(ws);
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
    wrapDzenBoxD(tmp2, tmp, &WhiteBoxPP);
    wrapDzenBoxD(str, "MOD", &Blue2BoxPP);
    strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrWorkspaceLoggerT(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  const char *name = NeuroCoreStackGetName(ws);
  if (name) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    snprintf(tmp2, LOGGER_MAX, "^fg(" NNOELL_colorGreenP ")%i^fg(" NNOELL_colorGrayP ")|^fg()%s^fg()",
        (ws + 1) % NeuroCoreGetSize(), name);
    wrapDzenBoxD(tmp, tmp2, &WhiteBoxPP);
    wrapDzenBoxD(str, "WORKSPACE", &Blue2BoxPP);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrTitleLoggerT(char *str) {
  assert(str);
  ClientPtrPtr c = NeuroCoreStackGetCurrClient(NeuroCoreGetCurrStack());
  if (c) {
    static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
    wrapDzenBoxD(tmp, CLI_GET(c).title, &WhiteBoxPP);
    wrapDzenBoxD(tmp2, "FOCUS", &White2BBoxPP);
    wrapDzenClickAreaD(str, tmp2, &titleCA);
    strncat(str, tmp, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellCurrWorkspaceListLoggerT(char *str) {
  assert(str);
  int size = NeuroCoreGetSize();
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  int i;
  for (i = 0; i < size; ++i) {
    snprintf(tmp, LOGGER_MAX, "%i", (i+1) % size);
    snprintf(tmp3, LOGGER_MAX, "/usr/bin/xdotool key super+%s", tmp);
    static const CA wslstCA = { tmp3, tmp3, tmp3, tmp3, tmp3 };
    if (i == NeuroCoreGetCurrStack())
      wrapDzenBoxD(tmp2, tmp, &Blue2BBoxPP);
    else if (findUrgentClientW(i))
      wrapDzenBoxD(tmp2, tmp, &Green2BBoxPP);
    else if (!NeuroCoreStackIsEmpty(i))
      wrapDzenBoxD(tmp2, tmp, &White2BBoxPP);
    else
      wrapDzenBoxD(tmp2, tmp, &WhiteBoxPP);
    wrapDzenClickAreaD(tmp4, tmp2, &wslstCA);
    strncat(str, tmp4, LOGGER_MAX - strlen(str) - 1);
  }
}

void nnoellTimeLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, LOGGER_MAX,
      "%02d^fg("NNOELL_colorGrayP"):^fg()%02d^fg("NNOELL_colorGrayP"):^fg("NNOELL_colorGreenP")%02d^fg()",
      res.tm_hour, res.tm_min, res.tm_sec);
  wrapDzenBoxD(str, tmp, &WhiteBoxPP);
}

void nnoellDateLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  time_t t = time(NULL);
  struct tm res;
  localtime_r(&t, &res);
  snprintf(tmp, LOGGER_MAX,
      "%d^fg("NNOELL_colorGrayP").^fg()%02d^fg("NNOELL_colorGrayP").^fg("NNOELL_colorBlueP")%02d^fg()",
      res.tm_year+1900, res.tm_mon+1, res.tm_mday);
  wrapDzenBoxD(str, tmp, &WhiteBoxPP);
}

void nnoellDayLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  dayLoggerD(tmp);
  wrapDzenBoxD(str, tmp, &White2BBoxPP);
}

void nnoellCalendarLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ];
  wrapDzenClickAreaD(tmp, "CALENDAR", &calendarCA);
  wrapDzenBoxD(str, tmp, &BlueBoxPP);
}

void nnoellDateTimeLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  nnoellCalendarLoggerT(tmp);
  nnoellTimeLoggerT(tmp2);
  nnoellDateLoggerT(tmp3);
  nnoellDayLoggerT(tmp4);
  snprintf(str, LOGGER_MAX, "%s%s%s%s", tmp4, tmp3, tmp2, tmp);
}

void nnoellUptimeLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  uptimeLoggerD(tmp);
  wrapDzenBoxD(tmp2, tmp, &WhiteBoxPP);
  wrapDzenBoxD(str, "UPTIME", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellCurrStackSizeLoggerT(char *str) {
  assert(str);
  int size = NeuroCoreStackGetSize(NeuroCoreGetCurrStack());
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  snprintf(tmp, LOGGER_MAX, "%i", size);
  wrapDzenBoxD(tmp2, tmp, &WhiteBoxPP);
  wrapDzenBoxD(str, "STACK", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellCurrMinimizedCountLoggerT(char *str) {
  assert(str);
  int count = NeuroCoreStackGetMinimizedNum(NeuroCoreGetCurrStack());
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  snprintf(tmp, LOGGER_MAX, "%i", count);
  wrapDzenBoxD(tmp2, tmp, &WhiteBoxPP);
  wrapDzenBoxD(str, "MIN", &Blue2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellCpuUsageLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  cpuPercUsageLoggerD(tmp);
  char *p = strchr(tmp, ' ');  // Skip general cpu usage
  if (!p)
    return;
  wrapDzenBoxD(tmp2, p + 1, &BlueBoxPP);
  wrapDzenBoxD(str, "CPU", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
  tmp[ 0 ] = '\0';
}

void nnoellRamPercLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  ramPercLoggerD(tmp);
  wrapDzenBoxD(tmp2, tmp, &BlueBoxPP);
  wrapDzenBoxD(str, "RAM", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellBatteryLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  if (-1 == readFileD(tmp, "/sys/class/power_supply/BAT0/capacity"))
    strncpy(tmp, "N/A", LOGGER_MAX);
  else
    strncat(tmp, "%", LOGGER_MAX);
  wrapDzenBoxD(tmp2, tmp, &BlueBoxPP);
  if (-1 == readFileD(tmp3, "/sys/class/power_supply/BAT0/status"))
    strncpy(tmp3, "AC Conection", LOGGER_MAX);
  wrapDzenBoxD(tmp4, tmp3, &WhiteBoxPP);
  wrapDzenBoxD(str, "BATTERY", &Gray2BoxPP);
  strncat(tmp2, tmp4, LOGGER_MAX - strlen(tmp2) - 1);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellWifiStrengthLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ];
  wifiStrengthD(tmp);
  wrapDzenBoxD(tmp2, tmp, &BlueBoxPP);
  wrapDzenBoxD(str, "WIFI", &Gray2BoxPP);
  strncat(str, tmp2, LOGGER_MAX - strlen(str) - 1);
}

void nnoellTemperatureLoggerT(char *str) {
  assert(str);
  static char tmp[ LOGGER_MAX ], tmp2[ LOGGER_MAX ], tmp3[ LOGGER_MAX ], tmp4[ LOGGER_MAX ];
  if (-1 == readFileD(tmp, "/sys/bus/acpi/devices/LNXTHERM:00/thermal_zone/temp"))
    strncpy(tmp, "N/A", LOGGER_MAX);
  else
    snprintf(tmp, LOGGER_MAX, "%i°C", atoi(tmp) / 1000);
  if (-1 == readFileD(tmp2, "/sys/bus/acpi/devices/LNXTHERM:01/thermal_zone/temp"))
    strncpy(tmp2, "N/A", LOGGER_MAX);
  else
    snprintf(tmp2, LOGGER_MAX, "%i°C", atoi(tmp2) / 1000);
  snprintf(tmp3, LOGGER_MAX, "%s %s", tmp, tmp2);
  wrapDzenBoxD(tmp4, tmp3, &BlueBoxPP);
  wrapDzenBoxD(str, "TEMP", &Gray2BoxPP);
  strncat(str, tmp4, LOGGER_MAX - strlen(str) - 1);
}


