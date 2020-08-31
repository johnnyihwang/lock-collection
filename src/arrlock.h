#ifndef ARRLOCK_H_
#define ARRLOCK_H_
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

typedef struct
{
	long pre_pad[8];
	volatile bool value;
	long post_pad[8];
} padded_bool_t;

typedef struct {
    volatile unsigned long *tail;
    unsigned long mask;
    padded_bool_t *flags;
} arrlock_t;

arrlock_t *arr_init(unsigned int num_threads);
int arr_lock(arrlock_t *l);
int arr_trylock(arrlock_t *l);
void arr_unlock(arrlock_t *l, int my_tail);
void arr_close(arrlock_t *l);

#endif
