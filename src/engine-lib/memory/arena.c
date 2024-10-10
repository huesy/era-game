#include "engine/memory/arena.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"

EngineResult memory_arena_create(MemoryArena *arena, u64 capacity, u16 alignment) {

    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");
    ENGINE_ASSERT(capacity > 0, "Capacity must be greater than zero.");
    ENGINE_ASSERT((alignment & (alignment - 1)) == 0, "Alignment must be a power of 2.");

    arena->memory = memory_allocate(capacity, MEMORY_TAG_ARENA);
    if (arena->memory == NULL) {
        log_error("Memory arena allocation failed."
                  "\n\tRequested capacity: %llu bytes"
                  "\n\tAlignment: %u bytes",
                  capacity, alignment);

        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    arena->alignment = alignment;
    arena->capacity = capacity;
    arena->used = 0;
    arena->marker = 0;

    // Initialize mutex for thread safety.
    platform_mutex_init(&arena->mutex);

    log_debug("Memory arena created:"
              "\nCapacity: %llu bytes"
              "\nAlignment: %u bytes",
              capacity, alignment);

    return ENGINE_SUCCESS;
}

void memory_arena_destroy(MemoryArena *arena) {
    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");

    if (arena->memory != NULL) {
        memory_free(arena->memory, arena->capacity, MEMORY_TAG_ARENA);
        arena->memory = NULL;
    }

    arena->capacity = 0;
    arena->used = 0;
    arena->marker = 0;

    // Destroy mutex.
    platform_mutex_destroy(&arena->mutex);

    log_debug("Memory arena destroyed.");
}

void *memory_arena_allocate(MemoryArena *arena, u64 size) {

    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");
    ENGINE_ASSERT(size > 0, "Allocation size must be greater than zero.");

    platform_mutex_lock(&arena->mutex);

    // Calculate the current aligned address.
    u64 currentAddress = (u64)(arena->memory) + arena->used;
    u64 alignedAddress = (currentAddress + (arena->alignment - 1)) & ~(arena->alignment - 1);
    u64 padding = alignedAddress - currentAddress;

    // Ensure that padding does not cause overflow.
    if (arena->used > arena->capacity - padding) {
        log_error("Padding causes overflow. Allocation failed."
                  "\n\tRequested size: %llu bytes"
                  "\n\tCurrent used: %llu bytes"
                  "\n\tTotal capacity: %llu bytes"
                  "\n\tPadding: %llu bytes",
                  size, arena->used, arena->capacity, padding);

        platform_mutex_unlock(&arena->mutex);

        return NULL;
    }

    u64 newUsed = arena->used + padding;

    // Check if there's enough space for the requested allocation.
    if (newUsed > arena->capacity) {
        log_error("Memory arena is full. Allocation failed."
                  "\n\tRequested size: %llu bytes"
                  "\n\tNew used: %llu bytes"
                  "\n\tTotal capacity: %llu bytes",
                  size, newUsed, arena->capacity);

        platform_mutex_unlock(&arena->mutex);

        return NULL;
    }

    arena->used = newUsed + size;

    log_debug("Memory allocated:"
              "\n\tRequested size: %llu bytes"
              "\n\tPadding: %llu bytes"
              "\n\tTotal used: %llu bytes"
              "\n\tRemaining capacity: %llu bytes",
              size, padding, arena->used, arena->capacity - arena->used);

    void *allocatedMemory = (void *)alignedAddress;

    platform_mutex_unlock(&arena->mutex);

    return allocatedMemory;
}

void memory_arena_reset(MemoryArena *arena) {
    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");

    platform_mutex_lock(&arena->mutex);

    arena->used = 0;
    arena->marker = 0;

    platform_mutex_unlock(&arena->mutex);

    log_debug("Memory arena reset.");
}

u64 memory_arena_get_marker(MemoryArena *arena) {
    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");

    platform_mutex_lock(&arena->mutex);
    u64 currentMarker = arena->marker;
    platform_mutex_unlock(&arena->mutex);

    return currentMarker;
}

void memory_arena_reset_to_marker(MemoryArena *arena, u64 marker) {
    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");
    ENGINE_ASSERT(marker <= arena->used, "Marker position is invalid.");

    platform_mutex_lock(&arena->mutex);
    arena->used = marker;
    platform_mutex_unlock(&arena->mutex);

    log_debug("Memory arena reset to marker: %llu", marker);
}

void memory_arena_set_marker(MemoryArena *arena) {
    ENGINE_ASSERT(arena != NULL, "Arena pointer cannot be NULL.");

    platform_mutex_lock(&arena->mutex);
    arena->marker = arena->used;
    platform_mutex_unlock(&arena->mutex);

    log_debug("Memory arena marker set to: %llu bytes", arena->marker);
}
