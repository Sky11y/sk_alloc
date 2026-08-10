#ifndef SK_POOL_H
 #define SK_POOL_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct free_list free_list;
typedef struct sk_pool sk_pool;

struct sk_pool {
    unsigned char*  buf;        // beginning of the pool buffer
    size_t          capacity;   // total capacity of the buffer
    size_t          chunk_size; // Size of one chunk
    size_t          alloced_chunks;     // how many chunks alloced
    size_t          free_chunks;       // how many chunks free
    free_list*      head;       // first free chunk
    int             flags;      // possible flags
};

struct free_list {
    free_list*  next;
};

#define USER_BUF 0x00000001
#define DEFAULT_COUNT 42
#define DEFAULT_SIZE 8

/*
   PROTOTYPES
*/
bool        sk_pool_init(sk_pool* pool, size_t object_size, size_t object_count);
bool        sk_pool_init_with_buffer(sk_pool* pool, void* buffer, size_t object_size, size_t object_count);
bool        sk_pool_destroy(sk_pool* pool); // destroy the whole pool
void*       sk_pool_alloc(sk_pool* pool); // allocate one object
bool        sk_pool_free(sk_pool* pool, void* ptr); // free one object
void        sk_pool_reset(sk_pool* pool); // free all the objects
ssize_t      sk_pool_capacity(sk_pool* pool); // total number of objects
ssize_t      sk_pool_count(sk_pool* pool); // objects alloced
ssize_t      sk_pool_remaining(sk_pool* pool); // free objects left

#endif
