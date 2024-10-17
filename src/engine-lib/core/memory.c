#include "engine/memory.h"
#include "engine/logging.h"
#include "engine/platform.h"

// =============================================================================
#pragma region Constants

typedef struct MemoryBlock {
    u64 size;
    MemoryTag tag;
    struct MemoryBlock *next;
} MemoryBlock;

typedef struct MemoryPool {
    u64 totalSize;
    u64 used;
    void *memory;
    MemoryBlock *freeList;
    ENGINE_MUTEX *lock;
} MemoryPool;

ENGINE_GLOBAL MemoryPool memoryPool;

// =============================================================================
#pragma region Implementation

ENGINE_API EngineResult memory_pool_init(u64 size) {
    memoryPool.memory = platform_memory_allocate(size);
    if (!memoryPool.memory) {
        log_error("Failed to allocate memory pool of size %llu bytes.", size);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    memoryPool.totalSize = size;
    memoryPool.used = 0;
    memoryPool.freeList = NULL;

    platform_mutex_create(&memoryPool.lock);
    if (!memoryPool.lock) {
        log_error("Failed to create memory pool mutex.");
        platform_memory_free(memoryPool.memory);
        return ENGINE_ERROR;
    }

    log_info("Memory pool initialized with size %llu bytes.", size);
    return ENGINE_SUCCESS;
}

ENGINE_API void memory_pool_shutdown(void) {
    if (memoryPool.memory) {
        platform_memory_free(memoryPool.memory);
        memoryPool.memory = NULL;
    }

    memoryPool.freeList = NULL;
    memoryPool.used = 0;
    platform_mutex_destroy(memoryPool.lock);

    log_info("Memory pool shutdown.");
}

ENGINE_API void *memory_allocate(u64 size, MemoryTag tag) {

    ENGINE_UNUSED(tag);

    platform_mutex_lock(memoryPool.lock);

    MemoryBlock *prev = NULL;
    MemoryBlock *current = memoryPool.freeList;

    while (current) {
        if (current->size >= size) {
            // split the block if it's significantly larger.
            if (current->size > size + sizeof(MemoryBlock)) {
                // Size for MemoryBlock header plus the desired block size.
                MemoryBlock *newBlock = (MemoryBlock *)((char *)current + sizeof(MemoryBlock) + size);
                newBlock->size = current->size - size - sizeof(MemoryBlock);
                newBlock->next = current->next;
                current->size = size;
                if (prev) {
                    prev->next = newBlock;
                } else {
                    memoryPool.freeList = newBlock;
                }
            } else {
                // Use the entire block.
                if (prev) {
                    prev->next = current->next;
                } else {
                    memoryPool.freeList = current->next;
                }
            }

            platform_mutex_unlock(memoryPool.lock);
            log_debug("Allocated %llu bytes from memory pool.", size);
            return (void *)(current + 1);
        }

        prev = current;
        current = current->next;
    }

    // No suitable block found; check if there's enough space to allocate at the end.
    if (memoryPool.used + size + sizeof(MemoryBlock) > memoryPool.totalSize) {
        log_error("Memory pool exhausted. Cannot allocate %llu bytes.", size);
        platform_mutex_unlock(memoryPool.lock);
        return NULL;
    }

    // Allocate from the end of the pool.
    MemoryBlock *newBlock = (MemoryBlock *)((char *)memoryPool.memory + memoryPool.used);
    newBlock->size = size;
    newBlock->next = NULL;

    memoryPool.used += size + sizeof(MemoryBlock);

    platform_mutex_unlock(memoryPool.lock);
    log_debug("Allocated %llu bytes from memory pool.", size);
    return (void *)(newBlock + 1);
}

ENGINE_API void memory_free(void *block, MemoryTag tag) {
    ENGINE_UNUSED(tag);

    if (!block) {
        log_error("Attempted to free a NULL pointer.");
        return;
    }

    platform_mutex_lock(memoryPool.lock);

    MemoryBlock *memBlock = ((MemoryBlock *)block) - 1;

    // Add the block back to the free list.
    memBlock->next = memoryPool.freeList;
    memoryPool.freeList = memBlock;

    platform_mutex_unlock(memoryPool.lock);
    log_debug("Freed memory block of size %zu bytes.", memBlock->size);
}

// TODO: Double check alignment logic.
ENGINE_API void *memory_aligned_allocate(u64 size, u16 alignment, MemoryTag tag) {

    ENGINE_UNUSED(tag);

    // Simple alignment: allocate extra memory and adjust the pointer.
    void *original = memory_allocate(size + alignment, tag);
    if (!original) {
        return NULL;
    }

    // Align
    u64 originalAddress = (u64)original;
    u64 alignedAddress = (originalAddress + alignment - 1) & ~(alignment - 1);
    return (void *)alignedAddress;
}

ENGINE_API void memory_aligned_free(void *block, MemoryTag tag) {
    ENGINE_UNUSED(tag);

    if (!block) {
        log_error("Attempted to free a NULL pointer.");
        return;
    }

    // Free the original pointer also taking into account alignment.
    MemoryBlock *memBlock = ((MemoryBlock *)block) - 1;
    memory_free(memBlock, tag);
}

ENGINE_API void *memory_copy(void *dest, const void *src, u64 size) {
    return platform_memory_copy(dest, src, size);
}

ENGINE_API void *memory_set(void *dest, i32 value, u64 size) {
    return platform_memory_set(dest, value, size);
}

ENGINE_API void *memory_zero(void *block, u64 size) {
    return platform_memory_zero(block, size);
}
