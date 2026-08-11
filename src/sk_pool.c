#include "../includes/sk_pool.h"

static bool is_power_of_two(uintptr_t x)
{
    return (x & (x - 1)) == 0;
}

static uintptr_t align_fw(uintptr_t ptr, uintptr_t align)
{
    uintptr_t p, a, mod;

    assert(is_power_of_two(align));

    p = ptr;
    a = (uintptr_t)align;
    mod = p & (a - 1);

    if (mod != 0) {
        p += a - mod;
    }
    return p;
}

static size_t align_size(size_t size, size_t align)
{
    size_t a, p, mod;

	assert(is_power_of_two((uintptr_t)align));

	a = align;
	p = size;
	mod = p & (a-1);
	if (mod!= 0) {
		p += a - mod;
	}
	return p;
}

static bool pool_init(sk_pool* p, size_t object_size, size_t object_count,
        size_t alignment)
{
    if (object_size) {
        p->chunk_size = align_size(object_size, alignment);
    } else {
        p->chunk_size = DEFAULT_SIZE;
    }

    p->free_chunks = object_count ? object_count : DEFAULT_COUNT;
    p->capacity = p->chunk_size * p->free_chunks;
    p->buf = (unsigned char*)malloc(p->capacity);
    assert(p->buf);
    p->alloced_chunks = 0;
    p->head = NULL;
    p->flags = 0;
    return true;

    // might need to "free" all
	// pool_free_all(p);
}

bool sk_pool_init(sk_pool* pool, size_t object_size, size_t object_count)
{
    if (!pool)
        return false;

    return pool_init(pool, object_size, object_count, DEFAULT_ALIGNMENT);
}

static bool pool_init_buf(sk_pool* p, void* buf, size_t buf_size,
        size_t obj_size, size_t alignment)
{
    // align the buffer (make it "shorter")
	uintptr_t initial_start = (uintptr_t)buf;
	uintptr_t start = align_fw(initial_start, (uintptr_t)alignment);
    buf_size -= (size_t)(start-initial_start);

    // align chunk size (make it "longer")
    size_t chunk_size;
    if (obj_size) {
        chunk_size = align_size(obj_size, alignment);
    } else {
        chunk_size = DEFAULT_SIZE;
    }

    // we need to assert if buffer is still valid after possible modifications
	assert(chunk_size >= sizeof(free_list) &&
	       "Chunk size is too small");
	assert(buf_size >= chunk_size &&
	       "Backing buffer length is smaller than the chunk size");

    p->buf = (unsigned char*)start;
    p->capacity = buf_size;
    p->chunk_size = chunk_size;
    p->free_chunks = buf_size / chunk_size;
    p->alloced_chunks = 0;
    p->head = NULL;
    p->flags = USER_BUF;
    return true;
}

bool sk_pool_init_with_buffer(sk_pool* pool, void* buffer, size_t buffer_size,
        size_t object_size)
{
    if (!pool || !buffer)
        return false;

    return pool_init_buf(pool, buffer, buffer_size, object_size, DEFAULT_ALIGNMENT);
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

