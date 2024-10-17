#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "engine/defines.h"

// =============================================================================
// Constants

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
// Interface

ENGINE_API void *memory_allocate(u64 size, MemoryTag tag);
ENGINE_API void *memory_aligned_allocate(u64 size, u16 alignment, MemoryTag tag);
ENGINE_API void memory_free(void *block, MemoryTag tag);
ENGINE_API void *memory_copy(void *dest, const void *src, u64 size);
ENGINE_API void *memory_set(void *dest, i32 value, u64 size);
ENGINE_API void *memory_zero(void *block, u64 size);

#endif // ENGINE_MEMORY_H
