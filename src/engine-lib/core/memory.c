#include "engine/memory.h"
#include "engine/logging.h"
#include "engine/platform.h"

typedef struct MemoryStats {
    u64 totalAllocated;
    u64 totalFreed;
    u64 peakUsage;
} MemoryStats;

typedef struct MemoryState {
    b8 isInitialised;
    MemoryStats stats;
} MemoryState;

ENGINE_API void *memory_allocate(u64 size, MemoryTag tag) {

    ENGINE_UNUSED(tag);

    void *block = platform_memory_allocate(size);

    if (!block) {
        log_error("Failed to allocate memory block of size %llu.", size);
        return NULL;
    }

    // TODO: Track memory usage and statistics.

    return block;
}

ENGINE_API void *memory_aligned_allocate(u64 size, u16 alignment, MemoryTag tag) {

    ENGINE_UNUSED(tag);

    void *block = platform_memory_aligned_allocate(size, alignment);

    if (!block) {
        log_error("Failed to allocate memory block of size %llu.", size);
        return NULL;
    }

    // TODO: Track memory usage and statistics.

    return block;
}

ENGINE_API void memory_free(void *block, MemoryTag tag) {
    ENGINE_UNUSED(tag);

    platform_memory_free(block);
}

ENGINE_API void *memory_copy(void *dest, const void *src, u64 size) {
    return platform_memory_copy(dest, src, size);
}

ENGINE_API void *memory_set(void *dest, i32 value, u64 size) {
    return platform_memory_set(dest, value, size);
}

ENGINE_API void *memory_zero(void *block, u64 size) {
    return platform_memory_zero(block, size);
}
