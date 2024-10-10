#include "engine/memory.h"
#include "engine/logging.h"

#include <stdlib.h>

// TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
#    include <windows.h> // For thread safey on Windows.
#else
#    include <pthread.h> // For thread safety on Unix-based systems.
#endif

/**
 * @brief Define a FreeBlock structure to keep track of free memory blocks.
 *
 * This structure represents a free block of memory, used to manage the free
 * list of the memory pool.
 */
typedef struct FreeBlock {
    u64 size;               /*< Size of the free block. */
    struct FreeBlock *next; /*< Pointer to the next free block in the list. */
} FreeBlock;

/**
 * @brief Memory pool structure to manage memory allocation.
 *
 * Represents a memory pool that can allocate and deallocate memory blocks from
 * a single contiguous memory block. The memory pool uses a free list to keep
 * track of free memory blocks and their sizes.
 */
typedef struct MemoryPool {
    void *memory;        /*< Pointer to the start of the memory pool. */
    u64 capacity;        /*< Total capacity of the memory pool. */
    u64 used;            /*< Amount of memory currently used. */
    FreeBlock *freeList; /*< Pointer to the head of the free list. */
#if defined(PLATFORM_WINDOWS)
    CRITICAL_SECTION lock; /*< Critical section lock for thread safety on Windows. */
#else
    pthread_mutex_t lock; /*< Mutex lock for thread safety on Unix-based systems. */
#endif
} MemoryPool;

static MemoryPool mainPool = {0};
static MemoryPool tempPool = {0};
static MemoryStats memoryStats = {0};

// =============================================================================

/**
 * @brief Initializes the memory pool by setting up the free list to cover the
 * entire pool.
 *
 * @param pool Pointer to the memory pool to initialize.
 */
static void initialize_memory_pool(MemoryPool *pool) {
    pool->freeList = (FreeBlock *)pool->memory;
    pool->freeList->size = pool->capacity;
    pool->freeList->next = NULL;
    // Initialize the lock for thread safety. The lock is used to protect the
    // free list when allocating and deallocating memory blocks.
// TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    InitializeCriticalSection(&pool->lock);
#else
    pthread_mutex_init(&pool->lock, NULL);
#endif
}

/**
 * @brief Allocates memory from the given memory pool.
 *
 * This function looks through the free list to find a suitable block for
 * allocation. If a block is found, it is removed from the free list and
 * returned to the caller. If no suitable block is found, an error message is
 * logged and NULL is returned.
 *
 * @param pool Pointer to the memory pool to allocate from.
 * @param size Size of the memory block to allocate.
 * @param tag Tag to classify the memory allocation.
 * @return void* Pointer to the allocated memory block.
 */
static void *allocate_from_pool(MemoryPool *pool, u64 size, u32 tag) {
    // Lock the pool for thread safety.
// TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    EnterCriticalSection(&pool->lock);
#else
    pthread_mutex_lock(&pool->lock);
#endif

    FreeBlock *prev = NULL;
    FreeBlock *current = pool->freeList;

    // Iterate through the free list to find a suitable block.
    while (current) {
        if (current->size >= size) {
            // Allocate from the current block.
            // Note:
            // (u8 *) is used to cast the FreeBlock pointer to a byte pointer
            // so that we can add the size of the FreeBlock to get the start of
            // the memory block.
            void *block = (u8 *)current + sizeof(FreeBlock);
            if (current->size > size + sizeof(FreeBlock)) {
                // If the block is larger than the requested size, split it by
                // creating a new block that represents the remaining free space
                // after the allocation.
                FreeBlock *newBlock = (FreeBlock *)((u8 *)current + sizeof(FreeBlock) + size);
                newBlock->size = current->size - size - sizeof(FreeBlock);
                newBlock->next = current->next;

                if (prev) {
                    prev->next = newBlock;
                } else {
                    pool->freeList = newBlock;
                }
            } else {
                // Use the entire block if it is too small to split.
                if (prev) {
                    prev->next = current->next;
                } else {
                    pool->freeList = current->next;
                }
            }

            // Update memory usage statistics.
            pool->used += size;
            memoryStats.taggedAllocations[tag] += size;
            memoryStats.currentUsage = pool->used;
            if (memoryStats.currentUsage > memoryStats.peakUsage) {
                memoryStats.peakUsage = memoryStats.currentUsage;
            }

// TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
            LeaveCriticalSection(&pool->lock);
#else
            pthread_mutex_unlock(&pool->lock);
#endif

            return block;
        }

        prev = current;
        current = current->next;
    }
    // Log an error if no suitable block was found.
    log_error("Memory pool out of space. Requested size: %llu bytes.", size);

// TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    LeaveCriticalSection(&pool->lock);
#else
    pthread_mutex_unlock(&pool->lock);
#endif

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
        return ENGINE_ERROR;
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
        return ENGINE_ERROR;
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
    // TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    DeleteCriticalSection(&mainPool.lock);
    DeleteCriticalSection(&tempPool.lock);
#else
    pthread_mutex_destroy(&mainPool.lock);
    pthread_mutex_destroy(&tempPool.lock);
#endif
    ENGINE_FREE(mainPool.memory);
    ENGINE_FREE(tempPool.memory);
    log_info("Memory system shut down. Total allocated: %llu bytes.", memoryStats.totalAllocated);
}

ENGINE_API void *memory_allocate(u64 size, u32 tag) {
    return allocate_from_pool(&mainPool, size, tag);
}

ENGINE_API void memory_free(void *block, u64 size, u32 tag) {
    if (!block) {
        return;
    }

    // TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    EnterCriticalSection(&mainPool.lock);
#else
    pthread_mutex_lock(&mainPool.lock);
#endif

    // Calculate the address of the FreeBlock header.
    FreeBlock *freeBlock = (FreeBlock *)((u8 *)block - sizeof(FreeBlock));
    freeBlock->size = size;

    // Insert the block back into the free list.
    freeBlock->next = mainPool.freeList;
    mainPool.freeList = freeBlock;

    // Update memory usage statistics.
    mainPool.used -= size;
    memoryStats.taggedAllocations[tag] -= size;
    memoryStats.currentUsage = mainPool.used;
    memoryStats.totalFreed += size;

    // TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    LeaveCriticalSection(&mainPool.lock);
#else
    pthread_mutex_unlock(&mainPool.lock);
#endif
}

ENGINE_API void *memory_zero(void *block, u64 size) {
    return ENGINE_ZERO(block, size);
}

ENGINE_API void *memory_copy(void *dest, const void *src, u64 size) {
    return ENGINE_COPY(dest, src, size);
}

ENGINE_API void *memory_set(void *dest, i32 value, u64 size) {
    return ENGINE_SET(dest, value, size);
}

MemoryStats memory_get_stats(void) {
    return memoryStats;
}

ENGINE_API void *memory_allocate_temporary(u64 size) {
    return allocate_from_pool(&tempPool, size, MEMORY_TAG_UNKNOWN);
}

ENGINE_API void memory_reset_temporary(void) {
    // TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    EnterCriticalSection(&tempPool.lock);
#else
    pthread_mutex_lock(&tempPool.lock);
#endif

    tempPool.used = 0;
    initialize_memory_pool(&tempPool);

    // TODO: Move to platform system
#if defined(PLATFORM_WINDOWS)
    LeaveCriticalSection(&tempPool.lock);
#else
    pthread_mutex_unlock(&tempPool.lock);
#endif
}
