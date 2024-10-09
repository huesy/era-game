#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "defines.h"

// clang-format off
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
    MEMORY_TAG_COMPONENT,
    MEMORY_TAG_SCENE,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_PLUGIN,
    MEMORY_TAG_EDITOR
} MemoryTag;

typedef struct MemoryStats {
    u64 totalAllocated;
    u64 taggedAllocations[32];
    u64 totalFreed;
    u64 peakUsage;
    u64 currentUsage;
} MemoryStats;

EngineResult memory_system_init(u64 totalSize);
void memory_system_shutdown(void);

void *memory_allocate(u64 size, u32 tag);
void memory_free(void *block, u64 size, u32 tag);
void *memory_zero(void *block, u64 size);
void *memory_copy(void *dest, const void *src, u64 size);
void *memory_set(void *dest, i32 value, u64 size);

MemoryStats memory_get_stats(void);

// Temporary memory allocation. Only valid until next temporary allocation or reset.
void *memory_allocate_temporary(u64 size);
void memory_reset_temporary(void);

#endif // ENGINE_MEMORY_H
