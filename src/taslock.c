#include "taslock.h"

taslock_t *tas_init() {
    taslock_t *l = malloc(sizeof(taslock_t));
    l->state = 0;
    return l;
}

void tas_lock(taslock_t *l) {
    while( __atomic_test_and_set(&(l->state), __ATOMIC_SEQ_CST)) {}
}

int tas_trylock(taslock_t *l) {
    return __atomic_load_n(&(l->state), __ATOMIC_SEQ_CST);
}

void tas_unlock(taslock_t *l) {
    //__atomic_store_n(&(l->state), 0, __ATOMIC_SEQ_CST);
    l->state = 0;
}

void tas_close(taslock_t *l) {
    free(l);
}