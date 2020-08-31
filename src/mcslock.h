#ifndef MCSLOCK_H_
#define MCSLOCK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

typedef struct mcsnode_tmp {
    // Added padding to ensure each node is in different cache line
    long pad[8];
    volatile struct mcsnode_tmp *next;
    volatile char locked;
    long pad2[8];
} mcsnode_t;

typedef struct {
    volatile mcsnode_t* tail;
} mcslock_t;

mcslock_t *mcs_init();
mcsnode_t *mcs_get_node();
void mcs_lock(mcslock_t *l, mcsnode_t *my_node);
int mcs_trylock(mcslock_t *l, mcsnode_t *my_node);
void mcs_unlock(mcslock_t *l, volatile mcsnode_t *my_node);
void mcs_close_node(mcsnode_t *my_node);
void mcs_close(mcslock_t *l);

#endif
