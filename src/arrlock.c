#include "arrlock.h"

unsigned int arr_size(unsigned int num_threads) {
    unsigned int i = 1;
    while(i < num_threads)
        i = i << 1;
    return i;
}

arrlock_t *arr_init(unsigned int num_threads) {
    arrlock_t *l = malloc(sizeof(arrlock_t));
    unsigned long size = arr_size(num_threads);
    l->mask = size - 1;
    //l->tail = 0L;
    l->tail = malloc(sizeof(unsigned long));
    *(l->tail) = 0L;
    l->flags = calloc(sizeof(padded_bool_t), size);
    l->flags[0].value = true;
    return l;
}

int arr_lock(arrlock_t *l) {
    unsigned long tail = __atomic_fetch_add(l->tail, 1, __ATOMIC_SEQ_CST);
    tail = tail & l->mask;
    while(!l->flags[tail].value) {}
    return tail;
}

int arr_trylock(arrlock_t *l) {
    unsigned long tail = __atomic_load_n(l->tail, __ATOMIC_SEQ_CST);
    tail = tail & l->mask;
    return !(l->flags[tail].value);
}

void arr_unlock(arrlock_t *l, int my_tail) {
    // Safer way to implement this would be to not rely on client code
    // to pass in the tail, but get thread_id, and use that
    // to access an array that stores the tail values for each thread
    // Or just thread_local
    l->flags[my_tail].value = false;
    my_tail = (my_tail + 1) & l->mask;
    l->flags[my_tail].value = true;
    return;
}

void arr_close(arrlock_t *l) {
    free(l->flags);
    free((unsigned long *) l->tail);
    free(l);
}
