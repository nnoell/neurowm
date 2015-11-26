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
#include "system.h"
#include "geometry.h"
#include "core.h"

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
  int size;  // Number of clients the stack has
  Rectangle region;  // Region where all the layouts can be
  int currLayoutIdx;
  int currTogLayoutIdx;  // -1 if none
  const int numLayouts;
  const int numTogLayouts;
  Layout *layouts;
  Layout *togLayouts;
  int minimizedNum;  // Number of minimized clients
  Client **minimizedClients;  // List of minimized clients
  int minimizedSize;  // Allocated size
};

// StackSet
typedef struct StackSet StackSet;
struct StackSet {
  Stack *stacks;
  int curr;
  int old;   // Previouse selected workspace
  int size;  // Number of stacks the stackset has
};


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static StackSet stack_set_;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void update_nsp_stack(Stack *s) {
  assert(s);
  Node *n;
  for (n=s->head; n; n=n->next)
    if (n->cli->is_nsp) {
      s->nsp = n;
      return;
    }
  s->nsp = NULL;
}

static Node *new_node(const Client *c) {
  assert(c);
  Node *n = (Node *)malloc(sizeof(Node));
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
  Stack *s = stack_set_.stacks + n->cli->ws;
  if (n == s->curr)
    return;
  s->prev = s->curr;
  s->curr = n;
}

static Client *remove_last_node(Stack *s) {
  assert(s);
  if (s->size < 1)
    return NULL;
  Node *t = s->last;
  Bool updateNSP = t->cli->is_nsp;
  if (s->size == 1) {
    s->head = NULL;
    s->last = NULL;
    s->curr = NULL;
  } else {
    set_curr_node(s->last->prev);
    s->last->prev->next = NULL;
    s->last = t->prev;
  }
  Client *ret = t->cli;
  delete_node(t);
  s->size--;
  if (updateNSP)
    update_nsp_stack(s);
  return ret;
}

static Client *remove_no_last_node(Node *n) {
  assert(n);
  Stack *s = stack_set_.stacks + n->cli->ws;
  if (s->size == 0 || s->last == n)
    return NULL;
  Bool updateNSP = n->cli->is_nsp;
  set_curr_node(n->next);
  if (n == s->head) {
    s->head = n->next;
    n->next->prev = NULL;
  } else {
    n->prev->next = n->next;
    n->next->prev = n->prev;
  }
  Client *ret = n->cli;
  delete_node(n);
  s->size--;
  if (updateNSP)
    update_nsp_stack(s);
  return ret;
}

static Bool realloc_minimized_clients_if_necessary(Stack *s, int newCount) {
  assert(s);
  int newsize = STEP_SIZE_REALLOC;
  while (newsize < newCount)
    newsize += STEP_SIZE_REALLOC;
  if (newsize != s->minimizedSize) {
    s->minimizedClients = (Client **)realloc(s->minimizedClients, (size_t)newsize*sizeof(void *));
    if (!s->minimizedClients)
      return False;
    s->minimizedSize = newsize;
  }
  return True;
}

static Client *push_minimized_client(Stack *s, Client *c) {
  assert(s);
  assert(c);
  int newCount = s->minimizedNum + 1;
  if (!realloc_minimized_clients_if_necessary(s, newCount))
    return NULL;
  s->minimizedNum = newCount;
  s->minimizedClients[ newCount - 1 ] = c;
  return c;
}

static Client *pop_minimized_client(Stack *s) {
  assert(s);
  if (s->minimizedNum == 0)
    return NULL;
  int newCount = s->minimizedNum - 1;
  Client *cli = s->minimizedClients[ newCount ];
  if (!realloc_minimized_clients_if_necessary(s, newCount))
    return NULL;
  s->minimizedNum = newCount;
  return cli;
}

static Client *remove_minimized_client(Stack *s, Window w) {
  assert(s);
  Client *c, *found = NULL;
  int i;
  for (i = 0; i < s->minimizedNum; ++i) {
    c = s->minimizedClients[ i ];
    if (found)
      s->minimizedClients[ i-1 ] = c;
    if (c->win == w)
      found = c;
  }
  if (found)
    s->minimizedNum--;
  return found;
}

static Stack *new_stack(Stack *s, size_t sizel, size_t sizetl) {
  assert(s);
  s->layouts = (Layout *)calloc(sizel, sizeof(Layout));
  if (!s->layouts)
    return NULL;
  s->togLayouts = (Layout *)calloc(sizetl, sizeof(Layout));
  if (!s->togLayouts)
    return NULL;
  s->minimizedClients = (Client **)calloc(STEP_SIZE_REALLOC, sizeof(void *));
  if (!s->minimizedClients)
    return NULL;
  s->size = 0;
  s->minimizedNum = 0;
  s->minimizedSize = STEP_SIZE_REALLOC;
  return s;
}

static void delete_stack(Stack *s) {
  if (!s)
    return;
  Client *c;
  while ((c = remove_last_node(s)))
    NeuroTypeDeleteClient(c);
  free(s->layouts);
  s->layouts = NULL;
  free(s->togLayouts);
  s->togLayouts = NULL;
  while ((c = pop_minimized_client(s)))
    NeuroTypeDeleteClient(c);
  free(s->minimizedClients);
  s->minimizedClients = NULL;
}

static Stack *new_stackset(size_t size) {
  return (Stack *)calloc(size, sizeof(Stack));
}

static void delete_stack_set(Stack *s) {
  if (!s)
    return;
  free(s);
  s = NULL;
}

static void set_layouts(Layout *l, const LayoutConf *const *lc, size_t size) {
  assert(l);
  assert(lc);
  size_t i;
  for (i = 0; i < size; ++i) {
    *(ArrangerFn *)&l[ i ].arranger_fn = lc[ i ]->arranger_fn;
    *(ColorSetterFn *)&l[ i ].border_color_setter_fn = lc[ i ]->border_color_setter_fn;
    *(BorderSetterFn *)&l[ i ].border_width_setter_fn = lc[ i ]->border_width_setter_fn;
    *(BorderSetterFn *)&l[ i ].border_gap_setter_fn = lc[ i ]->border_gap_setter_fn;
    *(const float **)&l[ i ].region = lc[ i ]->region;
    l[ i ].mod = lc[ i ]->mod;
    l[ i ].follow_mouse = lc[ i ]->follow_mouse;
    memmove(l[ i ].arrange_settings, lc[ i ]->arrange_settings, sizeof(GenericArg)*ARRSET_MAX);
  }
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// StackSet
Bool NeuroCoreInit() {
  // Allocate as many stacks as we need
  const Workspace *const *const workspaceSet = NeuroSystemGetConfiguration()->workspace_set;
  size_t size = NeuroTypeArrayLength((const void *const *const)workspaceSet);
  stack_set_.stacks = new_stackset(size + 1);  // We need on extra stack for NSP
  if (!stack_set_.stacks)
    return False;

  // Initialize the stackset
  stack_set_.curr = 0;
  stack_set_.old = 0;
  stack_set_.size = size;

  // Initialize the stacks
  const Workspace *ws;
  size_t i, sizel, sizetl;
  for (i = 0; workspaceSet[ i ]; ++i) {
    ws = workspaceSet[ i ];
    sizel = NeuroTypeArrayLength((const void *const *const)(ws->layouts));
    if (!sizel)
      return False;
    sizetl = NeuroTypeArrayLength((const void *const *const)(ws->toggled_layouts));
    Stack *s = new_stack(stack_set_.stacks + i, sizel, sizetl);
    if (!s)
      return False;
    s->name = ws->name;
    s->curr = NULL; s->prev = NULL; s->head = NULL; s->last = NULL; s->nsp = NULL;
    s->size = 0;
    NeuroGeometryGetAbsoluteRelativeRectangle(&s->region, NeuroSystemGetScreenRegion(), ws->gaps);
    s->currLayoutIdx = 0;
    s->currTogLayoutIdx = -1;  // No toggled layout by default
    *(int *)&(s->numLayouts) = sizel;
    *(int *)&(s->numTogLayouts) = sizetl;
    set_layouts(s->layouts, ws->layouts, sizel);
    set_layouts(s->togLayouts, ws->toggled_layouts, sizetl);
  }
  return True;
}

void NeuroCoreStop() {
  int i;
  for (i = 0; i < stack_set_.size; ++i)
    delete_stack(stack_set_.stacks + i);
  delete_stack_set(stack_set_.stacks);
  stack_set_.stacks = NULL;
}

int NeuroCoreGetCurrStack() {
  return stack_set_.curr;
}

int NeuroCoreGetPrevStack() {
  return stack_set_.curr - 1 < 0 ? stack_set_.size -1 : stack_set_.curr - 1;
}

int NeuroCoreGetNextStack() {
  return stack_set_.curr + 1 >= stack_set_.size ? 0 : stack_set_.curr + 1;
}

int NeuroCoreGetOldStack() {
  return stack_set_.old;
}

int NeuroCoreGetNspStack() {
  return stack_set_.size;
}

int NeuroCoreGetSize() {
  return stack_set_.size;
}

int NeuroCoreGetNspStackSize() {
  return stack_set_.stacks[ stack_set_.size ].size;
}

ClientPtrPtr NeuroCoreGetCurrClientNspStack() {
  return (ClientPtrPtr)stack_set_.stacks[ stack_set_.size ].curr;
}

void NeuroCoreSetCurrStack(int ws) {
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
  int i;
  for (i = 0; i < stack_set_.size; ++i) {
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
  Node *n = stack_set_.stacks[ stack_set_.curr ].nsp;
  if (n)
    return (ClientPtrPtr)n;
  int i;
  for (i = 0; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    n = stack_set_.stacks[ i ].nsp;
    if (n)
      return (ClientPtrPtr)n;
  }
  return NULL;
}

ClientPtrPtr NeuroCoreAddClientEnd(const Client *c) {
  if (!c)
    return NULL;
  Stack *s = stack_set_.stacks + c->ws;
  Node *n = new_node(c);
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
  Stack *s = stack_set_.stacks + c->ws;
  Node *n = new_node(c);
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

// NOTE: it only removes the Node, you must eventually free the return value with freeClientSS
Client *NeuroCoreRemoveClient(ClientPtrPtr c) {
  if (!c)
    return NULL;
  if (NeuroCoreClientIsLast(c))
    return remove_last_node(stack_set_.stacks + CLI_GET(c).ws);
  return remove_no_last_node((Node *)c);
}

Client *NeuroCorePushMinimizedClient(Client *c) {
  assert(c);
  Stack *s = stack_set_.stacks + (c->ws % stack_set_.size);
  return push_minimized_client(s, c);
}

Client *NeuroCorePopMinimizedClient(int ws) {
  Stack *s = stack_set_.stacks + (ws % stack_set_.size);
  return pop_minimized_client(s);
}

// First, search in the current stack, if is not there, search in the other stacks
Client *NeuroCoreRemoveMinimizedClient(Window w) {
  Stack *s = stack_set_.stacks + stack_set_.curr;
  Client *c = remove_minimized_client(s, w);
  if (c)
    return c;
  int i;
  for (i = 0; i < stack_set_.size; ++i) {
    if (i == stack_set_.curr)
      continue;
    c = remove_minimized_client(stack_set_.stacks + i, w);
    if (c)
      return c;
  }
  return NULL;
}

Bool NeuroCoreStackIsCurr(int ws) {
  return ws == stack_set_.curr;
}

Bool NeuroCoreStackIsNsp(int ws) {
  return ws == stack_set_.size;
}

Bool NeuroCoreStackIsEmpty(int ws) {
  return stack_set_.stacks[ ws % stack_set_.size ].size <= 0;
}

const char *NeuroCoreStackGetName(int ws) {
  return stack_set_.stacks[ ws % stack_set_.size ].name;
}

int NeuroCoreStackGetSize(int ws) {
  return stack_set_.stacks[ ws % stack_set_.size ].size;
}

int NeuroCoreStackGetMinimizedNum(int ws) {
  return stack_set_.stacks[ ws % stack_set_.size ].minimizedNum;
}

int NeuroCoreStackGetNumLayouts(int ws) {
  int s = ws % stack_set_.size;
  if (stack_set_.stacks[ s ].currTogLayoutIdx == -1)
    return stack_set_.stacks[ s ].numLayouts;
  return stack_set_.stacks[ s ].numTogLayouts;
}

int NeuroCoreStackGetLayoutIdx(int ws) {
  int s = ws % stack_set_.size;
  if (stack_set_.stacks[ s ].currTogLayoutIdx == -1)
    return stack_set_.stacks[ s ].currLayoutIdx;
  return stack_set_.stacks[ s ].currTogLayoutIdx;
}

Bool NeuroCoreStackIsCurrToggledLayout(int ws) {
  int s = ws % stack_set_.size;
  return stack_set_.stacks[ s ].currTogLayoutIdx != -1;
}

void NeuroCoreStackSetLayoutIdx(int ws, int i) {
  int s = ws % stack_set_.size;
  stack_set_.stacks[ s ].currLayoutIdx = i % stack_set_.stacks[ s ].numLayouts;
}

void NeuroCoreStackSetToggledLayout(int ws, int i) {
  int s = ws % stack_set_.size;
  if (i != -1)
    stack_set_.stacks[ s ].currTogLayoutIdx = i % stack_set_.stacks[ s ].numTogLayouts;
  else
    stack_set_.stacks[ s ].currTogLayoutIdx = -1;
}

Layout *NeuroCoreStackGetLayout(int ws, int i) {
  int s = ws % stack_set_.size;
  if (stack_set_.stacks[ s ].currTogLayoutIdx == -1)
    return stack_set_.stacks[ s ].layouts + (i % stack_set_.stacks[ s ].numLayouts);
  return stack_set_.stacks[ s ].togLayouts + (i % stack_set_.stacks[ s ].numTogLayouts);
}

const LayoutConf *NeuroCoreStackGetLayoutConf(int ws, int i) {
  const Workspace *const *const workspaceSet = NeuroSystemGetConfiguration()->workspace_set;
  int s = ws % stack_set_.size;
  if (stack_set_.stacks[ s ].currTogLayoutIdx == -1)
    return workspaceSet[ s ]->layouts[ i % stack_set_.stacks[ s ].numLayouts ];
  return workspaceSet[ s ]->toggled_layouts[ i % stack_set_.stacks[ s ].numTogLayouts ];
}

Layout *NeuroCoreStackGetCurrLayout(int ws) {
  return NeuroCoreStackGetLayout(ws, NeuroCoreStackGetLayoutIdx(ws));
}

const LayoutConf *NeuroCoreStackGetCurrLayoutConf(int ws) {
  return NeuroCoreStackGetLayoutConf(ws, NeuroCoreStackGetLayoutIdx(ws));
}

Rectangle *NeuroCoreStackGetRegion(int ws) {
  return &(stack_set_.stacks[ ws % stack_set_.size ].region);
}

ClientPtrPtr NeuroCoreStackGetCurrClient(int ws) {
  return (ClientPtrPtr)(stack_set_.stacks[ ws % stack_set_.size ].curr);
}

ClientPtrPtr NeuroCoreStackGetPrevClient(int ws) {
  return (ClientPtrPtr)(stack_set_.stacks[ ws % stack_set_.size ].prev);
}

ClientPtrPtr NeuroCoreStackGetHeadClient(int ws) {
  return (ClientPtrPtr)(stack_set_.stacks[ ws % stack_set_.size ].head);
}

ClientPtrPtr NeuroCoreStackGetLastClient(int ws) {
  return (ClientPtrPtr)(stack_set_.stacks[ ws % stack_set_.size ].last);
}

ClientPtrPtr NeuroCoreStackFindClient(int ws, const ClientTesterFn ctf, const void *data) {
  assert(tcfn);
  Stack *s = stack_set_.stacks + (ws % stack_set_.size);
  Node *n;
  for (n=s->head; n; n=n->next)
    if (ctf((ClientPtrPtr)n, data))
      return (ClientPtrPtr)n;
  return NULL;
}

// Client
Bool NeuroCoreClientIsCurr(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == stack_set_.stacks[ CLI_GET(c).ws ].curr;
}

Bool NeuroCoreClientIsPrev(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == stack_set_.stacks[ CLI_GET(c).ws ].prev;
}

Bool NeuroCoreClientIsHead(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == stack_set_.stacks[ CLI_GET(c).ws ].head;
}

Bool NeuroCoreClientIsLast(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == stack_set_.stacks[ CLI_GET(c).ws ].last;
}

Rectangle *NeuroCoreClientGetRegion(const ClientPtrPtr c) {
  Node *n = (Node *)c;
  return !c ? NULL : &n->region;
}

ClientPtrPtr NeuroCoreClientGetNext(const ClientPtrPtr c) {
  return !c ? NULL : (ClientPtrPtr)(((Node *)c)->next);
}

ClientPtrPtr NeuroCoreClientGetPrev(const ClientPtrPtr c) {
  return !c ? NULL : (ClientPtrPtr)(((Node *)c)->prev);
}

ClientPtrPtr NeuroCoreClientSwap(const ClientPtrPtr c1, const ClientPtrPtr c2) {
  if (!c1 || !c2)
    return NULL;
  if (c1 == c2)
    return NULL;
  Node *n1 = (Node *)c1;
  Node *n2 = (Node *)c2;
  Client *t = n1->cli;
  n1->cli = n2->cli;
  n2->cli = t;
  return c2;
}

