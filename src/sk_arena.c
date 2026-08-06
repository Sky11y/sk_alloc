#include "../includes/sk_arena.h"

bool sk_arena_init(sk_arena* arena, size_t size)
{
    if (!arena || size == 0)
        return false;
    arena->buf = malloc(size);
    assert(arena->buf);
    arena->cap = size;
    arena->cur_offset = 0;
    arena->prev_offset = 0;
    arena->flags = 0;
    return true;
}

bool sk_arena_init_with_buffer(sk_arena* arena, void* buffer, size_t size)
{
    if (!arena || !buffer || size == 0)
        return false;
    arena->buf = buffer;
    arena->cap = size;
    arena->cur_offset = 0;
    arena->prev_offset = 0;
    arena->flags = USER_BUF;
    return true;

}

bool sk_arena_destroy(sk_arena* arena)
{
    if (!arena)
        return false;

    if ((0xFF & arena->flags) != USER_BUF)
        free(arena->buf);

    memset((void*)arena, 0, sizeof(*arena));
    return true;
}

void* sk_arena_alloc(sk_arena* arena, size_t size)
{
    if (!arena || !size || (arena->cur_offset + size) > arena->cap)
        return NULL;

    void* ptr = arena->buf + arena->cur_offset;
    arena->prev_offset = arena->cur_offset;
    arena->cur_offset += size;

    return ptr;
}

bool sk_arena_reset(sk_arena* arena)
{
    if (!arena)
        return false;
    arena->cur_offset = 0;
    arena->prev_offset = 0;
    return true;
}

ssize_t sk_arena_capacity(sk_arena* arena)
{
    if (!arena)
        return -1;

    return arena->cap;
}

ssize_t sk_arena_offset(sk_arena* arena)
{
    if (!arena)
        return -1;

    return arena->cur_offset;
}

ssize_t sk_arena_remaining(sk_arena* arena)
{
    if (!arena)
        return -1;

    return (arena->cap - arena->cur_offset); }



