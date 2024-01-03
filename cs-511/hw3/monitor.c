/*
 * trafficmgr.c
 *
 * Copyright (C) Lingfei Hu
 */

#include <monitor.h>

static void q_init();
static void q_putCart(char);
static int q_cartIsWaiting(char);
static bool q_cartIsEmpty(char);
static void q_cartHasEntered(char);
static void q_shutdown();
static bool monitor_is_need_wait(char);
static char monitor_get_signal_direction(char);


static pthread_mutex_t QLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t monLock;
static pthread_cond_t northCond;
static pthread_cond_t southCond;
static pthread_cond_t eastCond;
static pthread_cond_t westCond;


static struct q_entry_t *northQ;
static int northCartIsWaiting;
static struct q_entry_t *southQ;
static int southCartIsWaiting;
static struct q_entry_t *eastQ;
static int eastCartIsWaiting;
static struct q_entry_t *westQ;
static int westCartIsWaiting;
static int cartNumber;
static int cartCrossedCount;

static char directions[] = {Q_NORTH, Q_WEST, Q_SOUTH, Q_EAST};
static int direction_count = 4;

void 
monitor_init(char *directions)
{
    int     rc;
    char    *c;

    q_init();

    /* init locks */
    if ((rc = pthread_mutex_init(&monLock, NULL)) != 0) {
        err_exit("monitor mutex init failed: %s\n", strerror(rc));
    }
    if ((rc = pthread_cond_init(&northCond, NULL)) != 0) {
        err_exit("north condition variable init failed: %s\n", strerror(rc));
    }
    if ((rc = pthread_cond_init(&southCond, NULL)) != 0) {
        err_exit("south condition variable init failed: %s\n", strerror(rc));
    }
    if ((rc = pthread_cond_init(&eastCond, NULL)) != 0) {
        err_exit("east condition variable init failed: %s\n", strerror(rc));
    }
    if ((rc = pthread_cond_init(&westCond, NULL)) != 0) {
        err_exit("west condition variable init failed: %s\n", strerror(rc));
    }

    cartCrossedCount = 0;

    /* insert carts into 4 queues according to directions */
    c = directions;
    while (*c) {
        q_putCart(*c);
        ++c;
    }
}

void 
monitor_shutdown(void)
{
    q_shutdown();

    (void) pthread_cond_destroy(&northCond);
    (void) pthread_cond_destroy(&southCond);
    (void) pthread_cond_destroy(&eastCond);
    (void) pthread_cond_destroy(&westCond);
    (void) pthread_mutex_destroy(&monLock);

}

void
monitor_arrive(struct cart_t *cart)
{
    pthread_mutex_lock(&monLock);
    
    printf("cart %i of thread for direction '%c' arrives at intersection\n", cart->num, cart->dir);
    if (monitor_is_need_wait(cart->dir)) {
        printf("cart %i of thread for direction '%c' must wait before entering intersection\n", cart->num, cart->dir);
        if (cart->dir == Q_NORTH) {
            pthread_cond_wait(&northCond, &monLock);
        }
        else if (cart->dir == Q_SOUTH) {
            pthread_cond_wait(&southCond, &monLock);
        }
        else if (cart->dir == Q_EAST) {
            pthread_cond_wait(&eastCond, &monLock);
        }
        else{
            pthread_cond_wait(&westCond, &monLock);
        }
    }
    printf("cart %i of thread for direction '%c' is allowed to proceed into intersection\n", cart->num, cart->dir);
    printf("cart %i of thread for direction '%c' enters intersection\n", cart->num, cart->dir);
    ++cartCrossedCount;
    
    pthread_mutex_unlock(&monLock);
}

void
monitor_cross(struct cart_t *cart)
{
    printf("cart %i of thread for direction '%c' crosses intersection\n", cart->num, cart->dir);
    /* take 1 second to pass through */
    sleep(1);
}

void
monitor_leave(struct cart_t *cart)
{
    char sig_direction;
    
    pthread_mutex_lock(&monLock);
    
    q_cartHasEntered(cart->dir);
    printf("cart %i of thread for direction '%c' leaves intersection\n", cart->num, cart->dir);

    sig_direction = monitor_get_signal_direction(cart->dir);
    if (sig_direction != '\0') {
        printf("thread for direction '%c' signals '%c'\n", cart->dir, sig_direction);
        if (sig_direction == Q_NORTH) {
            pthread_cond_signal(&northCond);
        }
        else if (sig_direction == Q_SOUTH) {
            pthread_cond_signal(&southCond);
        }
        else if (sig_direction == Q_EAST) {
            pthread_cond_signal(&eastCond);
        }
        else{
            pthread_cond_signal(&westCond);
        }
    }
    
    pthread_mutex_unlock(&monLock);
}

static bool
monitor_is_need_wait(char direction)
{
    int k;

    /* for the first cart, don't need wait to avoid deadlock */
    if (cartCrossedCount == 0) {
        return false;
    }
    
    /* find the index of direction */
    k = 0;
    while (directions[k] != direction) {
        k = k < direction_count - 1 ? k + 1 : 0;    
    }

    /* move to the next index */
    k = k < direction_count - 1 ? k + 1 : 0;    

    /* check whether other queues have carts needed to cross intersection */
    while (directions[k] != direction) {
        if (!q_cartIsEmpty(directions[k]) || q_cartIsWaiting(directions[k])) {
            return true;
        }
        k = k < direction_count - 1 ? k + 1 : 0;
    }

    return false;
}

static char
monitor_get_signal_direction(char direction)
{
    int k;
    
    /* find the index of direction */
    k = 0;
    while (directions[k] != direction) {
        k = k < direction_count - 1 ? k + 1 : 0;    
    }
    
    /* move to the next index */
    k = k < direction_count - 1 ? k + 1 : 0;

    /* find the next direction based on the right rule */
    while (directions[k] != direction) {
        if (!q_cartIsEmpty(directions[k]) || q_cartIsWaiting(directions[k])) {
            return directions[k];
        }
        k = k < direction_count - 1 ? k + 1 : 0;
    }

    return '\0';
}


static void 
q_init() 
{
  /* mutex initialized statically */
  northQ = NULL;
  northCartIsWaiting = 0;
  southQ = NULL;
  southCartIsWaiting = 0;
  eastQ = NULL;
  eastCartIsWaiting = 0;
  westQ = NULL;
  westCartIsWaiting = 0;
  cartNumber = 0;
  return;
}


/*
 *  Get cart from head of 'dir' queue; if queue is empty return NULL
 *  Caller is responsible for freeing memory once cart_t is no longer needed.
 */
struct cart_t *
q_getCart(char dir) 
{
  struct cart_t *cart = NULL;
  struct q_entry_t *temp;

  pthread_mutex_lock(&QLock);
  switch (dir) {
  case Q_NORTH:
    if (northQ == NULL)
      break;   /* jump out of switch stmt, execute unlock below */
    cart = northQ->cart;
    temp = northQ;
    northQ = northQ->next;
    northCartIsWaiting = 1;
    free(temp);
    break;
  case Q_SOUTH:
    if (southQ == NULL)
      break;   /* jump out of switch stmt, execute unlock below */
    cart = southQ->cart;
    temp = southQ;
    southQ = southQ->next;
    southCartIsWaiting = 1;
    free(temp);
    break;
  case Q_EAST:
    if (eastQ == NULL)
      break;   /* jump out of switch stmt, execute unlock below */
    cart = eastQ->cart;
    temp = eastQ;
    eastQ = eastQ->next;
    eastCartIsWaiting = 1;
    free(temp);
    break;
  case Q_WEST:
    if (westQ == NULL)
      break;   /* jump out of switch stmt, execute unlock below */
    cart = westQ->cart;
    temp = westQ;
    westQ = westQ->next;
    westCartIsWaiting = 1;
    free(temp);
    break;
  }
  pthread_mutex_unlock(&QLock);

  return cart;
}


/*
 *  Place new cart at tail of 'dir' queue
 */
static void 
q_putCart(char dir) 
{
  struct cart_t *cart = malloc(sizeof(struct cart_t));
  struct q_entry_t *newCart = malloc(sizeof(struct q_entry_t));
  if ((cart == NULL) || (newCart == NULL))
    return;

  pthread_mutex_lock(&QLock);
  cart->num = ++cartNumber;
  cart->dir = dir;
  newCart->cart = cart;
  newCart->next = NULL;
  switch (dir) {
  case Q_NORTH:
    if (northQ == NULL) {
      northQ = newCart;
    } else {
      struct q_entry_t *ptr = northQ;
      while (ptr->next != NULL)
    ptr = ptr->next;
      ptr->next = newCart;
    }
    break;
  case Q_SOUTH:
    if (southQ == NULL) {
      southQ = newCart;
    } else {
      struct q_entry_t *ptr = southQ;
      while (ptr->next != NULL)
    ptr = ptr->next;
      ptr->next = newCart;
    }
    break;
  case Q_EAST:
    if (eastQ == NULL) {
      eastQ = newCart;
    } else {
      struct q_entry_t *ptr = eastQ;
      while (ptr->next != NULL)
    ptr = ptr->next;
      ptr->next = newCart;
    }
    break;
  case Q_WEST:
    if (westQ == NULL) {
      westQ = newCart;
    } else {
      struct q_entry_t *ptr = westQ;
      while (ptr->next != NULL)
    ptr = ptr->next;
      ptr->next = newCart;
    }
    break;
  }
  pthread_mutex_unlock(&QLock);
  return;
}


/*
 *  Is there a cart from direction 'dir' waiting to enter intersection?
 */
static int 
q_cartIsWaiting(char dir) 
{
  int answer;

  pthread_mutex_lock(&QLock);
  switch (dir) {
  case Q_NORTH:
    answer = northCartIsWaiting;
    break;
  case Q_SOUTH:
    answer = southCartIsWaiting;
    break;
  case Q_EAST:
    answer = eastCartIsWaiting;
    break;
  case Q_WEST:
    answer = westCartIsWaiting;
    break;
  }
  pthread_mutex_unlock(&QLock);
  return answer;
}

/*
 * is the queue empty
 */
static bool
q_cartIsEmpty(char dir) 
{
  struct q_entry_t *answer;

  pthread_mutex_lock(&QLock);
  switch (dir) {
  case Q_NORTH:
    answer = northQ;
    break;
  case Q_SOUTH:
    answer = southQ;
    break;
  case Q_EAST:
    answer = eastQ;
    break;
  case Q_WEST:
    answer = westQ;
    break;
  }
  pthread_mutex_unlock(&QLock);
  return answer == NULL ? true : false;
}

/*
 *  Mark that cart from direction 'dir' is no longer waiting.
 */
static void 
q_cartHasEntered(char dir) 
{
  pthread_mutex_lock(&QLock);
  switch (dir) {
  case Q_NORTH:
    northCartIsWaiting = 0;
    break;
  case Q_SOUTH:
    southCartIsWaiting = 0;
    break;
  case Q_EAST:
    eastCartIsWaiting = 0;
    break;
  case Q_WEST:
    westCartIsWaiting = 0;
    break;
  }
  pthread_mutex_unlock(&QLock);
  return;
}

static void 
q_shutdown() 
{
  if ((northQ != NULL) || northCartIsWaiting)
    fprintf(stderr, "Warning!  There are still entries in the north queue\n");
  if ((southQ != NULL) || southCartIsWaiting)
    fprintf(stderr, "Warning!  There are still entries in the south queue\n");
  if ((eastQ != NULL) || eastCartIsWaiting)
    fprintf(stderr, "Warning!  There are still entries in the east queue\n");
  if ((westQ != NULL) || westCartIsWaiting)
    fprintf(stderr, "Warning!  There are still entries in the west queue\n");
  pthread_mutex_destroy(&QLock);
  return;
}

void
err_exit(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}
