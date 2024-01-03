/*
 * trafficmgr.c
 *
 * Copyright (C) Lingfei Hu
 */

#include <monitor.h>

static void usage(void);
static void *direction_func(void *);

char north = Q_NORTH;
char south = Q_SOUTH;
char east = Q_EAST;
char west = Q_WEST;

static pthread_barrier_t exitBarrier;
static const unsigned int threadCount = 4;

int
main(int argc, char **argv)
{
    char        *c;
    pthread_t   north_thread, south_thread, east_thread, west_thread;
    int         rc;

    if (argc != 2) {
        usage();
        exit(EXIT_FAILURE);
    }
    
    c = argv[1];
    while (*c) {
        if (*c != Q_NORTH && *c != Q_SOUTH && *c != Q_EAST && *c != Q_WEST) {
            usage();
            exit(EXIT_FAILURE);
        }
        ++c;
    }

    /* init barrier lock */
    if ((rc = pthread_barrier_init(&exitBarrier, NULL, threadCount)) != 0) {
        err_exit("barrier create init failed: %s\n", strerror(rc));
    }

    monitor_init(argv[1]);
    
    /* create threads */
    if (pthread_create(&north_thread, NULL, direction_func, &north) != 0) {
        err_exit("pthread_create error: %s\n", strerror(errno));
    }
    if (pthread_create(&south_thread, NULL, direction_func, &south) != 0) {
        err_exit("pthread_create error: %s\n", strerror(errno));
    }
    if (pthread_create(&east_thread, NULL, direction_func, &east) != 0) {
        err_exit("pthread_create error: %s\n", strerror(errno));
    }
    if (pthread_create(&west_thread, NULL, direction_func, &west) != 0) {
        err_exit("pthread_create error: %s\n", strerror(errno));
    }

    /* wait for termination of threads */
    if (pthread_join(north_thread, NULL) != 0) {
        err_exit("pthread_join error: %s\n", strerror(errno));
    }
    if (pthread_join(south_thread, NULL) != 0) {
        err_exit("pthread_join error: %s\n", strerror(errno));
    }
    if (pthread_join(east_thread, NULL) != 0) {
        err_exit("pthread_join error: %s\n", strerror(errno));
    }
    if (pthread_join(west_thread, NULL) != 0) {
        err_exit("pthread_join error: %s\n", strerror(errno));
    }

    monitor_shutdown();
    (void) pthread_barrier_destroy(&exitBarrier);

    exit(EXIT_SUCCESS);

}

static void *
direction_func(void *arg)
{
    char            direction;
    struct cart_t   *cart;

    direction = *(char *)arg;

    printf("thread for direction '%c' starts\n", direction);
    cart = q_getCart(direction);
    while (cart != NULL) {
        printf("thread for direction '%c' gets cart %i\n", direction, cart->num);
        monitor_arrive(cart);
        monitor_cross(cart);
        monitor_leave(cart);
        free(cart);
        cart = q_getCart(direction);
    }

    /* wait other threads to exit together */
    pthread_barrier_wait(&exitBarrier);
    printf("thread for direction '%c' exits\n", direction);

    return NULL;
}


static void
usage(void)
{
    printf("usage: ./trafficmgr [directions]\n");
    printf("The directions must be the string containing 'n', 'w', 's' or 'e'\n");
}


