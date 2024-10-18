#include "engine/engine.h"
#include "engine/application.h"
#include "engine/logging.h"

ENGINE_API EngineResult engine_init(const EngineConfig *config, Engine *engine) {
    if (!config || !engine) {
        log_error("Invalid EngineConfig or Engine provided to engine_init.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Initialize the memory pool.
    if (memory_pool_init(&engine->memoryPool, config->memoryPoolSize) != ENGINE_SUCCESS) {
        log_error("Memory pool initialization failed.");
        return ENGINE_FAILURE;
    }

    // Initialize the platform.
    if (platform_init(&engine->platform, &engine->memoryPool) != ENGINE_SUCCESS) {
        log_error("Platform initialization failed.");
        memory_pool_shutdown(&engine->memoryPool);
        return ENGINE_FAILURE;
    }

    // TODO: Initialize other systems as needed.
    // e.g. renderer, audio, input, physics, etc.

    log_info("Engine initialized successfully.");
    return ENGINE_SUCCESS;
}

ENGINE_API void engine_shutdown(Engine *engine) {
    if (!engine) {
        log_error("Invalid Engine provided to engine_shutdown.");
        return;
    }

    // TODO: Shutdown other systems as needed.
    // e.g. renderer, audio, input, physics, etc.

    // Shut down platform.
    platform_shutdown(&engine->platform);

    // Shut down memory pool.
    memory_pool_shutdown(&engine->memoryPool);

    log_info("Engine shutdown completed.");
}

ENGINE_API void engine_run(Engine *engine, Application *app) {
    if (!engine || !app) {
        log_error("Invalid Engine or Application provided to engine_run.");
        return;
    }

    log_info("Running engine...");
    application_run(app);
}
