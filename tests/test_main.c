void pool_basic_test();
void arena_basic_test();

int main(void)
{
#ifdef POOL
    pool_basic_test();
#endif

#ifdef ARENA
    arena_basic_test();
#endif

    return 0;
}
