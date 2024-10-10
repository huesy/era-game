#ifndef ENGINE_FREELIST_H
#define ENGINE_FREELIST_H

#include "engine/defines.h"
#include "engine/platform.h"

/**
 * @brief A free list node that keeps track of free blocks.
 */
typedef struct FreeListNode {
    struct FreeListNode *next; /*< Pointer to the next node in the free list. */
    u64 size;                  /*< Size of the current block, including any padding. */
} FreeListNode;

/**
 * @brief The FreeList structure is used to manage memory allocations.
 */
typedef struct FreeList {
    FreeListNode *head; /*< Head node of the free list. */
    u64 totalFree;      /*< Total size of all available free blocks. */
    PlatformMutex lock; /*< Mutex lock for thread safety. */
} FreeList;

/**
 * @brief Creates a new freelist.
 *
 * @param freelist Pointer to the free list structure to initialize.
 * @param memoryPool
 */
void freelist_create(FreeList *freelist);

/**
 * @brief Destroys the free list.
 *
 * @param freelist Pointer to the free list to destroy.
 */
void freelist_destroy(FreeList *freelist);

/**
 * @brief Allocate a block of memory from the free list with the specified size
 * and alignment.
 *
 * @param freelist Pointer to the free list to allocate from.
 * @param size Size of the memory block to allocate.
 * @param alignment Alignment of the memory block to allocate.
 * @return void* Pointer to the allocated memory block.
 */
void *freelist_allocate(FreeList *freelist, u64 size, u16 alignment);

/**
 * @brief Free a block of memory from the free list with the specified size.
 *
 * @param freelist Pointer to the free list to free the memory block.
 * @param block Pointer to the memory block to free.
 * @param size Size of the memory block to free.
 */
void freelist_free(FreeList *freelist, void *block, u64 size);

#endif // ENGINE_FREELIST_H
