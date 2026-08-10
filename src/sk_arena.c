#include "../includes/sk_arena.h"

static bool is_power_of_two(uintptr_t x) {
    return (x & (x - 1)) == 0;
}

/* align forward to the next address
    e.g. ptr = 0x1005, align = 8

    mod = 0x1005 & 0x7  =   0001_0000_0000_0101
                          & 0000_0000_0000_0111
                        -----------------------
                            0000_0000_0000_0101
    mod = 5
    p = 0x1005 + (8 - 5) <=> 0x1008
*/
static uintptr_t align_fw(uintptr_t ptr, size_t align) {
    uintptr_t p, a, mod;

    assert(is_power_of_two(align));

    p = ptr;
    a = (uintptr_t)align;
    // since 'a' is power of two we can do a faster version of modulo
    // see example above
    mod = p & (a - 1);

    if (mod != 0) {
        p += a - mod;
    }
    return p;
}

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

void* arena_alloc_align(sk_arena* arena, size_t size, size_t align)
{
    uintptr_t curr_ptr = (uintptr_t)arena->buf + (uintptr_t)arena->cur_offset;
    uintptr_t offset = align_fw(curr_ptr, align);
    offset -= (uintptr_t)arena->buf; // relative offset from arena->buf

    if (offset + size <= arena->cap) {
        void* ptr = &arena->buf[offset];
        arena->prev_offset = offset;
        arena->cur_offset = offset + size;

        memset(ptr, 0, size);
        return ptr;
    }
    return NULL;
}

void* sk_arena_alloc(sk_arena* arena, size_t size)
{
    if (!arena || !size)
        return NULL;

    return arena_alloc_align(arena, size, DEFAULT_ALIGNMENT);
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

    return (arena->cap - arena->cur_offset);
}



