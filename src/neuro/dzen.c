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
static CpuInfo *cpus_info_;
static int cpus_num_;
static pthread_t cpu_thread_id_;
static pthread_attr_t cpu_thread_attr_;
static Bool cpu_stop_update_while_ = False;

// Dzen
static PipeInfoPanels dzen_pipe_info_panels_;
static Bool dzen_stop_update_while_ = False;


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

  cpu_stop_update_while_ = False;
  while (!cpu_stop_update_while_) {
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
      get_perc_info(cpus_info_ + i, cpusFileInfo[ i ], prevIdle[ i ], prevTotal[ i ]);
      prevIdle[ i ] = cpus_info_[ i ].idle;
      prevTotal[ i ] = cpus_info_[ i ].total;
    }

    fclose(fd);
    sleep(1);
  }
}

static void *update_cpu_perc_thread(void *args) {
  (void)args;
  update_cpu_perc(CPU_FILE_PATH, cpus_num_);
  pthread_exit(NULL);
}

static Bool initCpuPercThread() {
  pthread_attr_init(&cpu_thread_attr_);
  pthread_attr_setdetachstate(&cpu_thread_attr_, PTHREAD_CREATE_JOINABLE);
  return pthread_create(&cpu_thread_id_, &cpu_thread_attr_, update_cpu_perc_thread, NULL);
}

static void stop_cpu_perc_thread() {
  pthread_attr_destroy(&cpu_thread_attr_);
  cpu_stop_update_while_ = True;
  void *status;
  if (pthread_join(cpu_thread_id_, &status))  // Wait
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
  dzen_stop_update_while_ = False;
  while (!dzen_stop_update_while_) {
    for (j = 0; j < dzen_pipe_info_panels_.numPanels; ++j) {
      dp = dzenPanelSetS[ j ];
      if (dp->refreshRate == WM_EVENT || dp->refreshRate <= 0)
        continue;
      if (i % dp->refreshRate == 0)
        update_dzen_panel(dp, dzen_pipe_info_panels_.pi[ j ].output);
    }
    ++i;
    i %= dzen_pipe_info_panels_.resetRate;
    sleep(1);
  }
  pthread_exit(NULL);
}

static Bool init_update_thread() {
  if (dzen_pipe_info_panels_.resetRate <= 0)
    return True;
  pthread_attr_init(&dzen_pipe_info_panels_.attr);
  pthread_attr_setdetachstate(&dzen_pipe_info_panels_.attr, PTHREAD_CREATE_JOINABLE);
  return pthread_create(&dzen_pipe_info_panels_.updateThread, &dzen_pipe_info_panels_.attr, update_thread, NULL) == 0;
}

static void stop_update_thread() {
  if (dzen_pipe_info_panels_.resetRate <= 0)
    return;
  pthread_attr_destroy(&dzen_pipe_info_panels_.attr);
  dzen_stop_update_while_ = True;
  void *status;
  if (pthread_join(dzen_pipe_info_panels_.updateThread, &status))  // Wait
    perror("stopUpdateThreadDP - Could not join thread");
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Dzen
Bool NeuroDzenInit() {
  dzen_pipe_info_panels_.numPanels = NeuroTypeArrayLength((const void const *const *)dzenPanelSetS);
  dzen_pipe_info_panels_.pi = (PipeInfo *)calloc(dzen_pipe_info_panels_.numPanels, sizeof(PipeInfo));
  dzen_pipe_info_panels_.updateThread = -1;
  dzen_pipe_info_panels_.resetRate = 1;
  if (!dzen_pipe_info_panels_.pi)
    return False;
  const DzenPanel *dp;
  int i;
  for (i = 0; i < dzen_pipe_info_panels_.numPanels; ++i) {
    char *dzenCmd[ DZEN_ARGS_MAX ];
    char line[ DZEN_LINE_MAX ];
    dp = dzenPanelSetS[ i ];
    if (dp->refreshRate > dzen_pipe_info_panels_.resetRate)
      dzen_pipe_info_panels_.resetRate *= dp->refreshRate;
    get_dzen_cmd(dzenCmd, line, dp->df);
    dzen_pipe_info_panels_.pi[ i ].output = NeuroSystemSpawnPipe((const char *const *)dzenCmd,
        &(dzen_pipe_info_panels_.pi[ i ].pid));
    if (dzen_pipe_info_panels_.pi[ i ].output == -1)
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
  for (i = 0; i < dzen_pipe_info_panels_.numPanels; ++i)
    if (kill(dzen_pipe_info_panels_.pi[ i ].pid, SIGTERM) == -1)
      perror("NeuroDzenStop - Could not kill panels");
  free(dzen_pipe_info_panels_.pi);
  dzen_pipe_info_panels_.pi = NULL;
}

void NeuroDzenUpdate(Bool onlyEvent) {
  const DzenPanel *dp;
  int i;
  for (i=0; i < dzen_pipe_info_panels_.numPanels; ++i) {
    dp = dzenPanelSetS[ i ];
    if (onlyEvent) {
      if (dp->refreshRate == WM_EVENT || dp->refreshRate <= 0)
        update_dzen_panel(dp, dzen_pipe_info_panels_.pi[ i ].output);
    } else {
      update_dzen_panel(dp, dzen_pipe_info_panels_.pi[ i ].output);
    }
  }
}

void NeuroDzenStartCpuCalc() {
  cpus_num_ = get_num_cpus(CPU_FILE_PATH);
  cpus_info_ = (CpuInfo *)calloc(cpus_num_, sizeof(CpuInfo));
  if (!cpus_info_)
    NeuroSystemError("NeuroDzenStartCpuCalc - Could not alloc cpus_info_");
  if (initCpuPercThread())
    NeuroSystemError("NeuroDzenStartCpuCalc - Could not init thread to update cpus");
}

void NeuroDzenStopCpuCalc() {
  stop_cpu_perc_thread();
  free(cpus_info_);
  cpus_info_ = NULL;
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
  for (i = 0; i < cpus_num_; ++i) {
    snprintf(buf, LOGGER_MAX, "%i%% ", cpus_info_[ i ].perc);
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

