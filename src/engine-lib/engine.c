#include "engine/engine.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"
#include <stdio.h>

EngineResult engine_init(EngineConfig *config) {
    log_info("Initializing engine...");

    // Initialize memory system.
    if (memory_system_init(config->memoryPoolSize) != ENGINE_SUCCESS) {
        log_error("Failed to initialize memory system.");
        return ENGINE_ERROR;
    }

    PlatformConfig platformConfig = {0};
    platformConfig.width = 1280;
    platformConfig.height = 720;
    platformConfig.title = "Era Engine";

    if (platform_init(&platformConfig) != ENGINE_SUCCESS) {
        log_error("Failed to initialize platform.");
        return ENGINE_ERROR;
    }

    log_info("Engine initialized successfully.");
    return ENGINE_SUCCESS;
}

void engine_shutdown(void) {
    log_info("Shutting down engine...");
    platform_shutdown();
    memory_system_shutdown();
}

void engine_run(void) {
    log_info("Running engine...");

    while (platform_is_running()) {
        platform_poll_events();
        // TODO: Additional game loop logic here.
    }

    log_info("Engine stopped.");
}
