#include "engine/engine.h"
#include "engine/logging.h"

ENGINE_API EngineResult engine_init(const EngineConfig *config, Application *app) {
    if (!config || !app) {
        log_error("Invalid arguments to engine_init.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Initialize the memory pool.
    if (memory_pool_init(config->memoryPoolSize) != ENGINE_SUCCESS) {
        log_error("Memory pool initialization failed.");
        return ENGINE_FAILURE;
    }

    // Window config
    // TODO: Give this to the binary to set.
    WindowConfig windowConfig = {0};
    windowConfig.title = "Era Engine";
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.fullScreen = false;

    // Renderer config.
    // TODO: Give this to the binary to set.
    RendererConfig rendererConfig = {0};
    rendererConfig.name = "SDL3";

    // Define application configuration.
    ApplicationConfig appConfig = {0};
    appConfig.window = windowConfig;
    appConfig.renderer = rendererConfig;

    // Create the application.
    if (application_create(&appConfig, app) != ENGINE_SUCCESS) {
        log_error("Failed to create Application.");
        memory_pool_shutdown();
        return ENGINE_FAILURE;
    }

    log_info("Engine initialized successfully.");
    return ENGINE_SUCCESS;
}

void engine_shutdown(void) {
    log_info("Shutting down engine...");
    platform_shutdown();
}

void engine_run(void) {
    log_info("Running engine...");

    while (platform_is_running()) {
        platform_poll_events();
        // TODO: Additional game loop logic here.
    }

    log_info("Engine stopped.");
}
