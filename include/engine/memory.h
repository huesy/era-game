#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "defines.h"

/**
 * @brief Memory tag enumeration to classify different types of memory usage,
 * useful for tracking allocations and debugging memory leaks.
 */
typedef enum MemoryTag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_COMPONENT,
    MEMORY_TAG_SCENE,
    MEMORY_TAG_PLUGIN,
    MEMORY_TAG_EDITOR,

    MEMORY_TAG_MAX_TAGS
} MemoryTag;

/**
 * @brief Memory statistics structure to keep track of memory usage statistics
 * for debugging and performance tracking.
 */
typedef struct MemoryStats {
    u64 totalAllocated;                         /*< Total memory allocated by the system. */
    u64 taggedAllocations[MEMORY_TAG_MAX_TAGS]; /*< Total memory allocated per tag type. */
    u64 totalFreed;                             /*< Total memory that has been freed. */
    u64 peakUsage;                              /*< Peak memory usage observed. */
    u64 currentUsage;                           /*< Current total memory usage. */
} MemoryStats;

/**
 * @brief Initialize the memory system with the specified total size. Splits the
 * memory pool into two separate pools, one for main memory allocations (90%)
 * and one for temporary memory allocations (10%).
 *
 * @param totalSize Total size of the memory pool.
 * @return EngineResult Result of the memory system initialization.
 */
EngineResult memory_system_init(u64 totalSize);

/**
 * @brief Shutdown the memory system by freeing the memory pools and destroying
 * any locks used for thread safety.
 */
void memory_system_shutdown(void);

/**
 * @brief Allocate a block of memory from the main memory pool with the
 * specified size and tag.
 *
 * @param size Size of the memory block to allocate.
 * @param tag Tag to classify the memory allocation.
 * @return void* Pointer to the allocated memory block.
 */
ENGINE_API void *memory_allocate(u64 size, u32 tag);

/**
 * @brief Free a block of memory from the main memory pool  with the specified
 * size and tag.
 *
 * @param block Pointer to the memory block to free.
 * @param size Size of the memory block to free.
 * @param tag Tag to classify the memory allocation.
 */
ENGINE_API void memory_free(void *block, u64 size, u32 tag);

/**
 * @brief Zero out the memory block with the specified size.
 *
 * @param block Pointer to the memory block to zero out.
 * @param size Size of the memory block to zero out.
 * @return void* Pointer to the zeroed out memory block.
 */
ENGINE_API void *memory_zero(void *block, u64 size);

/**
 * @brief Copy memory from the source block to the destination block.
 *
 * @param dest Pointer to the destination memory block.
 * @param src Pointer to the source memory block that will be copied.
 * @param size Size of the memory block to copy.
 * @return void* Pointer to the destination memory block.
 */
ENGINE_API void *memory_copy(void *dest, const void *src, u64 size);

/**
 * @brief Set the memory block with the specified value.
 *
 * @param dest Pointer to the memory block to set.
 * @param value Value to set the memory block to.
 * @param size Size of the memory block to set.
 * @return void* Pointer to the memory block.
 */
ENGINE_API void *memory_set(void *dest, i32 value, u64 size);

/**
 * @brief Get the memory statistics for the current memory usage.
 *
 * @return MemoryStats Memory statistics structure.
 */
MemoryStats memory_get_stats(void);

// Temporary memory allocation. Only valid until next temporary allocation or reset.
/**
 * @brief Allocate a block of temporary memory with the specified size.
 * This memory is only valid until the next temporary allocation or reset.
 * This is useful for temporary memory allocations that are short-lived.
 *
 * @param size Size of the temporary memory block to allocate.
 * @return void* Pointer to the allocated temporary memory block.
 */
ENGINE_API void *memory_allocate_temporary(u64 size);

/**
 * @brief Reset the temporary memory pool to the initial state and free all
 * temporary memory allocations.
 */
ENGINE_API void memory_reset_temporary(void);

#endif // ENGINE_MEMORY_H
