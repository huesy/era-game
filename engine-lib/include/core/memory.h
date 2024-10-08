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

/**
 * @brief Allocates a block of memory of the given size.
 *
 * @param size The size of the memory block to allocate in bytes.
 * @return Pointer to the allocated memory, or NULL if allocation fails.
 */
void *memory_allocate(u64 size);

/**
 * @brief Frees a previously allocated block of memory.
 *
 * @param ptr Pointer to the memory block to free.
 */
void memory_free(void *ptr);

/**
 * @brief Copies a block of memory from source to destination.
 *
 * @param dest Pointer to the destination memory.
 * @param src Pointer to the source memory.
 * @param size Number of bytes to copy.
 */
void memory_copy(void *dest, const void *src, u64 size);

/**
 * @brief Sets a block of memory to zero.
 *
 * @param dest Pointer to the memory block to set.
 * @param size Number of bytes to set to zero.
 */
void memory_zero(void *dest, u64 size);

/**
 * @brief Creates a memory pool with specified block size and count.
 *
 * @param blockSize Size of each block in bytes.
 * @param blockCount Number of blocks in the pool.
 * @return Pointer to the created MemoryPool, or NULL if creation fails.
 */
MemoryPool *memory_pool_create(u64 blockSize, u64 blockCount);

/**
 * @brief Allocates a block from the specified memory pool.
 *
 * @param pool Pointer to the MemoryPool to allocate from.
 * @return Pointer to the allocated block, or NULL if no blocks are available.
 */
void *memory_pool_allocate(MemoryPool *pool);

/**
 * @brief Frees a block back to the specified memory pool.
 *
 * @param pool Pointer to the MemoryPool.
 * @param ptr Pointer to the block to free.
 */
void memory_pool_free(MemoryPool *pool, void *ptr);

#endif // ENGINE_MEMORY_H
