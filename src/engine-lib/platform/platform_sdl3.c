#include "engine/logging.h"
#include "engine/platform.h"
#include <SDL3/SDL.h>

// Platform-specific window structure.
struct PlatformWindow {
    SDL_Window *handle; /*< The SDL window handle. */
};

// Platform-specific renderer structure.
struct PlatformRenderer {
    SDL_Renderer *handle; /*< The SDL renderer handle. */
};

// =============================================================================
#pragma region Platform

ENGINE_API EngineResult platform_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        log_error("SDL initialization failed: %s", SDL_GetError());
        return ENGINE_FAILURE;
    }

    log_info("SDL Initialized.");
    return ENGINE_SUCCESS;
}

ENGINE_API void platform_shutdown(void) {
    SDL_Quit();
    log_info("SDL Shutdown.");
}

// =============================================================================
#pragma region Window

ENGINE_API EngineResult platform_window_create(const PlatformWindowConfig *config, PlatformWindow **window) {
    if (!config || !window) {
        log_error("Invalid arguments to platform_window_create.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    u32 flags;
    if (config->fullScreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Attempt to create the window.
    SDL_Window *sdlWindow = SDL_CreateWindow(
        config->title,
        config->width,
        config->height,
        flags);

    if (!sdlWindow) {
        log_error("Failed to create window: %s", SDL_GetError());
        return ENGINE_FAILURE;
    }

    // Set the window position if an X or Y is non-zero.
    if (config->x != 0 || config->y != 0) {
        SDL_SetWindowPosition(sdlWindow, config->x, config->y);
    }

    *window = (PlatformWindow *)platform_memory_allocate(sizeof(PlatformWindow));
    if (!*window) {
        log_eror("Failed to allocate memory for PlatformWindow.");
        SDL_DestroyWindow(sdlWindow);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    (*window)->handle = sdlWindow;
    log_info("SDL Window created: %s (%dx%d)", config->title, config->width, config->height);
    return ENGINE_SUCCESS;
}

ENGINE_API void platform_window_destroy(PlatformWindow *window) {
    if (!window || !window->handle) {
        log_error("Invalid window or handle.");
        return;
    }

    SDL_DestroyWindow(window->handle);
    platform_memory_free(window);
}

// =============================================================================
#pragma region Renderer

ENGINE_API EngineResult platform_renderer_create(
    const PlatformRendererConfig *config,
    PlatformWindow *window,
    PlatformRenderer **renderer) {

    if (!config || !window || !renderer) {
        log_error("Invalid arguments provided to platform_renderer_create.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(
        window->handle,
        config->name);

    if (!sdlRenderer) {
        log_error("Failed to create SDL renderer: %s", SDL_GetError());
        return ENGINE_FAILURE;
    }

    *renderer = (PlatformRenderer *)platform_memory_allocate(sizeof(PlatformRenderer));
    if (!*renderer) {
        log_error("Failed to allocate memory for PlatformRenderer.");
        SDL_DestroyRenderer(sdlRenderer);
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    (*renderer)->handle = sdlRenderer;
    log_info("SDL Renderer created.");
    return ENGINE_SUCCESS;
}

ENGINE_API void platform_renderer_destroy(PlatformRenderer *renderer) {
    if (!renderer) {
        log_error("Invalid renderer provided to platform_renderer_destroy.");
        return;
    }

    if (renderer->handle) {
        SDL_DestroyRenderer(renderer->handle);
        log_info("SDL Renderer destroyed.");
    }

    platform_memory_free(renderer);
}

ENGINE_API void platform_renderer_present(PlatformRenderer *renderer) {
    if (!renderer || !renderer->handle) {
        log_error("Invalid renderer provided to platform_renderer_present.");
        return;
    }

    SDL_RenderPresent(renderer->handle);
}

ENGINE_API void platform_renderer_set_draw_color(
    PlatformRenderer *renderer,
    u8 r, u8 g, u8 b, u8 a) {

    if (!renderer || !renderer->handle) {
        log_error("Invalid renderer provided to platform_renderer_set_draw_color.");
        return;
    }

    SDL_SetRenderDrawColor(renderer->handle, r, g, b, a);
}

ENGINE_API void platform_renderer_clear(PlatformRenderer *renderer) {
    if (!renderer || !renderer->handle) {
        log_error("Invalid renderer provided to platform_renderer_clear.");
        return;
    }

    SDL_RenderClear(renderer->handle);
}

// =============================================================================
#pragma region Timing

ENGINE_API u64 platform_get_absolute_time(void) {
    return (u64)SDL_GetTicks();
}

// =============================================================================
#pragma region Memory

ENGINE_API void *platform_memory_allocate(u64 size) {
    return SDL_malloc(size);
}

ENGINE_API void platform_memory_free(void *block) {
    SDL_free(block);
}

ENGINE_API void *platform_memory_aligned_allocate(u64 size, u16 alignment) {
    return SDL_aligned_alloc(alignment, size);
}

ENGINE_API void platform_memory_aligned_free(void *block) {
    SDL_aligned_free(block);
}

ENGINE_API *platform_memory_copy(void *dest, const void *src, u64 size) {
    return SDL_memcpy(dest, src, size);
}

ENGINE_API *platform_memory_set(void *dest, i32 value, u64 size) {
    return SDL_memset(dest, value, size);
}

ENGINE_API *platform_memory_zero(void *block, u64 size) {
    ENGINE_UNUSED(size);
    return SDL_zero(block);
}

// =============================================================================
#pragma region Threading

ENGINE_API void platform_mutex_create(void **lock) {
    *lock = SDL_CreateMutex();

    if (!*lock) {
        log_error("Failed to create mutex: %s", SDL_GetError());
    }
}

ENGINE_API void platform_mutex_destroy(void *lock) {
    if (!lock) {
        log_error("Invalid lock provided to platform_mutex_destroy.");
        return;
    }

    SDL_DestroyMutex((SDL_Mutex *)lock);
}

ENGINE_API void platform_mutex_lock(void *lock) {
    SDL_LockMutex((SDL_Mutex *)lock);
}

ENGINE_API void platform_mutex_unlock(void *lock) {
    SDL_UnlockMutex((SDL_Mutex *)lock);
}

// =============================================================================
#pragma region Dynamic Library

ENGINE_API void *platform_dynamic_library_load(const char *path) {
    return SDL_LoadObject(path);
}

ENGINE_API void platform_dynamic_library_unload(void *library) {
    SDL_UnloadObject(library);
}

ENGINE_API void *platform_dynamic_library_load_function(
    void *library,
    const char *symbol) {
    return SDL_LoadFunction(library, symbol);
}
