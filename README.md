# Collection of Locks

Implements 3 types of locks in C, using GCC Builtin Atomics.

* TASLock : lock that functions by using test-and-set on a single variable.
* Array Lock : lock where each thread spins on a separate element in a preallocated padded array
* MCS Lock : lock where each thread spins on separate nodes which forms a linked list

TASLocks are simple to implement, but as it invalidates cache every time a thread acquires a lock, it does not scale well with large number of threads, especially in high contention situations. Array locks and MCS locks solve this problem as each thread spins on its own index/node.

## How to Use

Each class of locks has a `init`, `lock`, `unlock`, `close` function. Using them is fairly straight-forward: receive the lock pointer from the init function, copy it to each of the threads, call the lock function, and unlock similarly.

For the array lock, the `arr_lock(arrlock_t *l)` function returns an `int`. This is the index where the next thread in line is spinning on; therefore, this `int` must also be given to the `arr_unlock` function.

For the MCS lock, because the lock struct does not contain the lock "nodes" that threads spin on, the `mcs_get_node()` function must be called to get a node that each thread will use. This is an advantage as it allows the number of threads using the lock to be configured at runtime. Moreover, it allows each thread to reuse their nodes in repeated calls to `lock`, or free them if no more locking is needed. The `mcs_node_t` pointer must therefore be provided when locking/unlocking. `mcs_close_node(mcsnode_t *my_node)` allows the thread to free the memory for its node if it is no longer necessary.

