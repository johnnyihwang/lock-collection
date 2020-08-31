#include "mcslock.h"

mcslock_t *mcs_init() {
    mcslock_t *l = malloc(sizeof(mcslock_t));
    l->tail = NULL;
    return l;
}

mcsnode_t *mcs_get_node() {
    mcsnode_t *n = malloc(sizeof(mcsnode_t));
    n->locked = 0;
    n->next = NULL;
    return n;
}

void mcs_lock(mcslock_t *l, mcsnode_t *my_node) {
    volatile mcsnode_t *pred = __atomic_exchange_n(&(l->tail), my_node,__ATOMIC_SEQ_CST);
    if (pred != NULL) {
        my_node->locked = 1;
        pred->next = my_node;
        while (my_node->locked) {}
    }
}

int mcs_trylock(mcslock_t *l, mcsnode_t *my_node) {
    return __atomic_load_n(&(l->tail), __ATOMIC_SEQ_CST) == NULL;
}

void mcs_unlock(mcslock_t *l, volatile mcsnode_t *my_node) {
    volatile mcsnode_t *my_node2 = my_node;
    if (my_node->next == NULL) {
        // If anyone has the intention of getting the lock,
        // the tail must have been set through an atomic instruction
        // so the tail is not me
        if (__atomic_compare_exchange_n(&(l->tail), &my_node2, NULL, 0,__ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
            return;
        // Wait for the next node to set my next field
        while (my_node->next == NULL) {}
    }
    my_node->next->locked = 0;
    my_node->next = NULL;
}

void mcs_close_node(mcsnode_t *my_node) {
    free(my_node);
}

void mcs_close(mcslock_t *l) {
    free(l);
}