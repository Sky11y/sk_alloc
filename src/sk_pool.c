#include "../includes/sk_pool.h"

static bool is_power_of_two(uintptr_t x) {
    return (x & (x - 1)) == 0;
}

bool sk_pool_init(sk_pool* pool, size_t object_size, size_t object_count)
{
    if (!pool)
        return false;

    pool->chunk_size = object_size ? object_size : DEFAULT_SIZE;
    pool->free_chunks = object_count ? object_count : DEFAULT_COUNT;
    pool->capacity = pool->chunk_size * pool->free_chunks;
    pool->buf = malloc(pool->capacity);
    assert(pool->buf);
    pool->alloced_chunks = 0;
    pool->head = NULL;
    pool->flags = 0;
    return true;
}

bool sk_pool_init_with_buffer(sk_pool* pool, void* buffer, size_t object_size, size_t object_count)
{
    if (!pool || !buffer)
        return false;

    pool->buf = buffer;
    pool->chunk_size = object_size ? object_size : DEFAULT_SIZE;
    pool->free_chunks = object_count ? object_count : DEFAULT_COUNT;
    pool->capacity = pool->chunk_size * pool->free_chunks;
    pool->alloced_chunks = 0;
    pool->head = NULL;
    pool->flags = USER_BUF;
    return true;
}

bool sk_pool_destroy(sk_pool* pool)
{
    if (!pool)
        return false;

    if ((0xFF & pool->flags) != USER_BUF)
        free(pool->buf);

    memset((void*)pool, 0, sizeof(*pool));

    return true;
}

void* sk_pool_alloc(sk_pool* pool)
{
    void* ptr = NULL;
    if (!pool || pool->free_chunks == 0)
        return ptr;

    if (pool->head) {
        ptr = pool->head;
        pool->head = pool->head->next;
    } else {
        ptr = pool->buf + pool->alloced_chunks * pool->chunk_size;
    }

    ++pool->alloced_chunks;
    --pool->free_chunks;

    return ptr;
}

bool sk_pool_free(sk_pool* pool, void* ptr)
{
    if (!pool || !ptr)
        return false;

    free_list *node = (free_list*)ptr;
    node->next= pool->head;
    pool->head = node;

    return true;
}

void sk_pool_reset(sk_pool* pool)
{
    if (!pool)
        return;

    pool->alloced_chunks = 0;
    pool->free_chunks = pool->capacity / pool->chunk_size;
    pool->head = NULL;
}

ssize_t sk_pool_capacity(sk_pool* pool)
{
    if (!pool)
        return -1;

    return pool->capacity;
}

ssize_t sk_pool_count(sk_pool* pool)
{
    if (!pool)
        return -1;

    return pool->alloced_chunks;
}

ssize_t sk_pool_remaining(sk_pool* pool)
{
    if (!pool)
        return -1;

    return pool->free_chunks;
}

