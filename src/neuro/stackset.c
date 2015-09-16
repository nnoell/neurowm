//----------------------------------------------------------------------------------------------------------------------
// Module      :  stackset
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
#include "stackset.h"

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

static void updateNSPStack(Stack *s) {
  assert(s);
  Node *n;
  for (n=s->head; n; n=n->next)
    if (n->cli->isNSP) {
      s->nsp = n;
      return;
    }
  s->nsp = NULL;
}

static Node *allocNode(const Client *c) {
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

static void freeNode(Node *n) {
  if (!n)
    return;
  free(n);
  n = NULL;
}

static void setCurrNode(Node *n) {
  assert(n);
  Stack *s = SS.stacks + n->cli->ws;
  if (n == s->curr)
    return;
  s->prev = s->curr;
  s->curr = n;
}

static Client *rmvLastNode(Stack *s) {
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
    setCurrNode(s->last->prev);
    s->last->prev->next = NULL;
    s->last = t->prev;
  }
  Client *ret = t->cli;
  freeNode(t);
  s->size--;
  if (updateNSP)
    updateNSPStack(s);
  return ret;
}

static Client *rmvNoLastNode(Node *n) {
  assert(n);
  Stack *s = SS.stacks + n->cli->ws;
  if (s->size == 0 || s->last == n)
    return NULL;
  Bool updateNSP = n->cli->isNSP;
  setCurrNode(n->next);
  if (n == s->head) {
    s->head = n->next;
    n->next->prev = NULL;
  } else {
    n->prev->next = n->next;
    n->next->prev = n->prev;
  }
  Client *ret = n->cli;
  freeNode(n);
  s->size--;
  if (updateNSP)
    updateNSPStack(s);
  return ret;
}

static Bool reallocMinimizedClientsIfNecessary(Stack *s, int newCount) {
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

Client *pushMinimizedCliStack(Stack *s, Client *c) {
  assert(s);
  assert(c);
  int newCount = s->minimizedNum + 1;
  if (!reallocMinimizedClientsIfNecessary(s, newCount))
    return NULL;
  s->minimizedNum = newCount;
  s->minimizedClients[ newCount - 1 ] = c;
  return c;
}

Client *popMinimizedCliStack(Stack *s) {
  assert(s);
  if (s->minimizedNum == 0)
    return NULL;
  int newCount = s->minimizedNum - 1;
  Client *cli = s->minimizedClients[ newCount ];
  if (!reallocMinimizedClientsIfNecessary(s, newCount))
    return NULL;
  s->minimizedNum = newCount;
  return cli;
}

static Client *rmvMinimizedCliStack(Stack *s, Window w) {
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

static Stack *allocStack(Stack *s, size_t sizel, size_t sizetl) {
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

static void freeStack(Stack *s) {
  if (!s)
    return;
  Client *c;
  while ((c=rmvLastNode(s)))
    freeClientT(c);
  free(s->layouts);
  s->layouts = NULL;
  free(s->togLayouts);
  s->togLayouts = NULL;
  while ((c = popMinimizedCliStack(s)))
    freeClientT(c);
  free(s->minimizedClients);
  s->minimizedClients = NULL;
}

static Stack *allocStackSet(size_t size) {
  return (Stack *)calloc(size, sizeof(Stack));
}

static void freeStackSet(Stack *s) {
  if (!s)
    return;
  free(s);
  s = NULL;
}

static void initLayouts(Layout *l, const LayoutConf *const *lc, size_t size) {
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
Bool initSS() {
  assert(workspaceSetS);
  size_t size = ptrArrayLengthT((const void *const *const)workspaceSetS);
  SS.stacks = allocStackSet(size + 1);  // We need on extra stack for NSP
  if (!SS.stacks)
    return False;
  const Workspace *ws;
  size_t i, sizel, sizetl;
  for (i = 0; workspaceSetS[ i ]; ++i) {
    ws = workspaceSetS[ i ];
    sizel = ptrArrayLengthT((const void *const *const)(ws->layouts));
    if (!sizel)
      return False;
    sizetl = ptrArrayLengthT((const void *const *const)(ws->togLayouts));
    Stack *s = allocStack(SS.stacks + i, sizel, sizetl);
    if (!s)
      return False;
    s->name = ws->name;
    s->curr = NULL; s->prev = NULL; s->head = NULL; s->last = NULL; s->nsp = NULL;
    s->size = 0;
    getAbsoluteRelativeRectangleG(&s->region, &screenRegion, ws->gaps);
    s->currLayoutIdx = 0;
    s->currTogLayoutIdx = -1;  // No toggled layout by default
    *(int *)&(s->numLayouts) = sizel;
    *(int *)&(s->numTogLayouts) = sizetl;
    initLayouts(s->layouts, ws->layouts, sizel);
    initLayouts(s->togLayouts, ws->togLayouts, sizetl);
  }
  SS.curr = 0;
  SS.old = 0;
  SS.size = size;
  return True;
}

void endSS() {
  int i;
  for (i = 0; i < SS.size; ++i)
    freeStack(SS.stacks + i);
  freeStackSet(SS.stacks);
  SS.stacks = NULL;
}

int getCurrStackSS() {
  return SS.curr;
}

int getPrevStackSS() {
  return SS.curr - 1 < 0 ? SS.size -1 : SS.curr - 1;
}

int getNextStackSS() {
  return SS.curr + 1 >= SS.size ? 0 : SS.curr + 1;
}

int getOldStackSS() {
  return SS.old;
}

int getNSPStackSS() {
  return SS.size;
}

int getSizeSS() {
  return SS.size;
}

int getSizeNSPSS() {
  return SS.stacks[ SS.size ].size;
}

ClientPtrPtr getCurrClientNSPStackSS() {
  return (ClientPtrPtr)SS.stacks[ SS.size ].curr;
}

void setCurrStackSS(int ws) {
  SS.old = SS.curr;
  SS.curr = ws % SS.size;
}

void setCurrClientSS(const ClientPtrPtr c) {
  if (!c)
    return;
  setCurrNode((Node *)c);
}

// First off, search in the current stack, if it is not there, search in the other stacks
ClientPtrPtr findClientSS(const ClientTesterFn ctf, const void *p) {
  ClientPtrPtr c = findClientStackSS(SS.curr, ctf, p);
  if (c)
    return c;
  int i;
  for (i = 0; i < SS.size; ++i) {
    if (i == SS.curr)
      continue;
    c = findClientStackSS(i, ctf, p);
    if (c)
      return c;
  }
  return NULL;
}

// First, search in the current stack, if is not there, search in the other stacks
ClientPtrPtr findNSPClientSS() {
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

ClientPtrPtr addClientEndSS(const Client *c) {
  if (!c)
    return NULL;
  Stack *s = SS.stacks + c->ws;
  Node *n = allocNode(c);
  if (!n)
    return NULL;
  if (c->isNSP)
    s->nsp = n;
  if (s->size < 1) {
    s->head = n;
    s->last = n;
    setCurrNode(n);
  } else {
    n->prev = s->curr;
    if (s->curr->next)
      s->curr->next->prev = n;
    else
      s->last = n;
    n->next = s->curr->next;
    s->curr->next = n;
    setCurrNode(n);
  }
  s->size++;
  return (ClientPtrPtr)n;
}

ClientPtrPtr addClientStartSS(const Client *c) {
  if (!c)
    return NULL;
  Stack *s = SS.stacks + c->ws;
  Node *n = allocNode(c);
  if (!n)
    return NULL;
  if (c->isNSP)
    s->nsp = n;
  if (s->size < 1) {
    s->head = n;
    s->last = n;
    setCurrNode(n);
  } else {
    n->next = s->curr;
    if (s->curr->prev)
      s->curr->prev->next = n;
    else
      s->head = n;
    n->prev = s->curr->prev;
    s->curr->prev = n;
    setCurrNode(n);
  }
  s->size++;
  return (ClientPtrPtr)n;
}

// NOTE: it only removes the Node, you must eventually free the return value with freeClientSS
Client *rmvClientSS(ClientPtrPtr c) {
  if (!c)
    return NULL;
  if (isLastClientSS(c))
    return rmvLastNode(SS.stacks + CLI_GET(c).ws);
  return rmvNoLastNode((Node *)c);
}

Client *pushMinimizedClientSS(Client *c) {
  assert(c);
  Stack *s = SS.stacks + (c->ws % SS.size);
  return pushMinimizedCliStack(s, c);
}

Client *popMinimizedClientSS(int ws) {
  Stack *s = SS.stacks + (ws % SS.size);
  return popMinimizedCliStack(s);
}

// First, search in the current stack, if is not there, search in the other stacks
Client *rmvMinimizedClientSS(Window w) {
  Stack *s = SS.stacks + SS.curr;
  Client *c = rmvMinimizedCliStack(s, w);
  if (c)
    return c;
  int i;
  for (i = 0; i < SS.size; ++i) {
    if (i == SS.curr)
      continue;
    c = rmvMinimizedCliStack(SS.stacks + i, w);
    if (c)
      return c;
  }
  return NULL;
}

Bool isCurrStackSS(int ws) {
  return ws == SS.curr;
}

Bool isNSPStackSS(int ws) {
  return ws == SS.size;
}

Bool isEmptyStackSS(int ws) {
  return SS.stacks[ ws % SS.size ].size <= 0;
}

const char *getNameStackSS(int ws) {
  return SS.stacks[ ws % SS.size ].name;
}

int getSizeStackSS(int ws) {
  return SS.stacks[ ws % SS.size ].size;
}

int getMinimizedNumStackSS(int ws) {
  return SS.stacks[ ws % SS.size ].minimizedNum;
}

int getNumLayoutStackSS(int ws) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return SS.stacks[ s ].numLayouts;
  return SS.stacks[ s ].numTogLayouts;
}

int getLayoutIdxStackSS(int ws) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return SS.stacks[ s ].currLayoutIdx;
  return SS.stacks[ s ].currTogLayoutIdx;
}

Bool isCurrTogLayoutStackSS(int ws) {
  int s = ws % SS.size;
  return SS.stacks[ s ].currTogLayoutIdx != -1;
}

void setLayoutStackSS(int ws, int i) {
  int s = ws % SS.size;
  SS.stacks[ s ].currLayoutIdx = i % SS.stacks[ s ].numLayouts;
}

void setTogLayoutStackSS(int ws, int i) {
  int s = ws % SS.size;
  if (i != -1)
    SS.stacks[ s ].currTogLayoutIdx = i % SS.stacks[ s ].numTogLayouts;
  else
    SS.stacks[ s ].currTogLayoutIdx = -1;
}

Layout *getLayoutStackSS(int ws, int i) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return SS.stacks[ s ].layouts + (i % SS.stacks[ s ].numLayouts);
  return SS.stacks[ s ].togLayouts + (i % SS.stacks[ s ].numTogLayouts);
}

const LayoutConf *getLayoutConfStackSS(int ws, int i) {
  int s = ws % SS.size;
  if (SS.stacks[ s ].currTogLayoutIdx == -1)
    return workspaceSetS[ s ]->layouts[ i % SS.stacks[ s ].numLayouts ];
  return workspaceSetS[ s ]->togLayouts[ i % SS.stacks[ s ].numTogLayouts ];
}

Layout *getCurrLayoutStackSS(int ws) {
  return getLayoutStackSS(ws, getLayoutIdxStackSS(ws));
}

const LayoutConf *getCurrLayoutConfStackSS(int ws) {
  return getLayoutConfStackSS(ws, getLayoutIdxStackSS(ws));
}

Rectangle *getRegionStackSS(int ws) {
  return &(SS.stacks[ ws % SS.size ].region);
}

ClientPtrPtr getCurrClientStackSS(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].curr);
}

ClientPtrPtr getPrevClientStackSS(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].prev);
}

ClientPtrPtr getHeadClientStackSS(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].head);
}

ClientPtrPtr getLastClientStackSS(int ws) {
  return (ClientPtrPtr)(SS.stacks[ ws % SS.size ].last);
}

ClientPtrPtr findClientStackSS(int ws, const ClientTesterFn ctf, const void *data) {
  assert(tcfn);
  Stack *s = SS.stacks + (ws % SS.size);
  Node *n;
  for (n=s->head; n; n=n->next)
    if (ctf((ClientPtrPtr)n, data))
      return (ClientPtrPtr)n;
  return NULL;
}


// ClientPtrPtr
Bool isCurrClientSS(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].curr;
}

Bool isPrevClientSS(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].prev;
}

Bool isHeadClientSS(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].head;
}

Bool isLastClientSS(const ClientPtrPtr c) {
  return !c ? False : (Node *)c == SS.stacks[ CLI_GET(c).ws ].last;
}

Rectangle *getRegionClientSS(const ClientPtrPtr c) {
  Node *n = (Node *)c;
  return !c ? NULL : &n->region;
}

ClientPtrPtr getNextClientSS(const ClientPtrPtr c) {
  return !c ? NULL : (ClientPtrPtr)(((Node *)c)->next);
}

ClientPtrPtr getPrevClientSS(const ClientPtrPtr c) {
  return !c ? NULL : (ClientPtrPtr)(((Node *)c)->prev);
}

ClientPtrPtr swpClientSS(const ClientPtrPtr c1, const ClientPtrPtr c2) {
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

