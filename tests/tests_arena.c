#include "../includes/sk_arena.h"
#include <stdio.h>

void arena_basic_test()
{
    sk_arena arena;

    sk_arena_init(&arena, 4);
    int *myInt = sk_arena_alloc(&arena, sizeof(int));
    *myInt = 456;

    assert(*myInt == 456 && "Basic test failed");
    sk_arena_destroy(&arena);
}

// int main(void)
// {
//     basic_test();
//
//     return 0;
// }
//
