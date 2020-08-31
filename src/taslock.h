#ifndef TASLOCK_H_
#define TASLOCK_H_
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

typedef struct {
    char state;
} taslock_t;

taslock_t *tas_init();
void tas_lock(taslock_t *l);
int tas_trylock(taslock_t *l); // 1 means is locked, 0 means free
void tas_unlock(taslock_t *l);
void tas_close(taslock_t *l);



#endif