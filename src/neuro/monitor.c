//----------------------------------------------------------------------------------------------------------------------
// Module      :  monitor
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "monitor.h"
#include "system.h"
#include "config.h"
#include "geometry.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// MonitorSet
typedef struct MonitorSet MonitorSet;
struct MonitorSet {
  Monitor *monitor_list;
  size_t size;
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static MonitorSet monitor_set_;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

bool NeuroMonitorInit() {

  // There must have at least 1 monitor in the configuration
  const MonitorConf *const *const monitor_list = NeuroConfigGet()->monitor_list;
  if (!monitor_list || !*monitor_list)
    return false;

#ifdef XRANDR

  XRRScreenResources *const screen_list = XRRGetScreenResources(NeuroSystemGetDisplay(), NeuroSystemGetRoot());
  if (!screen_list)
    return false;

  // Return false if we have more monitors than the ones returned by xrandr
  const size_t num_monitors = NeuroTypeArrayLength((const void *const *)monitor_list);
  const size_t num_xrandr_monitors = screen_list->ncrtc;
  if (num_monitors >= num_xrandr_monitors)
    return false;

  // Alloc the monitor list
  monitor_set_.size = num_monitors;
  monitor_set_.monitor_list = (Monitor *)calloc(num_monitors, sizeof(Monitor));
  if (!monitor_set_.monitor_list)
    return false;

  size_t monitor_iterator = 0U;
  for (size_t i = 0U; i < num_xrandr_monitors; ++i) {
    // Add as many monitors as we have in the configuration
    if (monitor_iterator >= num_monitors)
      break;

    // Skip not valid monitors
    XRRCrtcInfo *screen = XRRGetCrtcInfo(NeuroSystemGetDisplay(), screen_list, screen_list->crtcs[ i ]);
    if (!screen->mode)
      continue;

    // Get Monitor and MonitorConf pointers
    const MonitorConf *const mc = monitor_list[ monitor_iterator ];
    const Monitor *const m = monitor_set_.monitor_list + monitor_iterator;

    // Initialize Monitor using MonitorConf and Screen data
    *(const char **)&m->name = mc->name;
    *(const int **)&m->gaps = mc->gaps;
    *(size_t *)&m->default_ws = mc->default_ws;
    *(const DzenPanel *const **)&m->dzen_panel_list = mc->dzen_panel_list;
    const Rectangle screen_region = { screen->x, screen->y, screen->width, screen->height };
    NeuroGeometrySetRectangleGaps((Rectangle *)&m->region, &screen_region, mc->gaps);

    // Increment monitor iterator
    ++monitor_iterator;
  }

  // Release screen_list
  XRRFreeScreenResources(screen_list);

#else

  // Alloc just 1 monitor
  monitor_set_.size = 1U;
  monitor_set_.monitor_list = (Monitor *)calloc(1U, sizeof(Monitor));
  if (!monitor_set_.monitor_list)
    return false;

  // Initialize the monitor
  const MonitorConf *const mc = monitor_list[ 0U ];
  const Monitor *const m = monitor_set_.monitor_list + 0U;
  const Rectangle *const screen = NeuroSystemGetScreenRegion();
  *(const char **)&m->name = mc->name;
  *(const int **)&m->gaps = mc->gaps;
  *(size_t *)&m->default_ws = mc->default_ws;
  *(const DzenPanel *const **)&m->dzen_panel_list = mc->dzen_panel_list;
  const Rectangle screen_region = { screen->x, screen->y, screen->w, screen->h };
  NeuroGeometrySetRectangleGaps((Rectangle *)&m->region, &screen_region, mc->gaps);

#endif

  return true;
}

void NeuroMonitorStop() {
  free(monitor_set_.monitor_list);
  monitor_set_.monitor_list = NULL;
}

size_t NeuroMonitorGetSize() {
  return monitor_set_.size;
}

const Monitor *NeuroMonitorGet(size_t m) {
  return monitor_set_.monitor_list + (m % monitor_set_.size);
}

const Monitor *NeuroMonitorFindDefault(size_t ws) {
  for (size_t i = 0U; i < monitor_set_.size; ++i) {
    const Monitor *const m = monitor_set_.monitor_list + i;
    if (m->default_ws == ws)
      return m;
  }
  return NULL;
}

const Monitor *NeuroMonitorFindPtr(int x, int y) {
  for (size_t i = 0U; i < monitor_set_.size; ++i) {
    const Monitor *const m = monitor_set_.monitor_list + i;
    if (NeuroGeometryIsPointInRectangle(&m->region, x, y)) {
      return m;
    }
  }
  return NULL;
}

const Monitor *NeuroMonitorSelectorHead(const Monitor *m) {
  (void)m;
  return monitor_set_.monitor_list;
}

const Monitor *NeuroMonitorSelectorLast(const Monitor *m) {
  (void)m;
  return monitor_set_.monitor_list + monitor_set_.size - 1;
}

const Monitor *NeuroMonitorSelectorNext(const Monitor *m) {
  const Monitor *const next = m + 1U;
  return (next >= monitor_set_.monitor_list + monitor_set_.size) ? NULL : next;
}

const Monitor *NeuroMonitorSelectorPrev(const Monitor *m) {
  const Monitor *const prev = m - 1U;
  return (prev < monitor_set_.monitor_list) ? NULL : prev;
}

const Monitor *NeuroMonitorSelector0(const Monitor *m) {
  (void)m;
  return (0U < monitor_set_.size) ? monitor_set_.monitor_list + 0U : NULL;
}
const Monitor *NeuroMonitorSelector1(const Monitor *m) {
  (void)m;
  return (1U < monitor_set_.size) ? monitor_set_.monitor_list + 1U : NULL;
}
const Monitor *NeuroMonitorSelector2(const Monitor *m) {
  (void)m;
  return (2U < monitor_set_.size) ? monitor_set_.monitor_list + 2U : NULL;
}
const Monitor *NeuroMonitorSelector3(const Monitor *m) {
  (void)m;
  return (3U < monitor_set_.size) ? monitor_set_.monitor_list + 3U : NULL;
}
const Monitor *NeuroMonitorSelector4(const Monitor *m) {
  (void)m;
  return (4U < monitor_set_.size) ? monitor_set_.monitor_list + 4U : NULL;
}
const Monitor *NeuroMonitorSelector5(const Monitor *m) {
  (void)m;
  return (5U < monitor_set_.size) ? monitor_set_.monitor_list + 5U : NULL;
}
const Monitor *NeuroMonitorSelector6(const Monitor *m) {
  (void)m;
  return (6U < monitor_set_.size) ? monitor_set_.monitor_list + 6U : NULL;
}
const Monitor *NeuroMonitorSelector7(const Monitor *m) {
  (void)m;
  return (7U < monitor_set_.size) ? monitor_set_.monitor_list + 7U : NULL;
}

