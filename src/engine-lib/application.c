#include "engine/application.h"
#include "engine/engine.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"
#include "engine/window.h"

ENGINE_API EngineResult application_init(Engine *engine, const ApplicationConfig *config, Application *app) {
    if (!engine || !config || !app) {
        log_error("Invalid Engine, ApplicationConfig, or Application provided to application_init.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Assign memory pool and platform from the engine.
    app->memoryPool = &engine->memoryPool;
    app->platform = &engine->platform;

    // Allocate memory for the window.
    app->window = (Window *)memory_allocate(app->memoryPool, sizeof(Window), MEMORY_TAG_PLATFORM);
    if (!app->window) {
        log_error("Failed to allocate memory for window.");
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    // Initialize the window.
    if (window_init(app->memoryPool, &config->window, app->window) != ENGINE_SUCCESS) {
        log_error("Failed to initialize window.");
        memory_free(app->memoryPool, app->window, MEMORY_TAG_PLATFORM);
        return ENGINE_FAILURE;
    }

    // Allocate memory for the renderer.
    app->renderer = (Renderer *)memory_allocate(app->memoryPool, sizeof(Renderer), MEMORY_TAG_RENDERER);
    if (!app->renderer) {
        log_error("Failed to allocate memory for renderer.");
        window_shutdown(app->memoryPool, app->window);
        memory_free(app->memoryPool, app->window, MEMORY_TAG_PLATFORM);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    // Initialize the renderer.
    if (renderer_init(app->memoryPool, &config->renderer, app->renderer, app->platform) != ENGINE_SUCCESS) {
        log_error("Failed to initialize renderer.");
        memory_free(app->memoryPool, app->renderer, MEMORY_TAG_RENDERER);
        window_shutdown(app->memoryPool, app->window);
        memory_free(app->memoryPool, app->window, MEMORY_TAG_PLATFORM);
        return ENGINE_FAILURE;
    }

    // Store user callbacks.
    app->update = config->update;
    app->render = config->render;

    log_info("Application initialized successfully.");
    return ENGINE_SUCCESS;
}

ENGINE_API void application_run(Application *app) {
    if (!app) {
        log_error("Invalid application provided to application_run.");
        return;
    }

    log_info("Starting application run loop.");

    while (platform_is_running(app->platform)) {
        // Poll platform-specific events.
        platform_poll_events(app->platform);

        // Calculate deltaTime.
        static u64 lastTime = 0;
        u64 currentTime = platform_get_absolute_time(app->platform);
        f32 deltaTime = (f32)(currentTime - lastTime) * 0.001f; // Convert to milliseconds.
        lastTime = currentTime;

        // Update logic.
        if (app->update) {
            app->update(deltaTime);
        }

        // Render.
        if (app->render) {
            app->render();
        }

        // Clear the renderer.
        renderer_clear(app->renderer, app->platform);

        // Present the renderer.
        renderer_present(app->renderer, app->platform);
    }

    log_info("Exiting application run loop.");
}

ENGINE_API void application_shutdown(Application *app) {
    if (!app) {
        log_error("Invalid Application pointer provided to application_shutdown.");
        return;
    }

    // Shutdown Renderer.
    if (app->renderer) {
        renderer_shutdown(app->renderer, app->platform);
        memory_free(app->memoryPool, app->renderer, MEMORY_TAG_RENDERER);
    }

    // Shutdown Window.
    if (app->window) {
        window_shutdown(app->memoryPool, app->window);
        memory_free(app->memoryPool, app->window, MEMORY_TAG_PLATFORM);
    }

    log_info("Application shutdown completed.");
}
