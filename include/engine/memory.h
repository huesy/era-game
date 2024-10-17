#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "engine/defines.h"

// =============================================================================
#pragma region Constants

typedef enum MemoryTag {
    MEMORY_TAG_NONE = 0,
    MEMORY_TAG_ENGINE,
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
    MEMORY_TAG_MAX
} MemoryTag;

// =============================================================================
#pragma region Interface

/**
 * @brief Initializes the memory pool.
 *
 * @param size The total size of the memory pool in bytes.
 * @return ENGINE_SUCCESS if the memory pool was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult memory_pool_init(u64 size);

/**
 * @brief Shuts down the memory pool.
 *
 * @return void
 */
ENGINE_API void memory_pool_shutdown(void);

/**
 * @brief Allocates memory.
 *
 * @param size The size of the memory to allocate.
 * @param tag The tag to associate with the memory.
 * @return A pointer to the allocated memory.
 */
ENGINE_API void *memory_allocate(u64 size, MemoryTag tag);

/**
 * @brief Frees memory.
 *
 * @param block A pointer to the memory to free.
 * @param tag The tag associated with the memory.
 * @return void
 */
ENGINE_API void memory_free(void *block, MemoryTag tag);

/**
 * @brief Allocates aligned memory.
 *
 * @param size The size of the memory to allocate.
 * @param alignment The alignment of the memory to allocate.
 * @param tag The tag to associate with the memory.
 * @return A pointer to the allocated memory.
 */
ENGINE_API void *memory_aligned_allocate(u64 size, u16 alignment, MemoryTag tag);

/**
 * @brief Frees aligned memory.
 *
 * @param block A pointer to the memory to free.
 * @param tag The tag associated with the memory.
 * @return void
 */
ENGINE_API void memory_aligned_free(void *block, MemoryTag tag);

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
