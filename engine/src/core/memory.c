#include "core/memory.h"
#include "core/logging.h"

#include <stdlib.h>

void *memory_allocate(u64 size) {
    return malloc(size);
}

void memory_free(void *ptr) {
    free(ptr);
}

void memory_copy(void *dest, const void *src, u64 size) {
    u8 *d = (u8 *)dest;
    const u8 *s = (const u8 *)src;
    for (u64 i = 0; i < size; ++i) {
        d[i] = s[i];
    }
}

void memory_zero(void *dest, u64 size) {
    u8 *d = (u8 *)dest;
    for (u64 i = 0; i < size; ++i) {
        d[i] = 0;
    }
}

MemoryPool *memory_pool_create(u64 blockSize, u64 blockCount) {
    // Allocate memory for the pool.
    MemoryPool *pool = memory_allocate(sizeof(MemoryPool));
    if (!pool) {
        log_error("Failed to allocate memory for memory pool.");
        return NULL;
    }

    // Allocate memory for the pool blocks.
    pool->memory = memory_allocate(blockSize * blockCount);
    if (!pool->memory) {
        log_error("Failed to allocate memory for memory pool blocks.");
        memory_free(pool);
        return NULL;
    }

    pool->blockSize = blockSize;
    pool->blockCount = blockCount;
    pool->freeBlockCount = blockCount;

    pool->freeList = (u8 *)memory_allocate(blockCount);
    if (!pool->freeList) {
        log_error("Failed to allocate memory for memory pool free list.");
        memory_free(pool->memory);
        memory_free(pool);
        return NULL;
    }

    // Set all blocks as free.
    for (u64 i = 0; i < blockCount; ++i) {
        pool->freeList[i] = 1;
    }

    return pool;
}

void *memory_pool_allocate(MemoryPool *pool) {
    // Find the first free block.
    for (u64 i = 0; i < pool->blockCount; ++i) {
        if (pool->freeList[i]) {
            pool->freeList[i] = 0;
            pool->freeBlockCount -= 1;
            return (void *)((u8 *)pool->memory + (i * pool->blockSize));
        }
    }

    return NULL; // No available block.
}

void memory_pool_free(MemoryPool *pool, void *ptr) {
    // Calculate the block index by finding the offset from the start of the pool memory.
    u64 index = ((u8 *)ptr - (u8 *)pool->memory) / pool->blockSize;
    pool->freeList[index] = 1;
    ++pool->freeBlockCount;
}
