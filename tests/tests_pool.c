#include "../includes/sk_pool.h"
#include <stdio.h>

void pool_basic_test()
{
    sk_pool pool;

    sk_pool_init(&pool, 4, 2);
    int *myInt = sk_pool_alloc(&pool);
    *myInt = 456;

    assert(*myInt == 456 && "Basic test failed");
    sk_pool_destroy(&pool);
}

// int main(void)
// {
//     basic_test();
//
//     return 0;
// }
//
