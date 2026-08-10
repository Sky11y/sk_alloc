#ifndef SK_ARENA_H
 #define SK_ARENA_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

// allow user to define own alignment
#ifndef DEFAULT_ALIGNMENT
 #define DEFAULT_ALIGNMENT (2*sizeof(void*)) // usually 16 bytes
#endif

// flags
#define USER_BUF 0x00000001

typedef struct sk_arena sk_arena;

struct sk_arena {
    unsigned char*  buf;            // beginning of the arena buffer
    size_t          cap;            // total capacity of the buffer
    size_t          cur_offset;     // current offset
    size_t          prev_offset;    // previous offset, might be needed later
    char            flags;          // possible flags
};


/*
   PROTOTYPES
*/
bool        sk_arena_init(sk_arena* arena, size_t size);
bool        sk_arena_init_with_buffer(sk_arena* arena, void* buffer, size_t size);
bool        sk_arena_destroy(sk_arena* arena);
void*       sk_arena_alloc(sk_arena* arena, size_t size); 
bool        sk_arena_reset(sk_arena* arena);
ssize_t      sk_arena_capacity(sk_arena* arena);
ssize_t      sk_arena_offset(sk_arena* arena);
ssize_t      sk_arena_remaining(sk_arena* arena);
// void*      sk_arena_resize(sk_arena* arena, size_t size);

#endif
