#ifndef ENGINE_FREELIST_H
#define ENGINE_FREELIST_H

#include "engine/defines.h"
#include "engine/platform.h"

/**
 * @brief Define a FreeListNode structure to keep track of free memory blocks.
 */
typedef struct FreeListNode {
    u64 size;                  /*< Size of the free block. */
    struct FreeListNode *next; /*< Pointer to the next free block in the list. */
} FreeListNode;

/**
 * @brief Define a FreeList structure to manage free memory blocks.
 */
typedef struct FreeList {
    FreeListNode *head; /*< Pointer to the head of the free list. */
    FreeListNode *tail; /*< Pointer to the tail of the free list. */
    u64 count;          /*< Number of free blocks in the list. */
    u64 total;          /*< Total size of all free blocks in the list. */
    u64 capacity;       /*< Total capacity of the free list. */
    PlatformMutex lock; /*< Mutex lock for thread safety. */
} FreeList;

/**
 * @brief Initialize the free list.
 *
 * @param freelist Pointer to the free list to initialize.
 */
void freelist_init(FreeList *freelist);

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
