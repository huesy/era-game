#ifndef ENGINE_MEMORY_ARENA_H
#define ENGINE_MEMORY_ARENA_H

#include "engine/defines.h"

/**
 * @brief MemoryArena allocator structure.
 */
typedef struct MemoryArena {
    u8 *memory;          /*< Pointer to the memory block. */
    u16 alignment;       /*< Alignment of the memory block. */
    u64 capacity;        /*< Total capacity of the memory block. */
    u64 used;            /*< Amount of memory used. */
    u64 marker;          /*< Marker to keep track of the current memory allocation. */
    PlatformMutex mutex; /*< Mutex lock for thread safety. */
} MemoryArena;

/**
 * @brief Create a new arena memory allocator.
 *
 * @param arena Pointer to the arena structure to initialize.
 * @param capacity Total capacity of the memory block.
 * @param alignment Alignment of the memory block.
 * @return EngineResult ENGINE_SUCCESS if successful, otherwise an error code.
 */
EngineResult memory_arena_create(MemoryArena *arena, u64 capacity, u16 alignment);

/**
 * @brief Destroy the arena memory allocator.
 *
 * @param arena Pointer to the arena structure to destroy.
 */
void memory_arena_destroy(MemoryArena *arena);

/**
 * @brief Allocate a block of memory from the arena memory with the specified size.
 *
 * This function aligns the allocation based on the arena's alignment requirement
 * and updates the arena's used memory accordingly. If there is insufficient space,
 * it logs an error and returns NULL.
 *
 * @param arena Pointer to the memory arena.
 * @param size The size of the memory block to allocate in bytes.
 * @return void* Pointer to the allocated memory block, or NULL if allocation fails.
 */
void *memory_arena_allocate(MemoryArena *arena, u64 size);

/**
 * @brief Reset the arena memory allocator to the initial state.
 *
 * @param arena Pointer to the arena memory allocator.
 */
void memory_arena_reset(MemoryArena *arena);

/**
 * @brief Get the current marker position of the arena memory allocator.
 *
 * @param arena Pointer to the arena memory allocator.
 * @return u64 The current marker position.
 */
u64 memory_arena_get_marker(MemoryArena *arena);

/**
 * @brief Reset the arena memory allocator to the specified marker position.
 *
 * @param arena Pointer to the arena memory allocator.
 * @param marker Marker position to reset the arena to.
 */
void memory_arena_reset_to_marker(MemoryArena *arena, u64 marker);

#endif // ENGINE_MEMORY_ALLOCATORS_ARENA_H
