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
  NeuroMonitor *monitor_list;
  NeuroIndex size;
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static MonitorSet monitor_set_;


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

bool NeuroMonitorInit(void) {
  // There must be at least 1 monitor in the configuration
  const NeuroMonitorConf *const *const monitor_list = NeuroConfigGet()->monitor_list;
  if (!monitor_list || !*monitor_list)
    return false;

#ifdef XRANDR

  XRRScreenResources *const screen_list = XRRGetScreenResources(NeuroSystemGetDisplay(), NeuroSystemGetRoot());
  if (!screen_list)
    return false;

  // Return false if we have more monitors than the ones returned by xrandr
  const NeuroIndex num_monitors = NeuroTypeArrayLength((const void *const *)monitor_list);
  const NeuroIndex num_xrandr_monitors = screen_list->ncrtc;
  if (num_monitors >= num_xrandr_monitors)
    return false;

  // Alloc the monitor list
  monitor_set_.size = num_monitors;
  monitor_set_.monitor_list = (NeuroMonitor *)calloc(num_monitors, sizeof(NeuroMonitor));
  if (!monitor_set_.monitor_list)
    return false;

  NeuroIndex monitor_iterator = 0U;
  for (NeuroIndex i = 0U; i < num_xrandr_monitors; ++i) {
    // Add as many monitors as we have in the configuration
    if (monitor_iterator >= num_monitors)
      break;

    // Skip not valid monitors
    XRRCrtcInfo *screen = XRRGetCrtcInfo(NeuroSystemGetDisplay(), screen_list, screen_list->crtcs[ i ]);
    if (!screen->mode)
      continue;

    // Get NeuroMonitor and NeuroMonitorConf pointers
    const NeuroMonitorConf *const mc = monitor_list[ monitor_iterator ];
    NeuroMonitor *const m = monitor_set_.monitor_list + monitor_iterator;

    // Initialize NeuroMonitor using NeuroMonitorConf and Screen data
    m->name = mc->name;
    m->gaps = mc->gaps;
    m->default_ws = mc->default_ws;
    m->dzen_panel_list = mc->dzen_panel_list;
    const NeuroRectangle screen_region = { (NeuroPoint){ screen->x, screen->y }, screen->width, screen->height };
    NeuroGeometryRectangleGetReduced((NeuroRectangle *)&m->region, &screen_region, mc->gaps);

    // Increment monitor iterator
    ++monitor_iterator;
  }

  // Release screen_list
  XRRFreeScreenResources(screen_list);

#else

  // Alloc just 1 monitor
  monitor_set_.size = 1U;
  monitor_set_.monitor_list = (NeuroMonitor *)calloc(1U, sizeof(NeuroMonitor));
  if (!monitor_set_.monitor_list)
    return false;

  // Initialize the monitor
  const NeuroMonitorConf *const mc = monitor_list[ 0U ];
  NeuroMonitor *const m = monitor_set_.monitor_list + 0U;
  const NeuroRectangle *const screen = NeuroSystemGetScreenRegion();
  m->name = mc->name;
  m->gaps = mc->gaps;
  m->default_ws = mc->default_ws;
  m->dzen_panel_list = mc->dzen_panel_list;
  const NeuroRectangle screen_region = { (NeuroPoint){ screen->p.x, screen->p.y }, screen->w, screen->h };
  NeuroGeometryRectangleGetReduced((NeuroRectangle *)&m->region, &screen_region, mc->gaps);

#endif

  return true;
}

void NeuroMonitorStop(void) {
  free(monitor_set_.monitor_list);
  monitor_set_.monitor_list = NULL;
}

NeuroIndex NeuroMonitorGetSize(void) {
  return monitor_set_.size;
}

const NeuroMonitor *NeuroMonitorGet(NeuroIndex m) {
  return monitor_set_.monitor_list + (m % monitor_set_.size);
}

const NeuroMonitor *NeuroMonitorFindDefault(NeuroIndex ws) {
  for (NeuroIndex i = 0U; i < monitor_set_.size; ++i) {
    const NeuroMonitor *const m = monitor_set_.monitor_list + i;
    if (m->default_ws == ws)
      return m;
  }
  return NULL;
}

const NeuroMonitor *NeuroMonitorFindPointed(const NeuroPoint *p) {
  for (NeuroIndex i = 0U; i < monitor_set_.size; ++i) {
    const NeuroMonitor *const m = monitor_set_.monitor_list + i;
    if (NeuroGeometryIsPointInRectangle(&m->region, p))
      return m;
  }
  return NULL;
}

const NeuroMonitor *NeuroMonitorSelectorHead(const NeuroMonitor *m) {
  (void)m;
  return monitor_set_.monitor_list;
}

const NeuroMonitor *NeuroMonitorSelectorLast(const NeuroMonitor *m) {
  (void)m;
  return monitor_set_.monitor_list + monitor_set_.size - 1;
}

const NeuroMonitor *NeuroMonitorSelectorNext(const NeuroMonitor *m) {
  const NeuroMonitor *const next = m + 1U;
  return (next >= monitor_set_.monitor_list + monitor_set_.size) ? NULL : next;
}

const NeuroMonitor *NeuroMonitorSelectorPrev(const NeuroMonitor *m) {
  const NeuroMonitor *const prev = m - 1U;
  return (prev < monitor_set_.monitor_list) ? NULL : prev;
}

const NeuroMonitor *NeuroMonitorSelector0(const NeuroMonitor *m) {
  (void)m;
  return (0U < monitor_set_.size) ? monitor_set_.monitor_list + 0U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector1(const NeuroMonitor *m) {
  (void)m;
  return (1U < monitor_set_.size) ? monitor_set_.monitor_list + 1U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector2(const NeuroMonitor *m) {
  (void)m;
  return (2U < monitor_set_.size) ? monitor_set_.monitor_list + 2U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector3(const NeuroMonitor *m) {
  (void)m;
  return (3U < monitor_set_.size) ? monitor_set_.monitor_list + 3U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector4(const NeuroMonitor *m) {
  (void)m;
  return (4U < monitor_set_.size) ? monitor_set_.monitor_list + 4U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector5(const NeuroMonitor *m) {
  (void)m;
  return (5U < monitor_set_.size) ? monitor_set_.monitor_list + 5U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector6(const NeuroMonitor *m) {
  (void)m;
  return (6U < monitor_set_.size) ? monitor_set_.monitor_list + 6U : NULL;
}
const NeuroMonitor *NeuroMonitorSelector7(const NeuroMonitor *m) {
  (void)m;
  return (7U < monitor_set_.size) ? monitor_set_.monitor_list + 7U : NULL;
}

