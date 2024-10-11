#include "engine/memory/aligned_alloc.h"
#include "engine/logging.h"
#include "engine/platform.h"

void *aligned_alloc(u64 size, u16 alignment) {
    // Allocate extra memory for alignment purposes. sizeof(void *) is added to
    // store the original pointer for freeing the memory later.

    // size: 9
    // alignment: 4
    // totalSize: 9 + 4 - 1 + 8 = 20
    // Memory: [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
    u64 totalSize = size + alignment - 1 + sizeof(void *);
    void *rawMemory = platform_memory_allocate(totalSize);
    if (!rawMemory) {
        log_error("Failed to allocate memory block.");
        return NULL;
    }

    // Calculate the aligned address and store the original pointer before it.
    // rawAddress: 0x0000000000000000
    // alignedAddress: 0x0000000000000008
    // 0x0000000000000000 + 4 - 1 = 0x0000000000000003
    // Memory: [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]

    u64 rawAddress = (u64)rawMemory;
    u64 alignedAddress = (rawAddress + alignment - 1) & ~(alignment - 1);

    return platform_aligned_alloc(size, alignment);
}
