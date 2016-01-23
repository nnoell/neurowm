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
  Client *cli;
  Node *next;
  Node *prev;
  Rectangle region;
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
  size_t size;  // Number of clients the stack has
  const Monitor *monitor;
  const int *gaps;
  Rectangle region;
  size_t curr_layout_index;
  size_t curr_toggled_layout_index;
  bool is_toggled_layout;
  const size_t num_layouts;
  const size_t num_toggled_layouts;
  Layout *layouts;
  Layout *toggled_layouts;
  size_t num_minimized;  // Number of minimized clients
  Client **minimized_clients;  // List of minimized clients
  size_t minimized_size;  // Allocated size
};

// StackSet
typedef struct StackSet StackSet;
struct StackSet {
  Stack *stack_list;
  size_t curr;
  size_t old;  // Previouse selected workspace
  size_t size;  // Number of stacks the stackset has
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

static Node *new_node(const Client *c) {
  assert(c);
  Node *const n = (Node *)malloc(sizeof(Node));
  if (!n)
    return NULL;
  n->cli = (Client *)c;
  memmove(&(n->region), &(c->float_region), sizeof(Rectangle));
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

static Client *remove_last_node(Stack *s) {
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
  Client *const ret = t->cli;
  delete_node(t);
  s->size--;
  if (update_nsp)
    update_nsp_stack(s);
  return ret;
}

static Client *remove_no_last_node(Node *n) {
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
  Client *const ret = n->cli;
  delete_node(n);
  s->size--;
  if (update_nsp)
    update_nsp_stack(s);
  return ret;
}

static bool realloc_minimized_clients_if_necessary(Stack *s, size_t new_total) {
  assert(s);
  size_t new_size = STEP_SIZE_REALLOC;
  while (new_size < new_total)
    new_size += STEP_SIZE_REALLOC;
  if (new_size != s->minimized_size) {
    s->minimized_clients = (Client **)realloc(s->minimized_clients, new_size*sizeof(void *));
    if (!s->minimized_clients)
      return false;
    s->minimized_size = new_size;
  }
  return true;
}

static Client *push_minimized_client(Stack *s, Client *c) {
  assert(s);
  assert(c);
  const size_t new_total = s->num_minimized + 1;
  if (!realloc_minimized_clients_if_necessary(s, new_total))
    return NULL;
  s->num_minimized = new_total;
  s->minimized_clients[ new_total - 1 ] = c;
  return c;
}

static Client *pop_minimized_client(Stack *s) {
  assert(s);
  if (s->num_minimized == 0)
    return NULL;
  const size_t new_count = s->num_minimized - 1;
  Client *cli = s->minimized_clients[ new_count ];
  if (!realloc_minimized_clients_if_necessary(s, new_count))
    return NULL;
  s->num_minimized = new_count;
  return cli;
}

static Client *remove_minimized_client(Stack *s, Window w) {
  assert(s);
  Client *c, *found = NULL;
  for (size_t i = 0U; i < s->num_minimized; ++i) {
    c = s->minimized_clients[ i ];
    if (found)
      s->minimized_clients[ i-1 ] = c;
    if (c->win == w)
      found = c;
  }
  if (found)
    s->num_minimized--;
  return found;
}

static void set_layouts(Layout *layout, const LayoutConf *const *layout_conf, size_t size) {
  assert(layout);
  assert(layout_conf);
  for (size_t i = 0U; i < size; ++i) {
    Layout *const l = layout + i;
    const LayoutConf *const lc = layout_conf[ i ];
    *(ArrangerFn *)&l->arranger_fn = lc->arranger_fn;
    *(ColorSetterFn *)&l->border_color_setter_fn = lc->border_color_setter_fn;
    *(BorderSetterFn *)&l->border_width_setter_fn = lc->border_width_setter_fn;
    *(BorderSetterFn *)&l->border_gap_setter_fn = lc->border_gap_setter_fn;
    *(const float **)&l->region = lc->region;
    l->mod = lc->mod;
    l->follow_mouse = lc->follow_mouse;
    memmove(l->parameters, lc->parameters, sizeof(GenericArg)*ARRSET_MAX);
  }
}

static bool init_stack(Stack *s, const char *name, const LayoutConf *const *l, const LayoutConf *const *tl) {
  if (!s || !l || !tl)
    return false;

  // Get sizes
  const size_t size_l = NeuroTypeArrayLength((const void *const *)(l));
  if (size_l == 0U)
    return false;
  const size_t size_tl = NeuroTypeArrayLength((const void *const *)(tl));
  if (size_tl == 0U)
    return false;

  s->layouts = (Layout *)calloc(size_l, sizeof(Layout));
  if (!s->layouts)
    return false;
  s->toggled_layouts = (Layout *)calloc(size_tl, sizeof(Layout));
  if (!s->toggled_layouts)
    return false;

  s->minimized_clients = (Client **)calloc(STEP_SIZE_REALLOC, sizeof(void *));
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
  *(size_t *)&(s->num_layouts) = size_l;
  *(size_t *)&(s->num_toggled_layouts) = size_tl;

  // Set configuration
  s->name = name;
  set_layouts(s->layouts, l, size_l);
  set_layouts(s->toggled_layouts, tl, size_tl);

  return true;
}

static void stop_stack(Stack *s) {
  if (!s)
    return;
  Client *c;
  while ((c = remove_last_node(s)))
    NeuroTypeDeleteClient(c);
  free(s->layouts);
  s->layouts = NULL;
  free(s->toggled_layouts);
  s->toggled_layouts = NULL;
  while ((c = pop_minimized_client(s)))
    NeuroTypeDeleteClient(c);
  free(s->minimized_clients);
  s->minimized_clients = NULL;
}

static Stack *new_stack_list(size_t size) {
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
bool NeuroCoreInit() {
  // Allocate as many stacks as we need
  const Workspace *const *const workspace_list = NeuroConfigGet()->workspace_list;
  if (!workspace_list)
    return false;
  const size_t size = NeuroTypeArrayLength((const void *const *const)workspace_list);
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
  for (size_t i = 0U; workspace_list[ i ]; ++i) {
    const Workspace *const ws = workspace_list[ i ];
    Stack *const s = stack_set_.stack_list + i;

    // Initialize the stack
    if (!init_stack(s, ws->name, ws->layouts, ws->toggled_layouts))
      return false;

    // Set the monitors
    const Monitor *const m = NeuroMonitorFindDefault(i);
    if (m)
      stack_set_.curr = m->default_ws;
    NeuroCoreStackSetMonitor(i, m);
  }

  return true;
}

void NeuroCoreStop() {
  for (size_t i = 0U; i < stack_set_.size + 1; ++i)
    stop_stack(stack_set_.stack_list + i);
  delete_stack_list(stack_set_.stack_list);
  stack_set_.stack_list = NULL;
}

size_t NeuroCoreGetHeadStack() {
  return 0U;
}
size_t NeuroCoreGetLastStack() {
  return stack_set_.size - 1U;
}

size_t NeuroCoreGetCurrStack() {
  return stack_set_.curr;
}

size_t NeuroCoreGetPrevStack() {
  return (stack_set_.size + stack_set_.curr - 1U) % stack_set_.size;
}

size_t NeuroCoreGetNextStack() {
  return (stack_set_.curr + 1U) % stack_set_.size;
}

size_t NeuroCoreGetOldStack() {
  return stack_set_.old;
}

size_t NeuroCoreGetNspStack() {
  return stack_set_.size - 1U;  // NSP stack is always the last one
}

size_t NeuroCoreGetMonitorStack(const Monitor *m) {
  for (size_t i = 0U; i < stack_set_.size; ++i) {
    const Stack *const s = stack_set_.stack_list + i;
    if (s->monitor == m)
      return i;
  }
  return stack_set_.curr;  // It should never happen
}

size_t NeuroCoreGetSize() {
  return stack_set_.size;
}

size_t NeuroCoreGetNspStackSize() {
  return stack_set_.stack_list[ stack_set_.size - 1U ].size;
}

ClientPtrPtr NeuroCoreGetCurrClientNspStack() {
  return (ClientPtrPtr)stack_set_.stack_list[ stack_set_.size - 1U ].curr;
}

void NeuroCoreSetCurrStack(size_t ws) {
  if (ws == stack_set_.curr)
    return;
  stack_set_.old = stack_set_.curr;
  stack_set_.curr = ws % stack_set_.size;
}

void NeuroCoreSetCurrClient(const ClientPtrPtr c) {
  if (!c)
    return;
  set_curr_node((Node *)c);
}

// First off, search in the current stack, if it is not there, search in the other stacks
ClientPtrPtr NeuroCoreFindClient(const ClientTesterFn ctf, const void *p) {
  ClientPtrPtr c = NeuroCoreStackFindClient(stack_set_.curr, ctf, p);
  if (c)
    return c;
  for (size_t i = 0U; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    c = NeuroCoreStackFindClient(i, ctf, p);
    if (c)
      return c;
  }
  return NULL;
}

// First, search in the current stack, if is not there, search in the other stacks
ClientPtrPtr NeuroCoreFindNspClient() {
  Node *n = stack_set_.stack_list[ stack_set_.curr ].nsp;
  if (n)
    return (ClientPtrPtr)n;
  for (size_t i = 0U; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    n = stack_set_.stack_list[ i ].nsp;
    if (n)
      return (ClientPtrPtr)n;
  }
  return NULL;
}

ClientPtrPtr NeuroCoreAddClientEnd(const Client *c) {
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
  return (ClientPtrPtr)n;
}

ClientPtrPtr NeuroCoreAddClientStart(const Client *c) {
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
  return (ClientPtrPtr)n;
}

// NOTE: it only removes the Node, you must eventually free the return value with NeuroTypeDeleteClient
Client *NeuroCoreRemoveClient(ClientPtrPtr c) {
  if (!c)
    return NULL;
  return NeuroCoreClientIsLast(c) ? remove_last_node(stack_set_.stack_list + CLI_GET(c).ws) :
      remove_no_last_node((Node *)c);
}

Client *NeuroCorePushMinimizedClient(Client *c) {
  assert(c);
  Stack *const s = stack_set_.stack_list + (c->ws % stack_set_.size);
  return push_minimized_client(s, c);
}

Client *NeuroCorePopMinimizedClient(size_t ws) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return pop_minimized_client(s);
}

// First, search in the current stack, if is not there, search in the other stacks
Client *NeuroCoreRemoveMinimizedClient(Window w) {
  Stack *const s = stack_set_.stack_list + stack_set_.curr;
  Client *c = remove_minimized_client(s, w);
  if (c)
    return c;
  for (size_t i = 0U; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    c = remove_minimized_client(stack_set_.stack_list + i, w);
    if (c)
      return c;
  }
  return NULL;
}

bool NeuroCoreStackIsCurr(size_t ws) {
  return ws == stack_set_.curr;
}

bool NeuroCoreStackIsNsp(size_t ws) {
  return ws == stack_set_.size - 1;
}

bool NeuroCoreStackIsEmpty(size_t ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].size <= 0;
}

const Monitor *NeuroCoreStackGetMonitor(size_t ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].monitor;
}

void NeuroCoreStackSetMonitor(size_t ws, const Monitor *m) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  if (m) {
    NeuroGeometrySetRectangle(&s->region, m->region.x, m->region.y, m->region.w, m->region.h);
    s->gaps = m->gaps;
    // NeuroCoreStackSetMonitor(m->ws, NULL);
  } else {
    const Rectangle *const hidden_region = NeuroSystemGetHiddenRegion();
    NeuroGeometrySetRectangle(&s->region, hidden_region->x, hidden_region->y, hidden_region->w, hidden_region->h);
    s->gaps = NeuroSystemGetHiddenGaps();
  }
  s->monitor = m;
}

const char *NeuroCoreStackGetName(size_t ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].name;
}

size_t NeuroCoreStackGetSize(size_t ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].size;
}

size_t NeuroCoreStackGetMinimizedNum(size_t ws) {
  return stack_set_.stack_list[ ws % stack_set_.size ].num_minimized;
}

size_t NeuroCoreStackGetNumLayouts(size_t ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout ? s->num_toggled_layouts : s->num_layouts;
}

size_t NeuroCoreStackGetLayoutIdx(size_t ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout ? s->curr_toggled_layout_index : s->curr_layout_index;
}

bool NeuroCoreStackIsCurrToggledLayout(size_t ws) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout;
}

void NeuroCoreStackSetLayoutIdx(size_t ws, size_t i) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  s->curr_layout_index = i % s->num_layouts;
}

void NeuroCoreStackSetToggledLayout(size_t ws, size_t *i) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  if (i) {
    s->curr_toggled_layout_index = *i % s->num_toggled_layouts;
    s->is_toggled_layout = true;
  } else {
    s->is_toggled_layout = false;
  }
}

Layout *NeuroCoreStackGetLayout(size_t ws, size_t i) {
  const Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->is_toggled_layout ? s->toggled_layouts + (i % s->num_toggled_layouts) : s->layouts + (i % s->num_layouts);
}

const LayoutConf *NeuroCoreStackGetLayoutConf(size_t ws, size_t i) {
  assert(NeuroConfigGet()->workspace_list);
  const size_t index = ws % stack_set_.size;
  const Stack *const s = stack_set_.stack_list + index;
  const Workspace *const w = NeuroConfigGet()->workspace_list[ index ];
  return s->is_toggled_layout ? w->toggled_layouts[ i % s->num_toggled_layouts ] : w->layouts[ i % s->num_layouts ];
}

Layout *NeuroCoreStackGetCurrLayout(size_t ws) {
  return NeuroCoreStackGetLayout(ws, NeuroCoreStackGetLayoutIdx(ws));
}

const LayoutConf *NeuroCoreStackGetCurrLayoutConf(size_t ws) {
  return NeuroCoreStackGetLayoutConf(ws, NeuroCoreStackGetLayoutIdx(ws));
}

Rectangle *NeuroCoreStackGetRegion(size_t ws) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return &s->region;
}

const int *NeuroCoreStackGetGaps(size_t ws) {
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  return s->gaps;
}

ClientPtrPtr NeuroCoreStackGetCurrClient(size_t ws) {
  return (ClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].curr);
}

ClientPtrPtr NeuroCoreStackGetPrevClient(size_t ws) {
  return (ClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].prev);
}

ClientPtrPtr NeuroCoreStackGetHeadClient(size_t ws) {
  return (ClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].head);
}

ClientPtrPtr NeuroCoreStackGetLastClient(size_t ws) {
  return (ClientPtrPtr)(stack_set_.stack_list[ ws % stack_set_.size ].last);
}

ClientPtrPtr NeuroCoreStackFindClient(size_t ws, const ClientTesterFn ctf, const void *data) {
  assert(ctf);
  Stack *const s = stack_set_.stack_list + (ws % stack_set_.size);
  for (Node *n = s->head; n; n = n->next)
    if (ctf((ClientPtrPtr)n, data))
      return (ClientPtrPtr)n;
  return NULL;
}

// Client
bool NeuroCoreClientIsCurr(const ClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ CLI_GET(c).ws ].curr;
}

bool NeuroCoreClientIsPrev(const ClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ CLI_GET(c).ws ].prev;
}

bool NeuroCoreClientIsHead(const ClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ CLI_GET(c).ws ].head;
}

bool NeuroCoreClientIsLast(const ClientPtrPtr c) {
  return c && (Node *)c == stack_set_.stack_list[ CLI_GET(c).ws ].last;
}

Rectangle *NeuroCoreClientGetRegion(const ClientPtrPtr c) {
  Node *const n = (Node *)c;
  return !c ? NULL : &n->region;
}

ClientPtrPtr NeuroCoreClientGetNext(const ClientPtrPtr c) {
  return !c ? NULL : (ClientPtrPtr)(((Node *)c)->next);
}

ClientPtrPtr NeuroCoreClientGetPrev(const ClientPtrPtr c) {
  return !c ? NULL : (ClientPtrPtr)(((Node *)c)->prev);
}

ClientPtrPtr NeuroCoreClientSwap(const ClientPtrPtr c1, const ClientPtrPtr c2) {
  if (!c1 || !c2 || c1 == c2)
    return NULL;
  Node *const n1 = (Node *)c1;
  Node *const n2 = (Node *)c2;
  Client *const t = n1->cli;
  n1->cli = n2->cli;
  n2->cli = t;
  return c2;
}

