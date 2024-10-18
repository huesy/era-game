#include "engine/memory.h"
#include "engine/logging.h"
#include "engine/platform.h"

// =============================================================================
#pragma region Mutex

typedef struct Mutex {
    void *handle;
} Mutex;

static void mutex_init_internal(Mutex *mutex) {
    if (!mutex) {
        log_error("Attempted to initialize a NULL mutex.");
        return;
    }

    platform_mutex_create(&mutex->handle);
}

static void mutex_destroy_internal(Mutex *mutex) {
    if (!mutex) {
        log_error("Attempted to destroy a NULL mutex.");
        return;
    }

    platform_mutex_destroy(mutex->handle);
}

static void mutex_lock_internal(Mutex *mutex) {
    if (!mutex) {
        log_error("Attempted to lock a NULL mutex.");
        return;
    }

    platform_mutex_lock(mutex->handle);
}

static void mutex_unlock_internal(Mutex *mutex) {
    if (!mutex) {
        log_error("Attempted to unlock a NULL mutex.");
        return;
    }

    platform_mutex_unlock(mutex->handle);
}

// =============================================================================
#pragma region Memory Pool

ENGINE_API EngineResult memory_pool_init(MemoryPool *pool, u64 size) {

    if (!pool || size == 0) {
        log_error("Invalid MemoryPool pointer or size in memory_pool_init.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Allocate the memory pool with alignment.
    pool->memory = platform_memory_allocate_aligned(size, ENGINE_STANDARD_ALIGNMENT);
    if (!pool->memory) {
        log_error("Aligned memory allocation failed for memory pool of size %zu bytes.", size);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    pool->totalSize = size;
    pool->used = 0;
    pool->freeList = NULL;

    // Initialize allocation tacking.
    for (u32 tag = 0; tag < MEMORY_TAG_MAX; ++tag) {
        pool->allocations[tag] = NULL;
    }

    // Initialize mutex.
    Mutex *mutex = (Mutex *)platform_memory_allocate_aligned(sizeof(Mutex), ENGINE_STANDARD_ALIGNMENT);
    if (!mutex) {
        log_error("Failed to allocate memory for mutex.");
        platform_memory_free(pool->memory);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    mutex_init_internal(mutex);
    pool->lock = mutex;

    log_info("Memory pool initialized with size %zu bytes.", size);
    return ENGINE_SUCCESS;
}

ENGINE_API void memory_pool_shutdown(MemoryPool *pool) {
    if (!pool) {
        log_error("Invalid MemoryPool pointer in memory_pool_shutdown.");
        return;
    }

    // Detect memory leaks before shutting down.
    memory_pool_detect_leaks(pool);

    // Free all allocations (if any) - In this simple pool, we don't track individual allocations beyond.
    // More sophisticated systems can iterate through the allocations array and free them appropriately.

    // Destroy mutex.
    Mutex *mutex = (Mutex *)pool->lock;
    if (mutex) {
        mutex_destroy_internal(mutex);
        platform_memory_free(mutex);
        pool->lock = NULL;
    }

    // Free the memory pool.
    if (pool->memory) {
        platform_memory_free_aligned(pool->memory);
        pool->memory = NULL;
    }

    log_info("Memory pool shutdown completed.");
}

#pragma endregion
// =============================================================================
#pragma region Memory Allocation

// Separate allocation tracking for AllocationRecord instances to prevent infinite recursion.
typedef struct AllocationRecordPool {
    AllocationRecord *head;
    Mutex lock;
} AllocationRecordPool;

static AllocationRecordPool allocationRecordPool = {0};

// Initialize the allocation record pool.
static EngineResult allocation_record_pool_init(u64 size) {
    // Preallocate a fixed number of AllocationRecords to avoid dynamic allocations during tracking.
    // For simplicity, allocate a separate memory pool for AllocationRecords.
    AllocationRecord *base = (AllocationRecord *)platform_memory_allocate_aligned(size * sizeof(AllocationRecord), ENGINE_STANDARD_ALIGNMENT);
    if (!base) {
        log_error("Failed to allocate memory for AllocationRecord pool.");
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    // Initialize the AllocationRecordPool.
    allocationRecordPool.head = NULL;
    mutex_init_internal(&allocationRecordPool.lock);

    // Populate the AllocationRecordPool with preallocated records.
    for (u64 i = 0; i < size; ++i) {
        base[i].ptr = NULL;
        base[i].size = 0;
        base[i].next = allocationRecordPool.head;
        allocationRecordPool.head = &base[i];
    }

    log_info("AllocationRecord pool initialized with %zu records.", size);
    return ENGINE_SUCCESS;
}

// Shutdown the AllocationRecordPool.
static void allocation_record_pool_shutdown(void) {
    // Destroy mutex.
    mutex_destroy_internal(&allocationRecordPool.lock);
    allocationRecordPool.head = NULL;
}

// Allocate an AllocationRecord from the AllocationRecordPool.
static AllocationRecord *allocate_allocation_record(void) {
    mutex_lock_internal(&allocationRecordPool.lock);

    if (!allocationRecordPool.head) {
        log_error("AllocationRecord pool exhausted.");
        mutex_unlock_internal(&allocationRecordPool.lock);
        return NULL;
    }

    AllocationRecord *record = allocationRecordPool.head;
    allocationRecordPool.head = record->next;

    mutex_unlock_internal(&allocationRecordPool.lock);

    return record;
}

ENGINE_API void *memory_allocate(MemoryPool *pool, u64 size, MemoryTag tag) {
    // Allocate memory with the standard alignment.
    return memory_allocate_aligned(pool, size, ENGINE_STANDARD_ALIGNMENT, tag);
}

ENGINE_API void *memory_allocate_aligned(MemoryPool *pool, u64 size, u16 alignment, MemoryTag tag) {

    if (!pool || size == 0 || tag >= MEMORY_TAG_MAX) {
        log_error("Invalid MemoryPool pointer, size, or tag in memory_allocate_aligned.");
        return NULL;
    }

    // Memory layout [Header][Padding][Data]
    // - Header: MemoryBlockHeader
    // - Padding: To align the data to the specified boundary.
    // - Data: The actual memory requested.

    // Calculate total size needed.
    u64 totalSize = sizeof(MemoryBlockHeader) + size + alignment;

    mutex_lock(pool->lock);

    // Search free list for a suitable block.
    MemoryBlockHeader **current = &pool->freeList;
    while (*current) {
        if ((*current)->size >= totalSize) {
            // Suitable block found.
            MemoryBlockHeader *block = *current;
            *current = block->next;

            // Set magic number.
            block->magic = MEMORY_BLOCK_MAGIC;
            block->tag = tag;

            // Calculate aligned address after header.
            u64 alignedAddress = ((u64)block + sizeof(MemoryBlockHeader) + alignment - 1) & ~(alignment - 1);
            u64 padding = alignedAddress - (u64)block;

            // Update used size.
            pool->used += padding + size;

            // Track allocation.
            AllocationRecord *record = (AllocationRecord *)memory_allocate(pool, sizeof(AllocationRecord), MEMORY_TAG_ENGINE);
            if (!record) {
                log_error("Failed to allocate AllocationRecord.");
                mutex_unlock(pool->lock);
                return NULL;
            }
            record->ptr = (void *)alignedAddress;
            record->size = size;
            record->next = pool->allocations[tag];
            pool->allocations[tag] = record;

            mutex_unlock(pool->lock);
            log_debug("Reused free block for allocation of size %zu bytes with alignment %d.", size, alignment);
            return (void *)alignedAddress;
        }

        current = &((*current)->next);
    }

    // No suitable free block found; check if there's enough space.
    if (pool->used + totalSize > pool->totalSize) {
        log_error("Memory pool exhausted. Cannot allocate %zu bytes with alignment %d.", size, alignment);
        mutex_unlock((Mutex *)pool->lock);
        return NULL;
    }

    // Allocate new block.
    MemoryBlockHeader *block = (MemoryBlockHeader *)((char *)pool->memory + pool->used);
    block->size = totalSize;
    block->tag = tag;
    block->magic = MEMORY_BLOCK_MAGIC;
    block->next = NULL;

    // Calculate aligned address after header.
    u64 alignedAddress = ((u64)block + sizeof(MemoryBlockHeader) + alignment - 1) & ~(alignment - 1);
    u64 padding = alignedAddress - (u64)block;

    // Update used size.
    pool->used += padding + size;

    // Track allocation.
    AllocationRecord *newRecord = (AllocationRecord *)memory_allocate(pool, sizeof(AllocationRecord), MEMORY_TAG_ENGINE);
    if (!newRecord) {
        log_error("Failed to allocate AllocationRecord.");
        mutex_unlock(pool->lock);
        return NULL;
    }
    newRecord->ptr = (void *)alignedAddress;
    newRecord->size = size;
    newRecord->next = pool->allocations[tag];
    pool->allocations[tag] = newRecord;

    mutex_unlock(pool->lock);

    log_debug("Allocated %zu bytes with alignment %d.", size, alignment);

    return (void *)alignedAddress;
}

ENGINE_API void memory_free(MemoryPool *pool, void *ptr, MemoryTag tag) {
    memory_free_aligned(pool, ptr, tag);
}

ENGINE_API void memory_free_aligned(MemoryPool *pool, void *ptr, MemoryTag tag) {
    if (!pool || !ptr || tag >= MEMORY_TAG_MAX) {
        log_error("Invalid MemoryPool pointer, memory block, or MemoryTag in memory_free_aligned.");
        return;
    }

    u64 ptrAddress = (u64)ptr;

    // Retrieve the block header.
    MemoryBlockHeader *block = (MemoryBlockHeader *)(ptrAddress - sizeof(MemoryBlockHeader));
    if (block->magic != MEMORY_BLOCK_MAGIC) {
        log_error("Memory corruption detected during free. Magic number mismatch.");
        return;
    }

    mutex_lock(pool->lock);

    // Remove allocation record from tracking.
    AllocationRecord **current = &pool->allocations[tag];
    while (*current) {
        if ((*current)->ptr == ptr) {
            AllocationRecord *toFree = *current;
            *current = (*current)->next;
            memory_free(pool, toFree, MEMORY_TAG_ENGINE);
            break;
        }
        current = &((*current)->next);
    }

    // Add the block to the free list.
    block->next = pool->freeList;
    pool->freeList = block;

    // Update used size.
    pool->used -= block->size;

    mutex_unlock(pool->lock);

    log_debug("Freed memory block of size %zu bytes with tag %d.", block->size, tag);
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

#pragma endregion
// =============================================================================
#pragma region Memory Leak Detection

ENGINE_API void memory_pool_detect_leaks(MemoryPool *pool) {
    if (!pool) {
        log_error("Invalid MemoryPool pointer in memory_pool_detect_leaks.");
        return;
    }

    mutex_lock(pool->lock);

    // Iterate through the allocations and log any leaks.
    b8 leaksDetected = false;
    for (u32 tag = 0; tag < MEMORY_TAG_MAX; ++tag) {
        if (pool->allocations[tag]) {
            log_warning("Memory leak detected for tag %d: Address %p", tag, pool->allocations[tag]);
            leaksDetected = true;
        }
    }

    if (!leaksDetected) {
        log_info("No memory leaks detected.");
    }

    mutex_unlock(pool->lock);

    log_info("Memory leak detection completed.");
}

#pragma endregion
// =============================================================================
