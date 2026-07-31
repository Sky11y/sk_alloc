#ifndef SK_POOL_H
 #define SK_POOL_H

#include <stdlib.h>

typedef struct free_list free_list;
typedef struct sk_pool sk_pool;

struct sk_pool {
    unsigned char*  buf;        // beginning of the pool buffer
    size_t          capacity;   // total capacity of the buffer
    size_t          chunk_size; // Size of one chunk
    size_t          chunks;     // how many chunks alloced
    size_t          free;       // how many chunks free
    int             flags;      // possible flags

    free_list*      head;       // first free chunk
};

struct free_list {
    free_list*  next;
};

/*
   PROTOTYPES
*/
bool        sk_pool_init(sk_pool* pool, size_t object_size, size_t object_count);
bool        sk_pool_init_with_buffer(sk_pool* pool, void* buffer, size_t object_size, size_t object_count);
bool        sk_pool_destroy(sk_pool* pool); // destroy the whole pool
void*       sk_pool_alloc(sk_pool* pool); // allocate one object
void        sk_pool_free(sk_pool* pool, void* ptr); // free one object
void        sk_pool_reset(sk_pool* pool); // free all the objects
size_t      sk_pool_capacity(sk_pool* pool); // total number of objects
size_t      sk_pool_count(sk_pool* pool); // objects alloced
size_t      sk_pool_remaining(sk_pool* pool); // free objects left

#endif
