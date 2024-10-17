#include "engine/logging.h"
#include "engine/platform.h"
#include <SDL3/SDL.h>

typedef PlatformWindow SDL_Window;
SDL_Window *window;
}
;

struct PlatformRenderer {
    SDL_Renderer *renderer;
};

ENGINE_API EngineResult
platform_init(PlatformConfig *config, Platform *platform) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return ENGINE_ERROR;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_error("Failed to initialise SDL: %s", SDL_GetError());
        return ENGINE_FAILURE;
    }

    platform->isRunning = true;

    return ENGINE_SUCCESS;
}

ENGINE_API void
platform_shutdown(Platform *platform) {
    SDL_Quit();
}

ENGINE_API void
platform_poll_events(Platform *platform) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT: {
                platform->isRunning = false;
            } break;

            case SDL_EVENT_KEY_DOWN: {
                // Handle key down event.
            } break;

            case SDL_EVENT_KEY_UP: {
                // Handle key up event.
            } break;

            case SDL_EVENT_MOUSE_MOTION: {
                // Handle mouse motion event.
            } break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                // Handle mouse button down event.
            } break;

            case SDL_EVENT_MOUSE_BUTTON_UP: {
                // Handle mouse button up event.
            } break;

            case SDL_EVENT_WINDOW_RESIZED: {
                // Handle window resized event.
            } break;

            default: {
                // Handle other events.
            } break;
        }
    }
}

ENGINE_API b8
platform_is_running(Platform *platform) {
    return platform->isRunning;
}

// =============================================================================
// Windowing

ENGINE_API EngineResult
platform_create_window(WindowConfig *config, void *window) {
    if (!config) {
        log_error("Invalid window configuration.");
        return ENGINE_FAILURE;
    }

    if (!window) {
        log_error("Invalid window.");
        return ENGINE_FAILURE;
    }

    if (!config->title) {
        log_error("Invalid window title.");
        return ENGINE_FAILURE;
    }

    if (config->width <= 0 || config->height <= 0) {
        log_error("Invalid window dimensions.");
        return ENGINE_FAILURE;
    }

    // Create the SDL window that is:
    // - Resizable
    // - Vulkan compatible
    window = SDL_CreateWindow(
        config->title,
        config->width,
        config->height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

    if (window == NULL) {
        log_error("Failed to create window: %s", SDL_GetError());
        return ENGINE_FAILURE;
    }

    return ENGINE_SUCCESS;
}

ENGINE_API void
platform_destroy_window(void *window) {
    if (!window) {
        log_error("Invalid window.");
        return;
    }

    SDL_DestroyWindow((SDL_Window *)window);
}

ENGINE_API void
platform_set_window_title(void *window, const char *title) {
    if (!window) {
        log_error("Invalid window.");
        return;
    }

    if (!title) {
        log_error("Invalid window title.");
        return;
    }

    SDL_SetWindowTitle((SDL_Window *)window, title);
}

// =============================================================================
// Renderer

void *platform_create_renderer(void *config) {
    return SDL_CreateRenderer(
        (SDL_Window *)config->window,
        config->name);
}

void platform_destroy_renderer(void) {
    SDL_DestroyRenderer((SDL_Renderer *)renderer->handle);
}

// =============================================================================
// Dynamic Library Loading

ENGINE_API void *
platform_load_library(const char *path);
ENGINE_API void *
platform_get_library_function(void *library, const char *functionName);
ENGINE_API void
platform_unload_library(void *library);
ENGINE_API f32
platform_get_absolute_time(void);

// =============================================================================
// Memory

void *
platform_memory_allocate(u64 size) {
    return SDL_malloc(size);
}

void *
platform_memory_aligned_allocate(u64 size, u16 alignment) {
    return SDL_aligned_alloc(alignment, size);
}

void platform_memory_free(void *block) {
    SDL_free(block);
}

void *
platform_memory_copy(void *dest, const void *src, u64 size) {
    return SDL_memcpy(dest, src, size);
}

void *
platform_memory_set(void *dest, i32 value, u64 size) {
    return SDL_memset(dest, value, size);
}

void *
platform_memory_zero(void *block, u64 size) {
    ENGINE_UNUSED(size);
    return SDL_zero(block);
}

// =============================================================================
// Threading

void platform_mutex_init(void *lock) {
    lock = SDL_CreateMutex();
}

void platform_mutex_lock(void *lock) {
    SDL_LockMutex((SDL_Mutex *)lock);
}

void platform_mutex_unlock(void *lock) {
    SDL_UnlockMutex((SDL_Mutex *)lock);
}

void platform_mutex_destroy(void *lock) {
    SDL_DestroyMutex((SDL_Mutex *)lock);
}
