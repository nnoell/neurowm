//----------------------------------------------------------------------------------------------------------------------
// Module      :  core
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "core.h"
#include "system.h"
#include "config.h"
#include "geometry.h"
#include "monitor.h"

// Defines
#define STEP_SIZE_REALLOC 32


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Node
typedef struct Node Node;
struct Node {
  NeuroClient *cli;
  Node *next;
  Node *prev;
  NeuroRectangle region;
};

// Stack
typedef struct Stack Stack;
struct Stack {
  const char *name;
  Node *curr;
  Node *prev;  // Previous selected node
  Node *head;
  Node *last;
  Node *nsp;
  NeuroIndex size;  // Number of clients the stack has
  const NeuroMonitor *monitor;
  const int *gaps;
  NeuroRectangle region;
  NeuroIndex curr_layout_index;
  NeuroIndex curr_toggled_layout_index;
  bool is_toggled_layout;
  NeuroIndex num_layouts;
  NeuroIndex num_toggled_layouts;
  NeuroLayout *layouts;
  NeuroLayout *toggled_layouts;
  NeuroIndex num_minimized;  // Number of minimized clients
  NeuroClient **minimized_clients;  // List of minimized clients
  NeuroIndex minimized_size;  // Allocated size
};

// StackSet
typedef struct StackSet StackSet;
struct StackSet {
  Stack *stack_list;
  NeuroIndex curr;
  NeuroIndex old;  // Previouse selected workspace
  NeuroIndex size;  // Number of stacks the stackset has
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// StackSet
static StackSet stack_set_;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void update_nsp_stack(Stack *s) {
  assert(s);
  for (Node *n = s->head; n; n = n->next)
    if (n->cli->is_nsp) {
      s->nsp = n;
      return;
    }
  s->nsp = NULL;
}

static Node *new_node(NeuroClient *c) {
  assert(c);
  Node *const n = (Node *)malloc(sizeof(Node));
  if (!n)
    return NULL;
  n->cli = (NeuroClient *)c;
  memmove(&(n->region), &(c->float_region), sizeof(NeuroRectangle));
  n->next = NULL;
  n->prev = NULL;
  return n;
}

static void delete_node(Node *n) {
  if (!n)
    return;
  free(n);
  n = NULL;
}

static void set_curr_node(Node *n) {
  assert(n);
  Stack *const s = stack_set_.stack_list + n->cli->ws;
  if (n == s->curr)
    return;
  s->prev = s->curr;
  s->curr = n;
}

static NeuroClient *remove_last_node(Stack *s) {
  assert(s);
  if (s->size < 1)
    return NULL;
  Node *const t = s->last;
  const bool update_nsp = t->cli->is_nsp;
  if (s->size == 1) {
    s->head = NULL;
    s->last = NULL;
    s->curr = NULL;
  } else {
    set_curr_node(s->last->prev);
    s->last->prev->next = NULL;
    s->last = t->prev;
  }
  NeuroClient *const ret = t->cli;
  delete_node(t);
  s->size--;
  if (update_nsp)
    update_nsp_stack(s);
  return ret;
}

static NeuroClient *remove_no_last_node(Node *n) {
  assert(n);
  Stack *const s = stack_set_.stack_list + n->cli->ws;
  if (s->size == 0U || s->last == n)
    return NULL;
  const bool update_nsp = n->cli->is_nsp;
  set_curr_node(n->next);
  if (n == s->head) {
    s->head = n->next;
    n->next->prev = NULL;
  } else {
    n->prev->next = n->next;
    n->next->prev = n->prev;
  }
  NeuroClient *const ret = n->cli;
  delete_node(n);
  s->size--;
  if (update_nsp)
    update_nsp_stack(s);
  return ret;
}

static bool update_minimized_clients_size(Stack *s, NeuroIndex new_total) {
  if (!s)
    return false;

  // Get the new needed size
  NeuroIndex new_size = STEP_SIZE_REALLOC;
  while (new_size < new_total)
    new_size += STEP_SIZE_REALLOC;

  // Realoc if it is different than the current allocated size
  if (new_size != s->minimized_size) {
    s->minimized_clients = (NeuroClient **)realloc(s->minimized_clients, new_size*sizeof(void *));
    if (!s->minimized_clients)
      return false;
    s->minimized_size = new_size;
  }
  return true;
}

static NeuroClient *push_minimized_client(Stack *s, NeuroClient *c) {
  if (!s || !c)
    return NULL;

  // Realoc memory if needed
  const NeuroIndex new_total = s->num_minimized + 1;
  if (!update_minimized_clients_size(s, new_total))
    return NULL;

  // Store the client
  s->num_minimized = new_total;
  s->minimized_clients[ new_total - 1 ] = c;
  return c;
}

static NeuroClient *pop_minimized_client(Stack *s) {
  if (!s || s->num_minimized == 0)
    return NULL;

  // Realloc memory if needed
  const NeuroIndex new_count = s->num_minimized - 1;
  if (!update_minimized_clients_size(s, new_count))
    return NULL;

  NeuroClient *const cli = s->minimized_clients[ new_count ];
  s->num_minimized = new_count;
  return cli;
}

static NeuroClient *remove_minimized_client(Stack *s, Window w) {
  if (!s)
    return NULL;

  NeuroClient *found = NULL;
  for (NeuroIndex i = 0U; i < s->num_minimized; ++i) {
    NeuroClient *const c = s->minimized_clients[ i ];
    if (found)
      s->minimized_clients[ i-1 ] = c;
    if (c->win == w)
      found = c;
  }
  if (found)
    s->num_minimized--;
  return found;
}

static void set_layouts(NeuroLayout *layout, const NeuroLayoutConf *const *layout_conf, NeuroIndex size) {
  assert(layout);
  assert(layout_conf);

  for (NeuroIndex i = 0U; i < size; ++i) {
    NeuroLayout *const l = layout + i;
    const NeuroLayoutConf *const lc = layout_conf[ i ];
    l->arranger_fn = lc->arranger_fn;
    l->border_color_setter_fn = lc->border_color_setter_fn;
    l->border_width_setter_fn = lc->border_width_setter_fn;
    l->border_gap_setter_fn = lc->border_gap_setter_fn;
    l->region = lc->region;
    l->mod = lc->mod;
    l->follow_mouse = lc->follow_mouse;
    memmove(l->parameters, lc->parameters, sizeof(NeuroArg)*NEURO_ARRANGE_ARGS_MAX);
  }
}

static bool init_stack(Stack *s, const char *name, const NeuroLayoutConf *const *l, const NeuroLayoutConf *const *tl) {
  if (!s || !l || !tl)
    return false;

  // Get sizes
  const NeuroIndex size_l = NeuroTypeArrayLength((const void *const *)(l));
  if (size_l == 0U)
    return false;
  const NeuroIndex size_tl = NeuroTypeArrayLength((const void *const *)(tl));
  if (size_tl == 0U)
    return false;

  s->layouts = (NeuroLayout *)calloc(size_l, sizeof(NeuroLayout));
  if (!s->layouts)
    return false;
  s->toggled_layouts = (NeuroLayout *)calloc(size_tl, sizeof(NeuroLayout));
  if (!s->toggled_layouts)
    return false;

  s->minimized_clients = (NeuroClient **)calloc(STEP_SIZE_REALLOC, sizeof(void *));
  if (!s->minimized_clients)
    return false;

  // Set clients
  s->curr = NULL;
  s->prev = NULL;
  s->head = NULL;
  s->last = NULL;
  s->nsp = NULL;
  s->size = 0;
  s->num_minimized = 0;
  s->minimized_size = STEP_SIZE_REALLOC;

  // Set the layouts
  s->curr_layout_index = 0U;
  s->curr_toggled_layout_index = 0U;
  s->is_toggled_layout = false;
  s->num_layouts = size_l;
  s->num_toggled_layouts = size_tl;

  // Set configuration
  s->name = name;
  set_layouts(s->layouts, l, size_l);
  set_layouts(s->toggled_layouts, tl, size_tl);

  return true;
}

static void stop_stack(Stack *s) {
  if (!s)
    return;

  // Remove clients
  NeuroClient *c = NULL;
  while ((c = remove_last_node(s)))
    NeuroTypeDeleteClient(c);

  // Remove minimized clients
  while ((c = pop_minimized_client(s)))
    NeuroTypeDeleteClient(c);
  free(s->minimized_clients);
  s->minimized_clients = NULL;

  // Remove layouts
  free(s->layouts);
  s->layouts = NULL;
  free(s->toggled_layouts);
  s->toggled_layouts = NULL;
}

static Stack *new_stack_list(NeuroIndex size) {
  return (Stack *)calloc(size, sizeof(Stack));
}

static void delete_stack_list(Stack *s) {
  if (!s)
    return;
  free(s);
  s = NULL;
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// StackSet
bool NeuroCoreInit(void) {
  // Allocate as many stacks as we need
  const NeuroWorkspace *const *const workspace_list = NeuroConfigGet()->workspace_list;
  if (!workspace_list)
    return false;
  const NeuroIndex size = NeuroTypeArrayLength((const void *const *const)workspace_list);
  if (size == 0U)
    return false;
  stack_set_.stack_list = new_stack_list(size);
  if (!stack_set_.stack_list)
    return false;

  // Initialize the stack set
  stack_set_.curr = 0U;
  stack_set_.old = 0U;
  stack_set_.size = size;

  // Initialize the stacks
  for (NeuroIndex i = 0U; workspace_list[ i ]; ++i) {
    const NeuroWorkspace *const ws = workspace_list[ i ];
    Stack *const s = stack_set_.stack_list + i;

    // Initialize the stack
    if (!init_stack(s, ws->name, ws->layouts, ws->toggled_layouts))
      return false;

    // Set the monitors
    const NeuroMonitor *const m = NeuroMonitorFindDefault(i);
    if (m)
      stack_set_.curr = m->default_ws;
    NeuroCoreStackSetMonitor(i, m);
  }

  return true;
}

void NeuroCoreStop(void) {
  // Remove the stacks
  for (NeuroIndex i = 0U; i < stack_set_.size; ++i)
    stop_stack(stack_set_.stack_list + i);

  // Remove the stack list
  delete_stack_list(stack_set_.stack_list);
  stack_set_.stack_list = NULL;
}

NeuroIndex NeuroCoreGetHeadStack(void) {
  return 0U;
}
NeuroIndex NeuroCoreGetLastStack(void) {
  return stack_set_.size - 1U;
}

NeuroIndex NeuroCoreGetCurrStack(void) {
  return stack_set_.curr;
}

NeuroIndex NeuroCoreGetPrevStack(void) {
  return (stack_set_.size + stack_set_.curr - 1U) % stack_set_.size;
}

NeuroIndex NeuroCoreGetNextStack(void) {
  return (stack_set_.curr + 1U) % stack_set_.size;
}

NeuroIndex NeuroCoreGetOldStack(void) {
  return stack_set_.old;
}

NeuroIndex NeuroCoreGetNspStack(void) {
  return stack_set_.size - 1U;  // NSP stack is always the last one
}

NeuroIndex NeuroCoreGetMonitorStack(const NeuroMonitor *m) {
  for (NeuroIndex i = 0U; i < stack_set_.size; ++i) {
    const Stack *const s = stack_set_.stack_list + i;
    if (s->monitor == m)
      return i;
  }
  return stack_set_.curr;  // It should never happen
}

NeuroIndex NeuroCoreGetSize(void) {
  return stack_set_.size;
}

NeuroIndex NeuroCoreGetNspStackSize(void) {
  return stack_set_.stack_list[ stack_set_.size - 1U ].size;
}

NeuroClientPtrPtr NeuroCoreGetCurrClientNspStack(void) {
  return (NeuroClientPtrPtr)stack_set_.stack_list[ stack_set_.size - 1U ].curr;
}

void NeuroCoreSetCurrStack(NeuroIndex ws) {
  if (ws == stack_set_.curr)
    return;
  stack_set_.old = stack_set_.curr;
  stack_set_.curr = ws % stack_set_.size;
}

void NeuroCoreSetCurrClient(NeuroClientPtrPtr c) {
  if (!c)
    return;
  set_curr_node((Node *)c);
}

// First, search in the current stack, if it is not there, search in the other stacks
NeuroClientPtrPtr NeuroCoreFindClient(const NeuroClientTesterFn ctf, const void *p) {
  NeuroClientPtrPtr c = NeuroCoreStackFindClient(stack_set_.curr, ctf, p);
  if (c)
    return c;
  for (NeuroIndex i = 0U; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    c = NeuroCoreStackFindClient(i, ctf, p);
    if (c)
      return c;
  }
  return NULL;
}

// First, search in the current stack, if is not there, search in the other stacks
NeuroClientPtrPtr NeuroCoreFindNspClient(void) {
  Node *n = stack_set_.stack_list[ stack_set_.curr ].nsp;
  if (n)
    return (NeuroClientPtrPtr)n;
  for (NeuroIndex i = 0U; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    n = stack_set_.stack_list[ i ].nsp;
    if (n)
      return (NeuroClientPtrPtr)n;
  }
  return NULL;
}

NeuroClientPtrPtr NeuroCoreAddClientEnd(NeuroClient *c) {
  if (!c)
    return NULL;
  Stack *const s = stack_set_.stack_list + c->ws;
  Node *const n = new_node(c);
  if (!n)
    return NULL;
  if (c->is_nsp)
    s->nsp = n;
  if (s->size < 1) {
    s->head = n;
    s->last = n;
    set_curr_node(n);
  } else {
    n->prev = s->curr;
    if (s->curr->next)
      s->curr->next->prev = n;
    else
      s->last = n;
    n->next = s->curr->next;
    s->curr->next = n;
    set_curr_node(n);
  }
  s->size++;
  return (NeuroClientPtrPtr)n;
}

NeuroClientPtrPtr NeuroCoreAddClientStart(NeuroClient *c) {
  if (!c)
    return NULL;
  Stack *const s = stack_set_.stack_list + c->ws;
  Node *const n = new_node(c);
  if (!n)
    return NULL;
  if (c->is_nsp)
    s->nsp = n;
  if (s->size < 1) {
    s->head = n;
    s->last = n;
    set_curr_node(n);
  } else {
    n->next = s->curr;
    if (s->curr->prev)
      s->curr->prev->next = n;
    else
      s->head = n;
    n->prev = s->curr->prev;
    s->curr->prev = n;
    set_curr_node(n);
  }
  s->size++;
  return (NeuroClientPtrPtr)n;
}

// NOTE: it only removes the Node, you must eventually free the return value with NeuroTypeDeleteClient
NeuroClient *NeuroCoreRemoveClient(NeuroClientPtrPtr c) {
  if (!c)
    return NULL;

  return NeuroCoreClientIsLast(c) ? remove_last_node(stack_set_.stack_list + NEURO_CLIENT_PTR(c)->ws) :
      remove_no_last_node((Node *)c);
}

NeuroClient *NeuroCorePushMinimizedClient(NeuroClient *c) {
  if (!c)
    return NULL;

  Stack *const s = stack_set_.stack_list + (c->ws % stack_set_.size);
  return push_minimized_client(s, c);
}

NeuroClient *NeuroCorePopMinimizedClient(NeuroIndex ws) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return pop_minimized_client(s);
}

// First, search in the current stack, if is not there, search in the other stacks
NeuroClient *NeuroCoreRemoveMinimizedClient(Window w) {
  Stack *const s = stack_set_.stack_list + stack_set_.curr;
  NeuroClient *c = remove_minimized_client(s, w);
  if (c)
    return c;
  for (NeuroIndex i = 0U; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    c = remove_minimized_client(stack_set_.stack_list + i, w);
    if (c)
      return c;
  }
  return NULL;
}

bool NeuroCoreStackIsCurr(NeuroIndex ws) {
  return ws == stack_set_.curr;
}

bool NeuroCoreStackIsNsp(NeuroIndex ws) {
  return ws == stack_set_.size - 1;
}

bool NeuroCoreStackIsEmpty(NeuroIndex ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].size <= 0;
}

const NeuroMonitor *NeuroCoreStackGetMonitor(NeuroIndex ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].monitor;
}

void NeuroCoreStackSetMonitor(NeuroIndex ws, const NeuroMonitor *m) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  if (m) {
    memmove(&s->region, &m->region, sizeof(NeuroRectangle));
    s->gaps = m->gaps;
    // NeuroCoreStackSetMonitor(m->ws, NULL);
  } else {
    const NeuroRectangle *const hidden_region = NeuroSystemGetHiddenRegion();
    memmove(&s->region, hidden_region, sizeof(NeuroRectangle));
    s->gaps = NeuroSystemGetHiddenGaps();
  }
  s->monitor = m;
}

const char *NeuroCoreStackGetName(NeuroIndex ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].name;
}

NeuroIndex NeuroCoreStackGetSize(NeuroIndex ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].size;
}

NeuroIndex NeuroCoreStackGetMinimizedNum(NeuroIndex ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].num_minimized;
}

NeuroIndex NeuroCoreStackGetNumLayouts(NeuroIndex ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout ? s->num_toggled_layouts : s->num_layouts;
}

NeuroIndex NeuroCoreStackGetLayoutIdx(NeuroIndex ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout ? s->curr_toggled_layout_index : s->curr_layout_index;
}

bool NeuroCoreStackIsCurrToggledLayout(NeuroIndex ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout;
}

void NeuroCoreStackSetLayoutIdx(NeuroIndex ws, NeuroIndex i) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  s->curr_layout_index = i % s->num_layouts;
}

void NeuroCoreStackSetToggledLayout(NeuroIndex ws, NeuroIndex *i) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  if (i) {
    s->curr_toggled_layout_index = *i % s->num_toggled_layouts;
    s->is_toggled_layout = true;
  } else {
    s->is_toggled_layout = false;
  }
}

NeuroLayout *NeuroCoreStackGetLayout(NeuroIndex ws, NeuroIndex i) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout ? s->toggled_layouts + (i % s->num_toggled_layouts) : s->layouts + (i % s->num_layouts);
}

const NeuroLayoutConf *NeuroCoreStackGetLayoutConf(NeuroIndex ws, NeuroIndex i) {
  assert(NeuroConfigGet()->workspace_list);
  const NeuroIndex index = ws % stack_set_.size;
  const Stack *const s = stack_set_.stack_list + index;
  const NeuroWorkspace *const w = NeuroConfigGet()->workspace_list[ index ];
  return s->is_toggled_layout ? w->toggled_layouts[ i % s->num_toggled_layouts ] : w->layouts[ i % s->num_layouts ];
}

NeuroLayout *NeuroCoreStackGetCurrLayout(NeuroIndex ws) {
  return NeuroCoreStackGetLayout(ws, NeuroCoreStackGetLayoutIdx(ws));
}

const NeuroLayoutConf *NeuroCoreStackGetCurrLayoutConf(NeuroIndex ws) {
  return NeuroCoreStackGetLayoutConf(ws, NeuroCoreStackGetLayoutIdx(ws));
}

NeuroRectangle *NeuroCoreStackGetRegion(NeuroIndex ws) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return &s->region;
}

const int *NeuroCoreStackGetGaps(NeuroIndex ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->gaps;
}

NeuroClientPtrPtr NeuroCoreStackGetCurrClient(NeuroIndex ws) {
  return (NeuroClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].curr);
}

NeuroClientPtrPtr NeuroCoreStackGetPrevClient(NeuroIndex ws) {
  return (NeuroClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].prev);
}

NeuroClientPtrPtr NeuroCoreStackGetHeadClient(NeuroIndex ws) {
  return (NeuroClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].head);
}

NeuroClientPtrPtr NeuroCoreStackGetLastClient(NeuroIndex ws) {
  return (NeuroClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].last);
}

NeuroClientPtrPtr NeuroCoreStackFindClient(NeuroIndex ws, const NeuroClientTesterFn ctf, const void *data) {
  assert(ctf);
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  for (Node *n = s->head; n; n = n->next)
    if (ctf((NeuroClientPtrPtr)n, data))
      return (NeuroClientPtrPtr)n;
  return NULL;
}

// Client
bool NeuroCoreClientIsCurr(const NeuroClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ NEURO_CLIENT_PTR(c)->ws ].curr;
}

bool NeuroCoreClientIsPrev(const NeuroClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ NEURO_CLIENT_PTR(c)->ws ].prev;
}

bool NeuroCoreClientIsHead(const NeuroClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ NEURO_CLIENT_PTR(c)->ws ].head;
}

bool NeuroCoreClientIsLast(const NeuroClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ NEURO_CLIENT_PTR(c)->ws ].last;
}

NeuroRectangle *NeuroCoreClientGetRegion(const NeuroClientPtrPtr c) {
  Node *const n = (Node *)c;
  return !c ? NULL : &n->region;
}

NeuroClientPtrPtr NeuroCoreClientGetNext(const NeuroClientPtrPtr c) {
  return !c ? NULL : (NeuroClientPtrPtr)(((Node *)c)->next);
}

NeuroClientPtrPtr NeuroCoreClientGetPrev(const NeuroClientPtrPtr c) {
  return !c ? NULL : (NeuroClientPtrPtr)(((Node *)c)->prev);
}

NeuroClientPtrPtr NeuroCoreClientSwap(const NeuroClientPtrPtr c1, const NeuroClientPtrPtr c2) {
  if (!c1 || !c2 || c1 == c2)
    return NULL;
  Node *const n1 = (Node *)c1;
  Node *const n2 = (Node *)c2;
  NeuroClient *const t = n1->cli;
  n1->cli = n2->cli;
  n2->cli = t;
  return c2;
}

