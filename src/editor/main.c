#include "editor/editor.h"
#include <engine/engine.h>
#include <stdio.h>
#include <time.h>

int main(void) {

    log_info("Starting editor...");

    // Define engine configuration
    EngineConfig engineConfig = {0};
    engineConfig.memoryPoolSize = 1024 * 1024 * 64; // 64MB

    // Allocate and initialize the application.
    Application *app = (Application *)memory_allocate(sizeof(Application), MEMORY_TAG_ENGINE);
    if (!app) {
        log_error("Failed to allocate memory for application.");
        return -1;
    }

    // Initialize the engine.
    if (engine_init(&engineConfig, app) != ENGINE_SUCCESS) {
        log_error("Failed to initialize engine.");
        memory_free(app, MEMORY_TAG_ENGINE);
        return -1;
    }

    // Run the main loop.
    engine_run(app);

    // Shutdown the engine.
    engine_shutdown(app);

    // Free the application memory.
    memory_free(app, MEMORY_TAG_ENGINE);

    log_info("Editor finished.");
    return 0;
}
