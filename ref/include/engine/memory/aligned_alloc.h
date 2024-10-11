#ifndef ENGINE_ALIGNED_ALLOC_H
#define ENGINE_ALIGNED_ALLOC_H

#include "engine/defines.h"

/**
 * @brief Allocates a block of memory with the specified size and alignment.
 *
 * This function abstracts platform-specific aligned allocation, ensuring that
 * memory is aligned to the given alignment boundary.
 *
 * @param size Size of the memory block to allocate.
 * @param alignment Alignment of the memory block. Must be a power of two.
 * @return void* Pointer to the allocated memory block, or NULL if allocation fails.
 */
ENGINE_API void *aligned_alloc(u64 size, u16 alignment);

/**
 * @brief Frees a block of memory previously allocated with aligned_alloc.
 *
 * @param block Pointer to the memory block to free.
 */
ENGINE_API void aligned_free(void *block);

#endif // ENGINE_ALIGNED_ALLOC_H
