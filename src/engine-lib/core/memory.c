#include "engine/memory.h"
#include "engine/logging.h"

#include <stdlib.h>

typedef struct MemoryPool {
    void *memory;
    u64 capacity;
    u64 used;
} MemoryPool;

static MemoryPool mainPool = {0};
static MemoryPool tempPool = {0};
static MemoryStats memoryStats = {0};

// =============================================================================

static void *allocate_from_pool(MemoryPool *pool, u64 size, u32 tag) {
    if (pool->used + size > pool->capacity) {
        log_error("Memory pool out of space. Requested size: %llu bytes.", size);
        return NULL;
    }

    void *block = (u8 *)pool->memory + pool->used;
    pool->used += size;

    memoryStats.taggedAllocations[tag] += size;

    return block;
}

// =============================================================================

b8 memory_system_init(u64 totalSize) {
    // Allocate main pool (90% of total size).
    u64 mainPoolSize = (totalSize * 9) / 10;
    mainPool.memory = ENGINE_ALLOC(mainPoolSize);

    // Memory allocation failed.
    if (!mainPool.memory) {
        log_error("Failed to allocate main memory pool.");
        return false;
    }

    mainPool.capacity = mainPoolSize;
    mainPool.used = 0;

    // Allocate temporary pool (10% of total size).
    u64 tempPoolSize = totalSize - mainPoolSize;
    tempPool.memory = ENGINE_ALLOC(tempPoolSize);

    // Temp memory pool allocation failed. Free main pool and return.
    if (!tempPool.memory) {
        ENGINE_FREE(mainPool.memory);
        log_error("Failed to allocate temporary memory pool.");
        return false;
    }

    tempPool.capacity = tempPoolSize;
    tempPool.used = 0;

    memoryStats.totalAllocated = totalSize;
    log_info("Memory system initialised with total size of %llu bytes.", totalSize);
}

void memory_system_shutdown(void) {
    ENGINE_FREE(mainPool.memory);
    ENGINE_FREE(tempPool.memory);
    log_info("Memory system shut down. Total allocated: %llu bytes.", memoryStats.totalAllocated);
}

void *memory_allocate(u64 size, u32 tag) {
    return allocate_from_pool(&mainPool, size, tag);
}

void memory_free(void *block, u64 size, u32 tag) {
    // TODO: Block
    memoryStats.taggedAllocations[tag] -= size;
}

void *memory_zero(void *block, u64 size) {
    return ENGINE_ZERO(block, size);
}

void *memory_copy(void *dest, const void *src, u64 size) {
    return ENGINE_COPY(dest, src, size);
}

void *memory_set(void *dest, i32 value, u64 size) {
    return ENGINE_SET(dest, value, size);
}

MemoryStats memory_get_stats(void) {
    return memoryStats;
}

void *memory_allocate_temporary(u64 size) {
    return allocate_from_pool(&tempPool, size, MEMORY_TAG_UNKNOWN);
}

void memory_reset_temporary(void) {
    tempPool.used = 0;
}
