#ifndef SK_ARENA_H
 #define SK_ARENA_H

#include <stdlib.h>

typedef struct sk_arena sk_arena;

struct sk_arena {
    unsigned char*  buf;
    size_t          cap;
    size_t          cur_offset;
    size_t          prev_offset;
    int             flags;
};


/*
   PROTOTYPES
*/
bool        sk_arena_init(sk_arena* arena, size_t size);
bool        sk_arena_init_with_buffer(sk_arena* arena, void* buffer, size_t size);
bool        sk_arena_destroy(sk_arena* arena);
void*       sk_arena_alloc(sk_arena* arena, size_t size); 
void        sk_arena_reset(sk_arena* arena);
size_t      sk_arena_capacity(sk_arena* arena);
size_t      sk_arena_offset(sk_arena* arena);
size_t      sk_arena_remaining(sk_arena* arena);
// void*      sk_arena_resize(sk_arena* arena, size_t size);

#endif
