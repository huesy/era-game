#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "defines.h"

typedef struct MemoryPool {
    void *memory;
    u64 blockSize;
    u64 blockCount;
    u64 freeBlockCount;
    u8 *freeList;
} MemoryPool;

void *memory_allocate(u64 size);
void memory_free(void *ptr);
void memory_copy(void *dest, const void *src, u64 size);

MemoryPool *memory_pool_create(u64 blockSize, u64 blockCount);
void *memory_pool_allocate(MemoryPool *pool);
void memory_pool_free(MemoryPool *pool, void *ptr);

#endif // ENGINE_MEMORY_H
