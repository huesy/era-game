#include "engine/memory/freelist.h"
#include "engine/memory.h"
#include "engine/platform.h"

void freelist_init(FreeList *freelist) {
    freelist->head = NULL;
    freelist->tail = NULL;
    freelist->count = 0;
    freelist->total = 0;
    freelist->capacity = 0;

    platform_mutex_init(&freelist->lock);
}

void *freelist_allocate(FreeList *freelist, u64 size, u16 alignment) {
    void *block = NULL;

    platform_mutex_lock(&freelist->lock);

    // Double-pointer so we can directly modify the head pointer.
    FreeListNode **current = &freelist->head;
    FreeListNode *prev = NULL;

    while (*current) {
        u64 address = (u64)(*current);
        // Calculate the aligned address for the block by adding the size of the FreeListNode
        // and aligning the address to the specified alignment.
        u64 aligned = (address + sizeof(FreeListNode) + alignment - 1) & ~(alignment - 1);
        // Calculate the padding between the aligned address and the current address.
        u64 padding = aligned - address - sizeof(FreeListNode);

        // Check if the current block has enough space for the requested size and alignment.
        if ((*current)->size >= size + padding) {
            // Calculate the address of the memory block.
            block = (void *)aligned;
            (*current)->size -= (padding + size);
            if ((*current)->size < sizeof(FreeListNode)) {
                *current = (*current)->next;
            }
            break;
        }

        current = &(*current)->next;
    }
}
