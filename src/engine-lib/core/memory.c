#include "engine/memory.h"
#include "engine/logging.h"

#include <stdlib.h>

// Define a FreeBlock structure to keep track of free memory blocks.
typedef struct FreeBlock {
    u64 size;               /*< Size of the free block. */
    struct FreeBlock *next; /*< Pointer to the next free block. */
} FreeBlock;

// Define a MemoryPool structure to manage memory allocation.
typedef struct MemoryPool {
    void *memory;        /*< Pointer to the memory block. */
    u64 capacity;        /*< Total capacity of the memory block. */
    u64 used;            /*< Amount of memory used. */
    FreeBlock *freeList; /*< List of free memory blocks. */
} MemoryPool;

static MemoryPool mainPool = {0};
static MemoryPool tempPool = {0};
static MemoryStats memoryStats = {0};

// =============================================================================

static void initialize_memory_pool(MemoryPool *pool) {
    pool->freeList = (FreeBlock *)pool->memory;
    pool->freeList->size = pool->capacity;
    pool->freeList->next = NULL;
}

static void *allocate_from_pool(MemoryPool *pool, u64 size, u32 tag) {
    FreeBlock *prev = NULL;
    FreeBlock *current = pool->freeList;

    // Iterate through the free list to find a suitable block.
    while (current) {
        if (current->size >= size) {
            // Allocate from the current block.
            void *block = (u8 *)current + sizeof(FreeBlock);
            if (current->size > size + sizeof(FreeBlock)) {
                // If the block is larger than the requested size, split it.
                FreeBlock *newBlock = (FreeBlock *)((u8 *)current + sizeof(FreeBlock) + size);
                newBlock->size = current->size - size - sizeof(FreeBlock);
                newBlock->next = current->next;

                if (prev) {
                    prev->next = newBlock;
                } else {
                    pool->freeList = newBlock;
                }
            } else {
                // Use the entire block.
                if (prev) {
                    prev->next = current->next;
                } else {
                    pool->freeList = current->next;
                }
            }

            pool->used += size;
            memoryStats.taggedAllocations[tag] += size;
            memoryStats.currentUsage = pool->used;
            if (memoryStats.currentUsage > memoryStats.peakUsage) {
                memoryStats.peakUsage = memoryStats.currentUsage;
            }
            return block;
        }

        prev = current;
        current = current->next;
    }

    log_error("Memory pool out of space. Requested size: %llu bytes.", size);
    return NULL;
}

// =============================================================================

EngineResult memory_system_init(u64 totalSize) {
    // Allocate main pool (90% of total size).
    u64 mainPoolSize = (totalSize * 9) / 10;
    mainPool.memory = ENGINE_ALLOC(mainPoolSize);

    // Memory allocation failed.
    if (!mainPool.memory) {
        log_error("Failed to allocate main memory pool.");
        return ENGINE_FAILURE;
    }

    mainPool.capacity = mainPoolSize;
    mainPool.used = 0;
    initialize_memory_pool(&mainPool);

    // Allocate temporary pool (10% of total size).
    u64 tempPoolSize = totalSize - mainPoolSize;
    tempPool.memory = ENGINE_ALLOC(tempPoolSize);

    // Temp memory pool allocation failed. Free main pool and return.
    if (!tempPool.memory) {
        ENGINE_FREE(mainPool.memory);
        log_error("Failed to allocate temporary memory pool.");
        return ENGINE_FAILURE;
    }

    tempPool.capacity = tempPoolSize;
    tempPool.used = 0;
    initialize_memory_pool(&tempPool);

    memoryStats.totalAllocated = totalSize;
    memoryStats.peakUsage = 0;
    memoryStats.currentUsage = 0;

    log_info("Memory system initialized with total size of %llu bytes.", totalSize);
    return ENGINE_SUCCESS;
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
    if (!block) {
        return;
    }

    // Calculate the start of the block.
    FreeBlock *freeBlock = (FreeBlock *)((u8 *)block - sizeof(FreeBlock));
    freeBlock->size = size;
    freeBlock->next = mainPool.freeList;
    mainPool.freeList = freeBlock;

    mainPool.used -= size;
    memoryStats.taggedAllocations[tag] -= size;
    memoryStats.currentUsage = mainPool.used;
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
    initialize_memory_pool(&tempPool);
}
