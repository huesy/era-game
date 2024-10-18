#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "engine/defines.h"

// Magic number for integrity checking.
#define MEMORY_MAGIC_NUMBER 0xDEADBEEF
#define ENGINE_STANDARD_ALIGNMENT 16

// =============================================================================

typedef enum MemoryTag {
    MEMORY_TAG_NONE = 0,

    MEMORY_TAG_ENGINE,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_PLATFORM,
    MEMORY_TAG_AUDIO,
    MEMORY_TAG_VIDEO,
    MEMORY_TAG_INPUT,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_PHYSICS,
    MEMORY_TAG_NETWORK,
    MEMORY_TAG_SCRIPTING,
    MEMORY_TAG_ASSET,
    MEMORY_TAG_EDITOR,
    MEMORY_TAG_GAME,

    MEMORY_TAG_MAX,
} MemoryTag;

// Forward declaration.
typedef struct MemoryBlockHeader MemoryBlockHeader;

/**
 * @brief Memory block header.
 */
typedef struct MemoryBlockHeader {
    struct MemoryBlockHeader *next; /**< Pointer to the next free memory block. */
    u64 size;                       /**< Size of the memory block. */
    MemoryTag tag;                  /**< Tag associated with the memory block. */
    u32 magic;                      /**< Magic number for memory block validation. */
} MemoryBlockHeader;

/**
 * @brief Allocation record.
 */
typedef struct AllocationRecord {
    void *ptr;                     /**< Address of the allocated memory. */
    u64 size;                      /**< Size of the allocated memory. */
    struct AllocationRecord *next; /**< Pointer to the next allocation record. */
} AllocationRecord;

/**
 * Memory pool structure.
 */
typedef struct MemoryPool {
    void *memory;                                  /**< Pointer to the memory pool. */
    u64 totalSize;                                 /**< Total size of the memory pool. */
    u64 used;                                      /**< Amount of memory used. */
    MemoryBlockHeader *freeList;                   /**< Pointer to the free memory block list. */
    AllocationRecord *allocations[MEMORY_TAG_MAX]; /**< Allocation tracking per tag. */
    void *lock;                                    /**< Pointer to the memory pool lock. */
} MemoryPool;

// =============================================================================
#pragma region Memory Pool

/**
 * @brief Initializes the memory pool.
 *
 * @param pool A pointer to the memory pool structure.
 * @param size The total size of the memory pool in bytes.
 * @return ENGINE_SUCCESS if the memory pool was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult memory_pool_init(MemoryPool *pool, u64 size);

/**
 * @brief Shuts down the memory pool.
 *
 * @param pool A pointer to the memory pool structure.
 * @return void
 */
ENGINE_API void memory_pool_shutdown(MemoryPool *pool);

/**
 * @brief Detect memory leaks.
 *
 * @param pool A pointer to the memory pool structure.
 * @return void
 */
ENGINE_API void memory_pool_detect_leaks(MemoryPool *pool);

// =============================================================================
#pragma region Memory Allocation

/**
 * @brief Allocates memory from the memory pool.
 *
 * @param pool A pointer to the memory pool structure.
 * @param size The size of the memory to allocate in bytes.
 * @param tag The tag to associate with the memory.
 * @return A pointer to the allocated memory, aligned to ENGINE_STANDARD_ALIGNMENT.
 */
ENGINE_API void *memory_allocate(MemoryPool *pool, u64 size, MemoryTag tag);

/**
 * @brief Allocates aligned memory from the memory pool.
 *
 * @param pool A pointer to the memory pool structure.
 * @param size The size of the memory to allocate in bytes.
 * @param alignment The alignment boundary.
 * @param tag The tag to associate with the memory.
 * @return A pointer to the allocated memory, aligned to the specified boundary.
 */
ENGINE_API void *memory_allocate_aligned(MemoryPool *pool, u64 size, u16 alignment, MemoryTag tag);

/**
 * @brief Frees memory back to the memory pool.
 *
 * @param pool A pointer to the memory pool structure.
 * @param ptr A pointer to the memory to free.
 * @param tag The tag associated with the memory.
 * @return void
 */
ENGINE_API void memory_free(MemoryPool *pool, void *ptr, MemoryTag tag);

/**
 * @brief Frees aligned memory back to the memory pool.
 *
 * @param pool A pointer to the memory pool structure.
 * @param ptr A pointer to the aligned memory to free.
 * @param tag The tag associated with the memory.
 * @return void
 */
ENGINE_API void memory_free_aligned(MemoryPool *pool, void *ptr, MemoryTag tag);

/**
 * @brief Copies memory.
 *
 * @param dest A pointer to the destination memory.
 * @param src A pointer to the source memory.
 * @param size The size of the memory to copy.
 * @return A pointer to the destination memory.
 */
ENGINE_API void *memory_copy(void *dest, const void *src, u64 size);

/**
 * @brief Sets memory.
 *
 * @param dest A pointer to the destination memory.
 * @param value The value to set the memory to.
 * @param size The size of the memory to set.
 * @return A pointer to the destination memory.
 */
ENGINE_API void *memory_set(void *dest, i32 value, u64 size);

/**
 * @brief Zeros memory.
 *
 * @param block A pointer to the memory to zero.
 * @param size The size of the memory to zero.
 * @return A pointer to the zeroed memory.
 */
ENGINE_API void *memory_zero(void *block, u64 size);

#endif // ENGINE_MEMORY_H
