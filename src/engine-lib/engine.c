#include "engine/engine.h"
#include "engine/memory.h"
#include <stdio.h>

ENGINE_API b8 engine_init(EngineConfig *config) {
    // Initialize memory system.
    memory_system_init(config->memoryPoolSize);

    return true;
}

ENGINE_API void engine_shutdown(void) {
    memory_system_shutdown();
}
