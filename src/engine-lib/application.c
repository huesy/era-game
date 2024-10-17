#include "engine/application.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"

ENGINE_API EngineResult application_create(const ApplicationConfig *config, Application *app) {
    if (!config) {
        log_error("Invalid ApplicationConfig provided.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    if (!app) {
        log_error("Invalid Application pointer.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Initialize the platform.
    app->platform = (Platform *)memory_allocate(sizeof(Platform), MEMORY_TAG_PLATFORM);
    if (!app->platform) {
        log_error("Failed to allocate memory for platform.");
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    if (platform_init() != ENGINE_SUCCESS) {
        log_error("Failed to initialize platform.");
        memory_free(app->platform, MEMORY_TAG_PLATFORM);
        return ENGINE_FAILURE;
    }

    // Create window.
    app->window = (Window *)memory_allocate(sizeof(Window), MEMORY_TAG_PLATFORM);
    if (!app->window) {
        log_error("Failed to allocate memory for window.");
        platform_shutdown();
        memory_free(app->platform, MEMORY_TAG_PLATFORM);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    if (platform_window_create(&config->window, &app->window->platformWindow) != ENGINE_SUCCESS) {
        log_error("Failed to create window.");
        memory_free(app->window, MEMORY_TAG_PLATFORM);
        platform_shutdown();
        memory_free(app->platform, MEMORY_TAG_PLATFORM);
        return ENGINE_FAILURE;
    }

    // Create Renderer.
    app->renderer = (Renderer *)memory_allocate(sizeof(Renderer), MEMORY_TAG_RENDERER);
    if (!app->renderer) {
        log_error("Failed to allocate memory for renderer.");
        platform_window_destroy(app->window->platformWindow);
        memory_free(app->window, MEMORY_TAG_PLATFORM);
        platform_shutdown();
        memory_free(app->platform, MEMORY_TAG_PLATFORM);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    if (platform_renderer_create(&config->renderer, app->window->platformWindow, &app->renderer->platformRenderer) != ENGINE_SUCCESS) {
        log_error("Failed to create renderer.");
        memory_free(app->renderer, MEMORY_TAG_RENDERER);
        platform_window_destroy(app->window->platformWindow);
        memory_free(app->window, MEMORY_TAG_PLATFORM);
        platform_shutdown();
        memory_free(app->platform, MEMORY_TAG_PLATFORM);
        return ENGINE_FAILURE;
    }

    log_info("Application created successfully.");
    return ENGINE_SUCCESS;
}

ENGINE_API void application_run(Application *app) {
    if (!app) {
        log_error("Invalid application provided to application_run.");
        return;
    }

    b8 isRunning = true;

    while (isRunning) {
        // Poll events.
        platform_poll_events();

        // Check if the window is still open.
        if (!platform_window_is_open(app->window->platformWindow)) {
            isRunning = false;
        }

        // Update and render the editor.
        editor_update(0.016f); // Assuming ~60 FPS.
        editor_render();

        // Clear the renderer.
        renderer_clear(app->renderer, app->platform);

        // Present the renderer.
        renderer_present(app->renderer, app->platform);
    }

    log_info("Application run loop exited.");
}

ENGINE_API void application_shutdown(Application *app) {
    if (!app) {
        log_error("Invalid Application provided to application_shutdown.");
        return;
    }

    // Destroy Renderer.
    if (app->renderer) {
        platform_renderer_destroy(app->renderer->platformRenderer);
        memory_free(app->renderer, MEMORY_TAG_RENDERER);
    }

    // Destroy Window.
    if (app->window) {
        platform_window_destroy(app->window->platformWindow);
        memory_free(app->window, MEMORY_TAG_PLATFORM);
    }

    // Shutdown Platform.
    if (app->platform) {
        platform_shutdown();
        memory_free(app->platform, MEMORY_TAG_PLATFORM);
    }

    log_info("Application shutdown completed.");
}
