#include "../includes/sk_arena.h"
#include <stdio.h>

int main(void)
{
    sk_arena arena;

    sk_arena_init(&arena, 4);
    int *myInt = sk_arena_alloc(&arena, sizeof(int));
    *myInt = 456;

    printf("%d\n", *myInt);

    return 0;
}

