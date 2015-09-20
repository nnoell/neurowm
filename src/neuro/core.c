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

static StackSet SS;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static void update_nsp_stack(Stack *s) {
  assert(s);
  Node *n;
  for (n=s->head; n; n=n->next)
    if (n->cli->isNSP) {
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
  memmove(&(n->region), &(c->floatRegion), sizeof(Rectangle));
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
  Stack *s = SS.stacks + n->cli->ws;
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
  Bool updateNSP = t->cli->isNSP;
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
  Stack *s = SS.stacks + n->cli->ws;
  if (s->size == 0 || s->last == n)
    return NULL;
  Bool updateNSP = n->cli->isNSP;
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
    *(ArrangerFn *)&l[ i ].arrangerFn = lc[ i ]->arrangerFn;
    *(ColorSetterFn *)&l[ i ].borderColorSetterFn = lc[ i ]->borderColorSetterFn;
    *(BorderSetterFn *)&l[ i ].borderWidthSetterFn = lc[ i ]->borderWidthSetterFn;
    *(BorderSetterFn *)&l[ i ].borderGapSetterFn = lc[ i ]->borderGapSetterFn;
    *(const float **)&l[ i ].region = lc[ i ]->region;
    l[ i ].mod = lc[ i ]->mod;
    l[ i ].followMouse = lc[ i ]->followMouse;
    memmove(l[ i ].arrangeSettings, lc[ i ]->arrangeSettings, sizeof(GenericArg)*ARRSET_MAX);
  }
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// StackSet
Bool NeuroCoreInit() {
  assert(workspaceSetS);
  size_t size = NeuroTypeArrayLength((const void *const *const)workspaceSetS);
  SS.stacks = new_stackset(size + 1);  // We need on extra stack for NSP
  if (!SS.stacks)
    return False;
  const Workspace *ws;
  size_t i, sizel, sizetl;
  for (i = 0; workspaceSetS[ i ]; ++i) {
    ws = workspaceSetS[ i ];
    sizel = NeuroTypeArrayLength((const void *const *const)(ws->layouts));
    if (!sizel)
      return False;
    sizetl = NeuroTypeArrayLength((const void *const *const)(ws->togLayouts));
    Stack *s = new_stack(SS.stacks + i, sizel, sizetl);
    if (!s)
      return False;
    s->name = ws->name;
    s->curr = NULL; s->prev = NULL; s->head = NULL; s->last = NULL; s->nsp = NULL;
    s->size = 0;
    NeuroGeometryGetAbsoluteRelativeRectangle(&s->region, &screenRegion, ws->gaps);
    s->currLayoutIdx = 0;
    s->currTogLayoutIdx = -1;  // No toggled layout by default
    *(int *)&(s->numLayouts) = sizel;
    *(int *)&(s->numTogLayouts) = sizetl;
    set_layouts(s->layouts, ws->layouts, sizel);
    set_layouts(s->togLayouts, ws->togLayouts, sizetl);
  }
  SS.curr = 0;
  SS.old = 0;
  SS.size = size;
  return True;
}

void NeuroCoreStop() {
  int i;
  for (i = 0; i < SS.size; ++i)
    delete_stack(SS.stacks + i);
  delete_stack_set(SS.stacks);
  SS.stacks = NULL;
}

int NeuroCoreGetCurrStack() {
  return SS.curr;
}

int NeuroCoreGetPrevStack() {
  return SS.curr - 1 < 0 ? SS.size -1 : SS.curr - 1;
}

int NeuroCoreGetNextStack() {
  return SS.curr + 1 >= SS.size ? 0 : SS.curr + 1;
}

int NeuroCoreGetOldStack() {
  return SS.old;
}

int NeuroCoreGetNspStack() {
  return SS.size;
}

int NeuroCoreGetSize() {
  return SS.size;
}

int NeuroCoreGetNspStackSize() {
  return SS.stacks[ SS.size ].size;
}

ClientPtrPtr NeuroCoreGetCurrClientNspStack() {
  return (ClientPtrPtr)SS.stacks[ SS.size ].curr;
}

void NeuroCoreSetCurrStack(int ws) {
  SS.old = SS.curr;
  SS.curr = ws % SS.size;
}

void NeuroCoreSetCurrClient(const ClientPtrPtr c) {
  if (!c)
    return;
  set_curr_node((Node *)c);
}

// First off, search in the current stack, if it is not there, search in the other stacks
ClientPtrPtr NeuroCoreFindClient(const ClientTesterFn ctf, const void *p) {
  ClientPtrPtr c = NeuroCoreStackFindClient(SS.curr, ctf, p);
  if (c)
    return c;
  int i;
  for (i = 0; i < SS.size; ++i) {
    if (i == SS.curr)
      continue;
    c = NeuroCoreStackFindClient(i, ctf, p);
    if (c)
      return c;
  }
  return NULL;
}

// First, search in the current stack, if is not there, search in the other stacks
ClientPtrPtr NeuroCoreFindNspClient() {
  Node *n = SS.stacks[ SS.curr ].nsp;
  if (n)
    return (ClientPtrPtr)n;
  int i;
  for (i = 0; i < SS.size; ++i) {
    if (i == SS.curr)
      continue;
    n = SS.stacks[ i ].nsp;
    if (n)
      return (ClientPtrPtr)n;
  }
  return NULL;
}

ClientPtrPtr NeuroCoreAddClientEnd(const Client *c) {
  if (!c)
    return NULL;
  Stack *s = SS.stacks + c->ws;
  Node *n = new_node(c);
  if (!n)
    return NULL;
  if (c->isNSP)
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
  Stack *s = SS.stacks + c->ws;
  Node *n = new_node(c);
  if (!n)
    return NULL;
  if (c->isNSP)
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
    return remove_last_node(SS.stacks + CLI_GET(c).ws);
  return remove_no_last_node((Node *)c);
}

Client *NeuroCorePushMinimizedClient(Client *c) {
  assert(c);
  Stack *s = SS.stacks + (c->ws % SS.size);
  return push_minimized_client(s, c);
}

Client *NeuroCorePopMinimizedClient(int ws) {
  Stack *s = SS.stacks + (ws % SS.size);
  return pop_minimized_client(s);
}

// First, search in the current stack, if is not there, search in the other stacks
Client *NeuroCoreRemoveMinimizedClient(Window w) {
  Stack *s = SS.stacks + SS.curr;
  Client *c = remove_minimized_client(s, w);
  if (c)
    return c;
  int i;
  for (i = 0; i < SS.size; ++i) {
    if (i == SS.curr)
      continue;
    c = remove_minimized_client(SS.stacks + i, w);
    if (c)
      return c;
  }
  return NULL;
}

Bool NeuroCoreStackIsCurr(int ws) {
  return ws == SS.curr;
}

Bool NeuroCoreStackIsNsp(int ws) {
  return ws == SS.size;
}

Bool NeuroCoreStackIsEmpty(int ws) {
  return SS.stacks[ ws % SS.size ].size <= 0;
}

const char *NeuroCoreStackGetName(int ws) {
  return SS.stacks[ ws % SS.size ].name;
}

int NeuroCoreStackGetSize(int ws) {
  return SS.stacks[ ws % SS.size ].size;
}

int NeuroCoreStackGetMinimizedNum(int ws) {
  return SS.stacks[ ws % SS.size ].minimizedNum;
}

int NeuroCoreStackGetNumLayouts(int ws) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return SS.stacks[ s ].numLayouts;
  return SS.stacks[ s ].numTogLayouts;
}

int NeuroCoreStackGetLayoutIdx(int ws) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return SS.stacks[ s ].currLayoutIdx;
  return SS.stacks[ s ].currTogLayoutIdx;
}

Bool NeuroCoreStackIsCurrToggledLayout(int ws) {
  int s = ws % SS.size;
  return SS.stacks[ s ].currTogLayoutIdx != -1;
}

void NeuroCoreStackSetLayoutIdx(int ws, int i) {
  int s = ws % SS.size;
  SS.stacks[ s ].currLayoutIdx = i % SS.stacks[ s ].numLayouts;
}

void NeuroCoreStackSetToggledLayout(int ws, int i) {
  int s = ws % SS.size;
  if (i != -1)
    SS.stacks[ s ].currTogLayoutIdx = i % SS.stacks[ s ].numTogLayouts;
  else
    SS.stacks[ s ].currTogLayoutIdx = -1;
}

Layout *NeuroCoreStackGetLayout(int ws, int i) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return SS.stacks[ s ].layouts + (i % SS.stacks[ s ].numLayouts);
  return SS.stacks[ s ].togLayouts + (i % SS.stacks[ s ].numTogLayouts);
}

const LayoutConf *NeuroCoreStackGetLayoutConf(int ws, int i) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return workspaceSetS[ s ]->layouts[ i % SS.stacks[ s ].numLayouts ];
  return workspaceSetS[ s ]->togLayouts[ i % SS.stacks[ s ].numTogLayouts ];
}

Layout *NeuroCoreStackGetCurrLayout(int ws) {
  return NeuroCoreStackGetLayout(ws, NeuroCoreStackGetLayoutIdx(ws));
}

const LayoutConf *NeuroCoreStackGetCurrLayoutConf(int ws) {
  return NeuroCoreStackGetLayoutConf(ws, NeuroCoreStackGetLayoutIdx(ws));
}

Rectangle *NeuroCoreStackGetRegion(int ws) {
  return &(SS.stacks[ ws % SS.size ].region);
}

ClientPtrPtr NeuroCoreStackGetCurrClient(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].curr);
}

ClientPtrPtr NeuroCoreStackGetPrevClient(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].prev);
}

ClientPtrPtr NeuroCoreStackGetHeadClient(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].head);
}

ClientPtrPtr NeuroCoreStackGetLastClient(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].last);
}

ClientPtrPtr NeuroCoreStackFindClient(int ws, const ClientTesterFn ctf, const void *data) {
  assert(tcfn);
  Stack *s = SS.stacks + (ws % SS.size);
  Node *n;
  for (n=s->head; n; n=n->next)
    if (ctf((ClientPtrPtr)n, data))
      return (ClientPtrPtr)n;
  return NULL;
}

// Client
Bool NeuroCoreClientIsCurr(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].curr;
}

Bool NeuroCoreClientIsPrev(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].prev;
}

Bool NeuroCoreClientIsHead(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].head;
}

Bool NeuroCoreClientIsLast(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].last;
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

