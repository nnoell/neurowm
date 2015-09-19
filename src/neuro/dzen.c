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

// Includes
#include "dzen.h"
#include "system.h"
#include "core.h"

// Defines
#define CPU_FILE_PATH "/proc/stat"
#define CPU_MAX_VALS 10

//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// CPU Calculation
typedef struct CpuInfo CpuInfo;
struct CpuInfo {
  long idle;
  long total;
  int perc;
};

// Dzen
typedef struct PipeInfo PipeInfo;
struct PipeInfo {
  int output;
  pid_t pid;
};

typedef struct PipeInfoPanels PipeInfoPanels;
struct PipeInfoPanels {
  PipeInfo *pi;
  int numPanels;
  pthread_t updateThread;
  pthread_attr_t attr;
  int resetRate;
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// CPU Calculation
static CpuInfo *cpusInfo;
static int numCpus;
static pthread_t threadID;
static pthread_attr_t threadAttr;
static Bool stopUpdateCpuPercWhile = False;

// Dzen
static PipeInfoPanels PIP;
static Bool stopUpdateWhile = False;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// CPU Calculation
static int get_num_cpus(const char *file) {
  assert(file);
  FILE *fd = fopen(file, "r");
  if (fd == NULL)
    NeuroSystemError("get_num_cpus - Could not open file");
  int i = 0;
  char buf[ 256 ];
  while (fgets(buf, sizeof(buf), fd)) {
    if (strncmp(buf, "cpu", 3) != 0)
      break;
    ++i;
  }
  fclose(fd);
  return i;
}

static void get_perc_info(CpuInfo *cpu_info, long *cpu_vals, long prev_idle, long prev_total) {
  assert(cpu_info);
  assert(cpu_vals);
  cpu_info->idle = cpu_vals[ 3 ];
  cpu_info->total = 0L;
  int i;
  for (i = 0; i < CPU_MAX_VALS; ++i)
    cpu_info->total += cpu_vals[ i ];
  long diffIdle = cpu_info->idle - prev_idle;
  long diffTotal = cpu_info->total - prev_total;
  cpu_info->perc = (100 * (diffTotal - diffIdle)) / diffTotal;
}

static void update_cpu_perc(const char *file, int ncpus) {
  assert(file);
  long cpusFileInfo[ ncpus ][ CPU_MAX_VALS ];
  long prevIdle[ ncpus ], prevTotal[ ncpus ];
  memset(prevIdle, 0, sizeof(prevIdle));
  memset(prevTotal, 0, sizeof(prevTotal));

  stopUpdateCpuPercWhile = False;
  while (!stopUpdateCpuPercWhile) {
    FILE *fd = fopen(file, "r");
    if (fd == NULL)
      return;

    int i;
    char buf[ 256 ];
    for (i = 0; i < ncpus; ++i) {
      fgets(buf, sizeof(buf), fd);
      if (EOF == sscanf(buf + 5, "%li %li %li %li %li %li %li %li %li %li",
          cpusFileInfo[ i ] + 0, cpusFileInfo[ i ] + 1,
          cpusFileInfo[ i ] + 2, cpusFileInfo[ i ] + 3,
          cpusFileInfo[ i ] + 4, cpusFileInfo[ i ] + 5,
          cpusFileInfo[ i ] + 6, cpusFileInfo[ i ] + 7,
          cpusFileInfo[ i ] + 8, cpusFileInfo[ i ] + 9))
        return;
      get_perc_info(cpusInfo + i, cpusFileInfo[ i ], prevIdle[ i ], prevTotal[ i ]);
      prevIdle[ i ] = cpusInfo[ i ].idle;
      prevTotal[ i ] = cpusInfo[ i ].total;
    }

    fclose(fd);
    sleep(1);
  }
}

static void *update_cpu_perc_thread(void *args) {
  (void)args;
  update_cpu_perc(CPU_FILE_PATH, numCpus);
  pthread_exit(NULL);
}

static Bool initCpuPercThread() {
  pthread_attr_init(&threadAttr);
  pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
  return pthread_create(&threadID, &threadAttr, update_cpu_perc_thread, NULL);
}

static void stop_cpu_perc_thread() {
  pthread_attr_destroy(&threadAttr);
  stopUpdateCpuPercWhile = True;
  void *status;
  if (pthread_join(threadID, &status))  // Wait
    perror("stopUpdateCpuPercThreadP - Could not join thread");
}

// Dzen
static char **str_to_cmd(char **cmd, char *str, const char *sep) {
  assert(cmd);
  assert(str);
  assert(sep);
  char *token, *saveptr;
  token = strtok_r(str, sep, &saveptr);
  int i;
  for (i = 0; token; ++i) {
    cmd[ i ] = token;
    token = strtok_r(NULL, sep, &saveptr);
  }
  cmd[ i ] = NULL;
  return cmd;
}

static char **get_dzen_cmd(char **cmd, char *line, const DzenFlags *df) {
  assert(cmd);
  assert(line);
  assert(df);
  snprintf(line, DZEN_LINE_MAX, "/usr/bin/dzen2 -x %i -y %i -w %i -h %i -fg %s -bg %s -ta %c -fn %s -e %s %s",
      df->x, df->y, df->w, df->h, df->fgColor, df->bgColor, df->align, df->font, df->event, df->extras);
  return str_to_cmd(cmd, line, " \t\n");
}

static void update_dzen_panel(const DzenPanel *dp, int fd) {
  assert(dp);
  char line[ DZEN_LINE_MAX ] = "\0";
  int i;
  for (i = 0; dp->loggers[ i ]; ++i) {
    char str[ LOGGER_MAX ] = "\0";
    dp->loggers[ i ](str);
    if (i > 0 && str[ 0 ] != '\0')  // Add separator if not first and not empty str
      strncat(line, dp->sep, DZEN_LINE_MAX - strlen(line) - 1);
    strncat(line, str, DZEN_LINE_MAX - strlen(line) - 1);
  }
  strncat(line, "\n", DZEN_LINE_MAX - strlen(line) - 1);  // Line must be '\n' terminated so that dzen shows it
  write(fd, line, strlen(line));
}

static void *update_thread(void *args) {
  (void)args;
  const DzenPanel *dp;
  int i = 0, j;
  stopUpdateWhile = False;
  while (!stopUpdateWhile) {
    for (j = 0; j < PIP.numPanels; ++j) {
      dp = dzenPanelSetS[ j ];
      if (dp->refreshRate == WM_EVENT || dp->refreshRate <= 0)
        continue;
      if (i % dp->refreshRate == 0)
        update_dzen_panel(dp, PIP.pi[ j ].output);
    }
    ++i;
    i %= PIP.resetRate;
    sleep(1);
  }
  pthread_exit(NULL);
}

static Bool init_update_thread() {
  if (PIP.resetRate <= 0)
    return True;
  pthread_attr_init(&PIP.attr);
  pthread_attr_setdetachstate(&PIP.attr, PTHREAD_CREATE_JOINABLE);
  return pthread_create(&PIP.updateThread, &PIP.attr, update_thread, NULL) == 0;
}

static void stop_update_thread() {
  if (PIP.resetRate <= 0)
    return;
  pthread_attr_destroy(&PIP.attr);
  stopUpdateWhile = True;
  void *status;
  if (pthread_join(PIP.updateThread, &status))  // Wait
    perror("stopUpdateThreadDP - Could not join thread");
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Dzen
Bool NeuroDzenInit() {
  PIP.numPanels = NeuroTypeArrayLength((const void const *const *)dzenPanelSetS);
  PIP.pi = (PipeInfo *)calloc(PIP.numPanels, sizeof(PipeInfo));
  PIP.updateThread = -1;
  PIP.resetRate = 1;
  if (!PIP.pi)
    return False;
  const DzenPanel *dp;
  int i;
  for (i = 0; i < PIP.numPanels; ++i) {
    char *dzenCmd[ DZEN_ARGS_MAX ];
    char line[ DZEN_LINE_MAX ];
    dp = dzenPanelSetS[ i ];
    if (dp->refreshRate > PIP.resetRate)
      PIP.resetRate *= dp->refreshRate;
    get_dzen_cmd(dzenCmd, line, dp->df);
    PIP.pi[ i ].output = NeuroSystemSpawnPipe((const char *const *)dzenCmd, &(PIP.pi[ i ].pid));
    if (PIP.pi[ i ].output == -1)
      return False;
  }
  if (!init_update_thread())
    NeuroSystemError("NeuroDzenInit - Could not init thread to update panels");
  NeuroDzenUpdate(False);
  return True;
}

void NeuroDzenStop() {
  stop_update_thread();
  int i;
  for (i = 0; i < PIP.numPanels; ++i)
    if (kill(PIP.pi[ i ].pid, SIGTERM) == -1)
      perror("NeuroDzenStop - Could not kill panels");
  free(PIP.pi);
  PIP.pi = NULL;
}

void NeuroDzenUpdate(Bool onlyEvent) {
  const DzenPanel *dp;
  int i;
  for (i=0; i < PIP.numPanels; ++i) {
    dp = dzenPanelSetS[ i ];
    if (onlyEvent) {
      if (dp->refreshRate == WM_EVENT || dp->refreshRate <= 0)
        update_dzen_panel(dp, PIP.pi[ i ].output);
    } else {
      update_dzen_panel(dp, PIP.pi[ i ].output);
    }
  }
}

void NeuroDzenStartCpuCalc() {
  numCpus = get_num_cpus(CPU_FILE_PATH);
  cpusInfo = (CpuInfo *)calloc(numCpus, sizeof(CpuInfo));
  if (!cpusInfo)
    NeuroSystemError("NeuroDzenStartCpuCalc - Could not alloc cpusInfo");
  if (initCpuPercThread())
    NeuroSystemError("NeuroDzenStartCpuCalc - Could not init thread to update cpus");
}

void NeuroDzenStopCpuCalc() {
  stop_cpu_perc_thread();
  free(cpusInfo);
  cpusInfo = NULL;
}

void NeuroDzenWrapDzenBox(char *dst, const char *src, const BoxPP *b) {
  assert(dst);
  assert(src);
  assert(b);
  snprintf(dst, LOGGER_MAX,
      "^fg(%s)^i(%s)^ib(1)^r(1920x%i)^p(-1920x)^fg(%s)%s^fg(%s)^i(%s)^fg(%s)^r(1920x%i)^p(-1920)^fg()^ib(0)",
      b->boxColor, b->leftIcon, b->boxHeight, b->fgColor, src, b->boxColor, b->rightIcon, b->bgColor, b->boxHeight);
}

void NeuroDzenWrapClickArea(char *dst, const char *src, const CA *ca) {
  assert(dst);
  assert(src);
  assert(ca);
  snprintf(dst, LOGGER_MAX, "^ca(1,%s)^ca(2,%s)^ca(3,%s)^ca(4,%s)^ca(5,%s)%s^ca()^ca()^ca()^ca()^ca()",
      ca->leftClick, ca->middleClick, ca->rightClick, ca->wheelUp, ca->wheelDown, src);
}

int NeuroDzenReadFile(char *buf, const char *fileName) {
  assert(buf);
  assert(fileName);
  FILE *fd;
  fd = fopen(fileName, "r");
  if (!fd)
    return -1;
  fgets(buf, LOGGER_MAX, fd);
  char *last = buf + strlen(buf) - 1;
  if (*last == '\n')
    *last = '\0';
  fclose(fd);
  return 0;
}

// Loggers
void NeuroDzenLoggerTime(char *str) {
  assert(str);
  struct tm res;
  time_t t = time(NULL);
  localtime_r(&t, &res);
  snprintf(str, LOGGER_MAX, "%02d:%02d:%02d", res.tm_hour, res.tm_min, res.tm_sec);
}


void NeuroDzenLoggerDate(char *str) {
  assert(str);
  struct tm res;
  time_t t = time(NULL);
  localtime_r(&t, &res);
  snprintf(str, LOGGER_MAX, "%d.%02d.%02d", res.tm_year + 1900, res.tm_mon + 1, res.tm_mday);
}

void NeuroDzenLoggerDay(char *str) {
  assert(str);
  struct tm res;
  time_t t = time(NULL);
  localtime_r(&t, &res);
  switch (res.tm_wday) {
    case 1:  { strncpy(str, "Monday", LOGGER_MAX); break; }
    case 2:  { strncpy(str, "Tuesday", LOGGER_MAX); break; }
    case 3:  { strncpy(str, "Wednesday", LOGGER_MAX); break; }
    case 4:  { strncpy(str, "Thursday", LOGGER_MAX); break; }
    case 5:  { strncpy(str, "Friday", LOGGER_MAX); break; }
    case 6:  { strncpy(str, "Saturday", LOGGER_MAX); break; }
    case 0:  { strncpy(str, "Sunday", LOGGER_MAX); break; }
    default: { strncpy(str, "Unknown", LOGGER_MAX); break; }
  }
}

void NeuroDzenLoggerUptime(char *str) {
  assert(str);
  struct sysinfo info;
  sysinfo(&info);
  int hours = (int)(info.uptime / 3600UL);
  int hrest = (int)(info.uptime % 3600UL);
  int minutes = hrest / 60;
  int seconds = hrest % 60;
  snprintf(str, LOGGER_MAX, "%ih %im %is", hours, minutes, seconds);
}

void NeuroDzenLoggerCpu(char *str) {
  assert(str);
  char buf[ LOGGER_MAX ];
  int i;
  for (i = 0; i < numCpus; ++i) {
    snprintf(buf, LOGGER_MAX, "%i%% ", cpusInfo[ i ].perc);
    strncat(str, buf, LOGGER_MAX - strlen(str) - 1);
  }
  str[ strlen(str) - 1 ] = '\0';
}

void NeuroDzenLoggerRam(char *str) {
  assert(str);
  char buf[ LOGGER_MAX ];
  unsigned long memTotal = 0UL, memAvailable = 0UL;
  FILE *fd = fopen("/proc/meminfo", "r");
  if (!fd)
    return;
  fgets(buf, LOGGER_MAX, fd);
  sscanf(buf, "%*s %lu\n", &memTotal);
  fgets(buf, LOGGER_MAX, fd);
  fgets(buf, LOGGER_MAX, fd);
  sscanf(buf, "%*s %lu\n", &memAvailable);
  unsigned long memUsed = memTotal - memAvailable;
  int perc = (int)((memUsed * 100UL) / memTotal);
  snprintf(str, LOGGER_MAX, "%i%% %luMB", perc, memUsed / 1024UL);
  fclose(fd);
}

void NeuroDzenLoggerWifiStrength(char *str) {
  assert(str);
  char buf[ LOGGER_MAX ];
  FILE *fd = fopen("/proc/net/wireless", "r");
  if (!fd)
    return;
  int strength = 0, tmp = 0;
  fgets(buf, LOGGER_MAX, fd);
  fgets(buf, LOGGER_MAX, fd);
  fgets(buf, LOGGER_MAX, fd);
  sscanf(buf, "%*s %i   %i\n", &tmp, &strength);
  snprintf(str, LOGGER_MAX, "%i%%", strength);
  fclose(fd);
}

void NeuroDzenLoggerCurrWorkspace(char *str) {
  assert(str);
  const char *name = NeuroCoreStackGetName(NeuroCoreGetCurrStack());
  if (name)
    strncpy(str, name, LOGGER_MAX);
}

void NeuroDzenLoggerCurrLayout(char *str) {
  assert(str);
  int ws = NeuroCoreGetCurrStack();
  const LayoutConf *lc = NeuroCoreStackGetCurrLayoutConf(ws);
  if (lc)
    strncpy(str, lc->name, LOGGER_MAX);
}

void NeuroDzenLoggerCurrTitle(char *str) {
  assert(str);
  ClientPtrPtr c = NeuroCoreStackGetCurrClient(NeuroCoreGetCurrStack());
  if (c)
    strncpy(str, CLI_GET(c).title, LOGGER_MAX);
}

