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
#include "config.h"
#include "core.h"
#include "monitor.h"
#include "geometry.h"

// Defines
#define CPU_FILE_PATH "/proc/stat"
#define CPU_MAX_VALS 10


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// CPU Calculation
typedef struct CpuInfo CpuInfo;
struct CpuInfo {
  uint64_t idle;
  uint64_t total;
  uint32_t perc;
};

typedef struct CpuCalcRefreshInfo CpuCalcRefreshInfo;
struct CpuCalcRefreshInfo {
  pthread_t thread;            // Thread that calculates cpu percent
  pthread_mutex_t wait_mutex;  // Interval wait mutex
  pthread_cond_t wait_cond;    // Interval wait condition variable
  CpuInfo *cpu_info;
  NeuroIndex num_cpus;
};

// Dzen
typedef struct PipeInfo PipeInfo;
struct PipeInfo {
  const NeuroDzenPanel *dzen_panel;
  const NeuroMonitor *monitor;
  int output;
  pid_t pid;
};

typedef struct DzenRefreshInfo DzenRefreshInfo;
struct DzenRefreshInfo {
  pthread_t thread;            // Thread that displays info on the panels
  pthread_mutex_t sync_mutex;  // Sync control mutex
  pthread_mutex_t wait_mutex;  // Interval wait mutex
  pthread_cond_t wait_cond;    // Interval wait conditional variable
  const NeuroDzenPanel *const *const dzen_panel_list;
  PipeInfo *pipe_info;
  NeuroIndex num_panels;
  uint32_t reset_rate;
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// CPU Calculation
static CpuCalcRefreshInfo cpu_calc_refresh_info_;
static bool cpu_calc_stop_refresh_cond_ = false;

// Dzen
static DzenRefreshInfo dzen_refresh_info_;
static bool dzen_stop_refresh_cond_ = false;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Note: Returns true if it has timed out or false when the condition variable has been notified
static bool cond_timedwait(time_t seconds, bool *cond_stop, pthread_mutex_t *mutex, pthread_cond_t *cond_var) {
  struct timespec deadline;
  clock_gettime(CLOCK_REALTIME, &deadline);
  deadline.tv_sec += seconds;
  bool timedout = false;
  pthread_mutex_lock(mutex);
  while (!*cond_stop)
    if (ETIMEDOUT == pthread_cond_timedwait(cond_var, mutex, &deadline)) {
      timedout = true;
      break;
    }
  pthread_mutex_unlock(mutex);
  return timedout;
}

// CPU Calculation (Thread 1)
static bool cpu_calc_refresh_timedwait(time_t seconds) {
  return cond_timedwait(seconds, &cpu_calc_stop_refresh_cond_, &cpu_calc_refresh_info_.wait_mutex,
      &cpu_calc_refresh_info_.wait_cond);
}

static NeuroIndex get_num_cpus(const char *file) {
  assert(file);
  FILE *const fd = fopen(file, "r");
  if (!fd)
    return 0;
  NeuroIndex i = 0U;
  char buf[ 256 ];
  while (fgets(buf, sizeof(buf), fd)) {
    if (strncmp(buf, "cpu", 3) != 0)
      break;
    ++i;
  }
  fclose(fd);
  return i;
}

static void get_perc_info(CpuInfo *cpu_info, uint64_t *cpu_vals, uint64_t prev_idle, uint64_t prev_total) {
  assert(cpu_info);
  assert(cpu_vals);
  cpu_info->idle = cpu_vals[ 3 ];
  cpu_info->total = 0L;
  for (NeuroIndex i = 0U; i < CPU_MAX_VALS; ++i)
    cpu_info->total += cpu_vals[ i ];
  const uint64_t diff_idle = cpu_info->idle - prev_idle;
  const uint64_t diff_total = cpu_info->total - prev_total;
  cpu_info->perc = (100 * (diff_total - diff_idle)) / diff_total;
}

static void refresh_cpu_calc(const char *file, NeuroIndex ncpus) {
  assert(file);
  uint64_t cpus_file_info[ ncpus ][ CPU_MAX_VALS ];
  uint64_t prev_idle[ ncpus ], prev_total[ ncpus ];
  memset(prev_idle, 0, sizeof(prev_idle));
  memset(prev_total, 0, sizeof(prev_total));

  while (true) {
    // Open the file
    FILE *const fd = fopen(file, "r");
    if (!fd)
      return;

    // Do the percent calculation
    char buf[ 256 ];
    for (NeuroIndex i = 0U; i < ncpus; ++i) {
      fgets(buf, sizeof(buf), fd);
      if (EOF == sscanf(buf + 5, "%" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64
          " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64,
          cpus_file_info[ i ] + 0, cpus_file_info[ i ] + 1,
          cpus_file_info[ i ] + 2, cpus_file_info[ i ] + 3,
          cpus_file_info[ i ] + 4, cpus_file_info[ i ] + 5,
          cpus_file_info[ i ] + 6, cpus_file_info[ i ] + 7,
          cpus_file_info[ i ] + 8, cpus_file_info[ i ] + 9))
        return;
      get_perc_info(cpu_calc_refresh_info_.cpu_info + i, cpus_file_info[ i ], prev_idle[ i ], prev_total[ i ]);
      prev_idle[ i ] = cpu_calc_refresh_info_.cpu_info[ i ].idle;
      prev_total[ i ] = cpu_calc_refresh_info_.cpu_info[ i ].total;
    }

    // Close the file
    fclose(fd);

    // Wait 1 second or break if the conditional variable has been signaled
    if (!cpu_calc_refresh_timedwait(1))
      break;
  }
}

static void *refresh_cpu_calc_thread(void *args) {
  (void)args;
  refresh_cpu_calc(CPU_FILE_PATH, cpu_calc_refresh_info_.num_cpus);
  pthread_exit(NULL);
}

static bool init_cpu_calc_thread(void) {
  // Init mutex and cond
  pthread_mutex_init(&cpu_calc_refresh_info_.wait_mutex, NULL);
  pthread_cond_init(&cpu_calc_refresh_info_.wait_cond, NULL);

  // Create thread
  return pthread_create(&cpu_calc_refresh_info_.thread, NULL, refresh_cpu_calc_thread, NULL);
}

static void stop_cpu_calc_thread(void) {
  // Stop calc thread
  cpu_calc_stop_refresh_cond_ = true;
  pthread_cond_broadcast(&cpu_calc_refresh_info_.wait_cond);

  // Join thread
  void *status;
  if (pthread_join(cpu_calc_refresh_info_.thread, &status))  // Wait
    perror("stop_cpu_perc_thread - Could not join thread");

  // Destroy cond and mutex
  pthread_cond_destroy(&cpu_calc_refresh_info_.wait_cond);
  pthread_mutex_destroy(&cpu_calc_refresh_info_.wait_mutex);
}

static bool init_cpu_calc_refresh_info(void) {
  cpu_calc_refresh_info_.num_cpus = get_num_cpus(CPU_FILE_PATH);
  if (cpu_calc_refresh_info_.num_cpus <= 0)
    return false;
  cpu_calc_refresh_info_.cpu_info = (CpuInfo *)calloc(cpu_calc_refresh_info_.num_cpus, sizeof(CpuInfo));
  return cpu_calc_refresh_info_.cpu_info != NULL;
}

static void stop_cpu_calc_refresh_info(void) {
  free(cpu_calc_refresh_info_.cpu_info);
  cpu_calc_refresh_info_.cpu_info = NULL;
}

// Dzen (Thread 2)
static bool dzen_refresh_timedwait(time_t seconds) {
  return cond_timedwait(seconds, &dzen_stop_refresh_cond_, &dzen_refresh_info_.wait_mutex,
      &dzen_refresh_info_.wait_cond);
}

static char **str_to_cmd(char **cmd, char *str, const char *sep) {
  assert(cmd);
  assert(str);
  assert(sep);
  char *save_ptr = NULL;
  char *token = strtok_r(str, sep, &save_ptr);
  NeuroIndex i = 0U;
  for ( ; token; ++i) {
    cmd[ i ] = token;
    token = strtok_r(NULL, sep, &save_ptr);
  }
  cmd[ i ] = NULL;
  return cmd;
}

static char **get_dzen_cmd(char **cmd, char *line, const NeuroDzenFlags *df, const NeuroMonitor *m) {
  assert(cmd);
  assert(line);
  assert(df);
  assert(m);
  snprintf(line, NEURO_DZEN_LINE_MAX, "/usr/bin/dzen2 -x %i -y %i -w %i -h %i -fg %s -bg %s -ta %c -fn %s -e %s %s",
      m->region.p.x - m->gaps[ 2 ] + df->x, m->region.p.y - m->gaps[ 0 ] + df->y, df->w, df->h, df->fg_color,
      df->bg_color, df->align, df->font, df->event, df->extras);
  return str_to_cmd(cmd, line, " \t\n");
}

static void refresh_dzen(const NeuroMonitor *m, const NeuroDzenPanel *dp, int fd) {
  assert(dp);

  // Lock
  pthread_mutex_lock(&dzen_refresh_info_.sync_mutex);

  // Refresh
  char line[ NEURO_DZEN_LINE_MAX ] = "\0";
  for (NeuroIndex i = 0U; dp->loggers[ i ]; ++i) {
    char str[ NEURO_DZEN_LOGGER_MAX ] = "\0";
    dp->loggers[ i ](m, str);

    // Add separator if not first and not empty str
    if (i > 0U && str[ 0 ] != '\0')
      strncat(line, dp->sep, NEURO_DZEN_LINE_MAX - strlen(line) - 1);
    strncat(line, str, NEURO_DZEN_LINE_MAX - strlen(line) - 1);
  }

  // Line must be '\n' terminated so that dzen can display it
  strncat(line, "\n", NEURO_DZEN_LINE_MAX - strlen(line) - 1);
  write(fd, line, strlen(line));

  // Unlock
  pthread_mutex_unlock(&dzen_refresh_info_.sync_mutex);
}

static void *refresh_dzen_thread(void *args) {
  (void)args;
  uint32_t i = 0U;
  while (true) {
    for (NeuroIndex j = 0U; j < dzen_refresh_info_.num_panels; ++j) {
      const PipeInfo *const pi = dzen_refresh_info_.pipe_info + j;
      if (pi->dzen_panel->refresh_rate == NEURO_DZEN_REFRESH_ON_EVENT)
        continue;
      if (i % pi->dzen_panel->refresh_rate == 0U)
        refresh_dzen(pi->monitor, pi->dzen_panel, dzen_refresh_info_.pipe_info[ j ].output);
    }
    ++i;
    i %= dzen_refresh_info_.reset_rate;

    // Wait 1 second or break if the conditional variable has been signaled
    if (!dzen_refresh_timedwait(1))
      break;
  }
  pthread_exit(NULL);
}

static bool init_dzen_refresh_thread(void) {
  if (dzen_refresh_info_.reset_rate == 0U)
    return true;

  // Init mutex and cond
  pthread_mutex_init(&dzen_refresh_info_.wait_mutex, NULL);
  pthread_cond_init(&dzen_refresh_info_.wait_cond, NULL);

  // Create thread
  return pthread_create(&dzen_refresh_info_.thread, NULL, refresh_dzen_thread, NULL) == 0;
}

static void stop_dzen_refresh_thread(void) {
  if (dzen_refresh_info_.reset_rate == 0U)
    return;

  // Stop refresh thread
  dzen_stop_refresh_cond_ = true;
  pthread_cond_broadcast(&dzen_refresh_info_.wait_cond);

  // Join thread
  void *status;
  if (pthread_join(dzen_refresh_info_.thread, &status))  // Wait
    perror("stop_refresh_thread - Could not join thread");

  // Init cond and mutex
  pthread_cond_destroy(&dzen_refresh_info_.wait_cond);
  pthread_mutex_destroy(&dzen_refresh_info_.wait_mutex);
}

static bool init_dzen_refresh_info(void) {
  // Get the number of pannels
  NeuroIndex num_panels = 0U;
  for (const NeuroMonitor *m = NeuroMonitorSelectorHead(NULL); m; m = NeuroMonitorSelectorNext(m)) {
    const NeuroIndex size = NeuroTypeArrayLength((const void *const *)m->dzen_panel_list);
    for (NeuroIndex i = 0U; i < size; ++i)
      ++num_panels;
  }

  // Allocate
  dzen_refresh_info_.num_panels = num_panels;
  dzen_refresh_info_.pipe_info = (PipeInfo *)calloc(num_panels, sizeof(PipeInfo));
  if (!dzen_refresh_info_.pipe_info)
    return false;

  // Initialize
  NeuroIndex panel_iterator = 0U;
  dzen_refresh_info_.reset_rate = 1U;
  for (const NeuroMonitor *m = NeuroMonitorSelectorLast(NULL); m; m = NeuroMonitorSelectorPrev(m)) {
    const NeuroIndex size = NeuroTypeArrayLength((const void *const *)m->dzen_panel_list);
    for (NeuroIndex i = 0U; i < size; ++i) {
      const NeuroDzenPanel *const dp = m->dzen_panel_list[ i ];

      // Get max refresh rate
      if (dp->refresh_rate > dzen_refresh_info_.reset_rate)
        dzen_refresh_info_.reset_rate *= dp->refresh_rate;

      // Create a dzen pipe for every panel
      char *dzen_cmd[ NEURO_DZEN_ARGS_MAX ];
      char line[ NEURO_DZEN_LINE_MAX ];
      get_dzen_cmd(dzen_cmd, line, dp->df, m);
      dzen_refresh_info_.pipe_info[ panel_iterator ].output = NeuroSystemSpawnPipe((const char *const *)dzen_cmd,
          &(dzen_refresh_info_.pipe_info[ panel_iterator ].pid));
      if (dzen_refresh_info_.pipe_info[ panel_iterator ].output == -1)
        return false;
      dzen_refresh_info_.pipe_info[ panel_iterator ].dzen_panel = dp;
      dzen_refresh_info_.pipe_info[ panel_iterator ].monitor = m;

      ++panel_iterator;
    }
  }

  // Init sync mutex
  pthread_mutex_init(&dzen_refresh_info_.sync_mutex, NULL);
  return true;
}

static void stop_dzen_refresh_info(void) {
  // Destroy sync mutex
  pthread_mutex_destroy(&dzen_refresh_info_.sync_mutex);

  // Release pipe info
  for (NeuroIndex i = 0U; i < dzen_refresh_info_.num_panels; ++i)
    if (kill(dzen_refresh_info_.pipe_info[ i ].pid, SIGTERM) == -1)
      perror("stop_dzen_refresh_info - Could not kill panels");
  free(dzen_refresh_info_.pipe_info);
  dzen_refresh_info_.pipe_info = NULL;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Dzen
bool NeuroDzenInit(void) {
  if (!init_dzen_refresh_info() || !init_dzen_refresh_thread())
    return false;
  NeuroDzenRefresh(false);
  return true;
}

void NeuroDzenStop(void) {
  stop_dzen_refresh_thread();
  stop_dzen_refresh_info();
}

void NeuroDzenRefresh(bool on_event_only) {
  for (NeuroIndex i = 0U; i < dzen_refresh_info_.num_panels; ++i) {
    const PipeInfo *const pi = dzen_refresh_info_.pipe_info + i;
    if (on_event_only && (pi->dzen_panel->refresh_rate == NEURO_DZEN_REFRESH_ON_EVENT)) {
      refresh_dzen(pi->monitor, pi->dzen_panel, dzen_refresh_info_.pipe_info[ i ].output);
      continue;
    }
    refresh_dzen(pi->monitor, pi->dzen_panel, dzen_refresh_info_.pipe_info[ i ].output);
  }
}

void NeuroDzenInitCpuCalc(void) {
  if (!init_cpu_calc_refresh_info())
    NeuroSystemError(__func__, "Could not init CPU Set");
  if (init_cpu_calc_thread())
    NeuroSystemError(__func__, "Could not init CPU percent thread");
}

void NeuroDzenStopCpuCalc(void) {
  stop_cpu_calc_thread();
  stop_cpu_calc_refresh_info();
}

void NeuroDzenWrapDzenBox(char *dst, const char *src, const NeuroDzenBox *b) {
  assert(dst);
  assert(src);
  assert(b);
  snprintf(dst, NEURO_DZEN_LOGGER_MAX,
      "^fg(%s)^i(%s)^ib(1)^r(1920x%i)^p(-1920x)^fg(%s)%s^fg(%s)^i(%s)^fg(%s)^r(1920x%i)^p(-1920)^fg()^ib(0)",
      b->box_color, b->left_icon, b->box_height, b->fg_color, src, b->box_color, b->right_icon, b->bg_color,
      b->box_height);
}

void NeuroDzenWrapClickArea(char *dst, const char *src, const NeuroDzenClickableArea *ca) {
  assert(dst);
  assert(src);
  assert(ca);
  snprintf(dst, NEURO_DZEN_LOGGER_MAX, "^ca(1,%s)^ca(2,%s)^ca(3,%s)^ca(4,%s)^ca(5,%s)%s^ca()^ca()^ca()^ca()^ca()",
      ca->left_click, ca->middle_click, ca->right_click, ca->wheel_up, ca->wheel_down, src);
}

bool NeuroDzenReadFirstLineFile(char *buf, const char *path) {
  assert(buf);
  assert(path);
  FILE *const fd = fopen(path, "r");
  if (!fd)
    return false;
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  char *const last = buf + strlen(buf) - 1;
  if (*last == '\n')
    *last = '\0';
  fclose(fd);
  return true;
}

// Loggers
void NeuroDzenLoggerTime(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  struct tm res;
  time_t t = time(NULL);
  localtime_r(&t, &res);
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "%02d:%02d:%02d", res.tm_hour, res.tm_min, res.tm_sec);
}


void NeuroDzenLoggerDate(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  struct tm res;
  time_t t = time(NULL);
  localtime_r(&t, &res);
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "%d.%02d.%02d", res.tm_year + 1900, res.tm_mon + 1, res.tm_mday);
}

void NeuroDzenLoggerDay(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  struct tm res;
  time_t t = time(NULL);
  localtime_r(&t, &res);
  switch (res.tm_wday) {
    case 1:  strncpy(str, "Monday", NEURO_DZEN_LOGGER_MAX); break;
    case 2:  strncpy(str, "Tuesday", NEURO_DZEN_LOGGER_MAX); break;
    case 3:  strncpy(str, "Wednesday", NEURO_DZEN_LOGGER_MAX); break;
    case 4:  strncpy(str, "Thursday", NEURO_DZEN_LOGGER_MAX); break;
    case 5:  strncpy(str, "Friday", NEURO_DZEN_LOGGER_MAX); break;
    case 6:  strncpy(str, "Saturday", NEURO_DZEN_LOGGER_MAX); break;
    case 0:  strncpy(str, "Sunday", NEURO_DZEN_LOGGER_MAX); break;
    default: strncpy(str, "Unknown", NEURO_DZEN_LOGGER_MAX); break;
  }
}

void NeuroDzenLoggerUptime(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  struct sysinfo info;
  sysinfo(&info);
  const uint32_t hours = (uint32_t)(info.uptime / 3600UL);
  const uint32_t hrest = (uint32_t)(info.uptime % 3600UL);
  const uint32_t minutes = hrest / 60;
  const uint32_t seconds = hrest % 60;
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "%" PRIu32 "h %" PRIu32 "m %" PRIu32 "s", hours, minutes, seconds);
}

void NeuroDzenLoggerCpu(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  char buf[ NEURO_DZEN_LOGGER_MAX ];
  for (NeuroIndex i = 0U; i < cpu_calc_refresh_info_.num_cpus; ++i) {
    snprintf(buf, NEURO_DZEN_LOGGER_MAX, "%" PRIu32 "%% ", cpu_calc_refresh_info_.cpu_info[ i ].perc);
    strncat(str, buf, NEURO_DZEN_LOGGER_MAX - strlen(str) - 1);
  }
  str[ strlen(str) - 1 ] = '\0';
}

void NeuroDzenLoggerRam(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  char buf[ NEURO_DZEN_LOGGER_MAX ];
  FILE *const fd = fopen("/proc/meminfo", "r");
  if (!fd)
    return;
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  uint64_t mem_total = 0UL, mem_available = 0UL;
  sscanf(buf, "%*s %" PRIu64 "\n", &mem_total);
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  sscanf(buf, "%*s %" PRIu64 "\n", &mem_available);
  const uint64_t mem_used = mem_total - mem_available;
  const uint32_t perc = (uint32_t)((mem_used * 100UL) / mem_total);
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "%" PRIu32 "%% %" PRIu64 "MB", perc, mem_used / 1024UL);
  fclose(fd);
}

void NeuroDzenLoggerWifiStrength(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  char buf[ NEURO_DZEN_LOGGER_MAX ];
  FILE *const fd = fopen("/proc/net/wireless", "r");
  if (!fd)
    return;
  uint32_t strength = 0U, tmp = 0U;
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  fgets(buf, NEURO_DZEN_LOGGER_MAX, fd);
  sscanf(buf, "%*s %" PRIu32 "   %" PRIu32 "\n", &tmp, &strength);
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "%" PRIu32 "%%", strength);
  fclose(fd);
}

void NeuroDzenLoggerMonitorWorkspace(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const char *const name = NeuroCoreStackGetName(NeuroCoreGetMonitorStack(m));
  if (name)
    strncpy(str, name, NEURO_DZEN_LOGGER_MAX);
}

void NeuroDzenLoggerMonitorCurrLayout(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroLayoutConf *const lc = NeuroCoreStackGetCurrLayoutConf(NeuroCoreGetMonitorStack(m));
  if (lc)
    strncpy(str, lc->name, NEURO_DZEN_LOGGER_MAX);
}

void NeuroDzenLoggerMonitorCurrTitle(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  const NeuroClientPtrPtr c = NeuroCoreStackGetCurrClient(NeuroCoreGetMonitorStack(m));
  if (c)
    strncpy(str, NEURO_CLIENT_PTR(c)->title, NEURO_DZEN_LOGGER_MAX);
}

void NeuroDzenLoggerScreen(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  const NeuroRectangle *const r = NeuroSystemGetScreenRegion();
  snprintf(str, NEURO_DZEN_LOGGER_MAX, "[screen: %i,%i %ix%i]", r->p.x, r->p.y, r->w, r->h);
}

void NeuroDzenLoggerMonitorList(const NeuroMonitor *m, char *str) {
  assert(m);
  assert(str);
  (void)m;
  for (const NeuroMonitor *mon = NeuroMonitorSelectorHead(NULL); mon; mon = NeuroMonitorSelectorNext(mon)) {
    static char buf[ NEURO_DZEN_LINE_MAX ];
    static NeuroRectangle r;
    NeuroGeometryRectangleGetIncreased(&r, &mon->region, mon->gaps);
    snprintf(buf, NEURO_DZEN_LOGGER_MAX, "[%s: %ix%i]", mon->name ? mon->name : "Unknown", r.w, r.h);
    strncat(str, buf, NEURO_DZEN_LINE_MAX - strlen(str) - 1);
  }
}

