#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include "engine/defines.h"

// clang-format off
#define MEMORY_TAG_UNKNOWN      0
#define MEMORY_TAG_ARRAY        1
#define MEMORY_TAG_DARRAY       2
#define MEMORY_TAG_DICT         3
#define MEMORY_TAG_RING_QUEUE   4
#define MEMORY_TAG_BST          5
#define MEMORY_TAG_STRING       6
#define MEMORY_TAG_APPLICATION  7
#define MEMORY_TAG_JOB          8
#define MEMORY_TAG_TEXTURE      9
#define MEMORY_TAG_MATERIAL     10
#define MEMORY_TAG_RENDERER     11
#define MEMORY_TAG_GAME         12
#define MEMORY_TAG_TRANSFORM    13
#define MEMORY_TAG_ENTITY       14
#define MEMORY_TAG_ENTITY_NODE  15
#define MEMORY_TAG_SCENE        16
#define MEMORY_TAG_PLUGIN       17
// clang-format on

typedef struct MemoryStats {
    u64 totalAllocated;
    u64 taggedAllocations[32];
    u64 totalFreed;
    u64 peakUsage;
    u64 currentUsage;
} MemoryStats;

b8 memory_system_init(u64 totalSize);
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
