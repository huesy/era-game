#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"
#include <SDL3/SDL.h>

// Platform-specific data structure.
typedef struct SDL3_PlatformData {
    SDL_Window *window;     /**< The SDL window handle. */
    SDL_Renderer *renderer; /**< The SDL renderer handle. */
    b8 isRunning;           /**< True if the platform is running. */
    u64 absoluteTime;       /**< The absolute time of the platform. */
} SDL3_PlatformData;

// =============================================================================
#pragma region SDL3

/**
 * @brief Initializes the SDL3 platform.
 *
 * @param platform A pointer to the Platform structure.
 * @param pool A pointer to the memory pool structure.
 * @return ENGINE_SUCCESS if the platform was initialized successfully, otherwise an error code.
 */
static EngineResult sdl3_platform_init(Platform *platform, MemoryPool *pool) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        log_error("SDL_Init Error: %s", SDL_GetError());
        return ENGINE_FAILURE;
    }

    // Allocate platform-specific data.
    SDL3_PlatformData *data = (SDL3_PlatformData *)memory_allocate(pool, sizeof(SDL3_PlatformData), MEMORY_TAG_PLATFORM);
    if (!data) {
        log_error("Failed to allocate memory for SDL3_PlatformData.");
        SDL_Quit();
        return ENGINE_ERROR_ALLOCATION_FAILED;
    }

    SDL_WindowFlags flags = 0;

    if (platform->window->config.fullScreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Create SDL window.
    data->window = SDL_CreateWindow(platform->window->config.title,
                                    platform->window->config.width,
                                    platform->window->config.height,
                                    flags);

    if (!data->window) {
        log_error("SDL_CreateWindow Error: %s", SDL_GetError());
        memory_free(pool, data, MEMORY_TAG_PLATFORM);
        SDL_Quit();
        return ENGINE_FAILURE;
    }

    // Create SDL renderer.
    data->renderer = SDL_CreateRenderer(data->window, NULL);
    if (!data->renderer) {
        log_error("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(data->window);
        memory_free(pool, data, MEMORY_TAG_PLATFORM);
        SDL_Quit();
        return ENGINE_FAILURE;
    }

    data->isRunning = true;
    platform->data = data;

    log_info("SDL3 platform initialized successfully.");
    return ENGINE_SUCCESS;
}

/**
 * @brief Shuts down the SDL3 platform.
 *
 * @param platform A pointer to the Platform structure.
 * @return void
 */
static void sdl3_platform_shutdown(Platform *platform) {
    if (!platform || !platform->data) {
        log_warning("sdl3_platform_shutdown called with invalid platform or data.");
        return;
    }

    SDL3_PlatformData *data = (SDL3_PlatformData *)platform->data;

    // Destroy renderer.
    if (data->renderer) {
        SDL_DestroyRenderer(data->renderer);
    }

    // Destroy window.
    if (data->window) {
        SDL_DestroyWindow(data->window);
    }

    // Quit SDL.
    SDL_Quit();

    // Free platform-specific data.
    MemoryPool *pool = platform->memoryPool;
    if (pool) {
        memory_free(pool, data, MEMORY_TAG_PLATFORM);
    }

    platform->data = NULL;
    log_info("SDL3 platform shutdown completed.");
}

/**
 * @brief Polls SDL3 platform-specific events.
 *
 * @param platform A pointer to the Platform structure.
 * @return void
 */
static void sdl3_platform_poll_events(Platform *platform) {
    if (!platform || !platform->data) {
        log_warning("sdl3_platform_poll_events called with invalid platform or data.");
        return;
    }

    SDL_Event event;
    SDL3_PlatformData *data = (SDL3_PlatformData *)platform->data;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT: {
                data->isRunning = false;
            } break;

            default: {
            } break;
        }
    }
}

/**
 * @brief Checks if the SDL3 platform should continue running.
 *
 * @param platform A pointer to the Platform structure.
 * @return b8 True if the platform should continue running, otherwise false.
 */
static b8 sdl3_platform_is_running(Platform *platform) {
    if (!platform || !platform->data) {
        log_warning("sdl3_platform_is_running called with invalid platform or data.");
        return false;
    }

    SDL3_PlatformData *data = (SDL3_PlatformData *)platform->data;
    return data->isRunning;
}

/**
 * @brief Gets the absolute time of the SDL3 platform in milliseconds.
 *
 * @param platform A pointer to the Platform structure.
 * @return u64 representing the current absolute time.
 */
static u64 sdl3_platform_get_absolute_time(Platform *platform) {
    if (!platform || !platform->data) {
        log_warning("sdl3_platform_get_absolute_time called with invalid platform or data.");
        return 0;
    }

    SDL3_PlatformData *data = (SDL3_PlatformData *)platform->data;
    data->absoluteTime = SDL_GetTicks();
    return data->absoluteTime;
}

#pragma endregion
// =============================================================================
#pragma region Platform

ENGINE_API EngineResult platform_init(Platform *platform, MemoryPool *pool) {
    if (!platform || !pool) {
        log_error("Invalid Platform or MemoryPool provided to platform_init.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Assign function pointers.
    platform->init = sdl3_platform_init;
    platform->shutdown = sdl3_platform_shutdown;
    platform->pollEvents = sdl3_platform_poll_events;
    platform->isRunning = sdl3_platform_is_running;
    platform->getAbsoluteTime = sdl3_platform_get_absolute_time;

    // Initialize platform-specific data.
    return platform->init(platform, pool);
}

ENGINE_API void platform_shutdown(Platform *platform) {
    if (!platform) {
        log_error("Invalid Platform provided to platform_shutdown.");
        return;
    }

    platform->shutdown(platform);
}

ENGINE_API void platform_poll_events(Platform *platform) {
    if (!platform) {
        log_error("Invalid Platform provided to platform_poll_events.");
        return;
    }

    platform->pollEvents(platform);
}

ENGINE_API b8 platform_is_running(Platform *platform) {
    if (!platform) {
        log_error("Invalid Platform provided to platform_is_running.");
        return false;
    }

    return platform->isRunning(platform);
}

ENGINE_API u64 platform_get_absolute_time(Platform *platform) {
    if (!platform) {
        log_error("Invalid Platform provided to platform_get_absolute_time.");
        return 0;
    }

    return platform->getAbsoluteTime(platform);
}

#pragma endregion
// =============================================================================
#pragma region Renderer

ENGINE_API void platform_renderer_clear(Platform *platform) {
    if (!platform || !platform->data) {
        log_error("Invalid platform or platform data provided to platform_renderer_clear.");
        return;
    }

    SDL3_PlatformData *data = (SDL3_PlatformData *)platform->data;
    if (!data->renderer) {
        log_error("Invalid renderer handle provided to platform_renderer_clear.");
        return;
    }

    SDL_RenderClear(data->renderer);
}

ENGINE_API void platform_renderer_present(Platform *platform) {
    if (!platform || !platform->data) {
        log_error("Invalid platform or platform data provided to platform_renderer_present.");
        return;
    }

    SDL3_PlatformData *data = (SDL3_PlatformData *)platform->data;
    if (!data->renderer) {
        log_error("Invalid renderer handle provided to platform_renderer_present.");
        return;
    }

    SDL_RenderPresent(data->renderer);
}

#pragma endregion
// =============================================================================
#pragma region Memory

ENGINE_API void *platform_memory_allocate(u64 size) {
    return SDL_malloc(size);
}

ENGINE_API void platform_memory_free(void *block) {
    SDL_free(block);
}

ENGINE_API void *platform_memory_allocate_aligned(u64 size, u16 alignment) {
    return SDL_aligned_alloc(alignment, size);
}

ENGINE_API void platform_memory_free_aligned(void *block) {
    SDL_aligned_free(block);
}

ENGINE_API void *platform_memory_copy(void *dest, const void *src, u64 size) {
    return SDL_memcpy(dest, src, size);
}

ENGINE_API void *platform_memory_set(void *dest, i32 value, u64 size) {
    return SDL_memset(dest, value, size);
}

ENGINE_API void *platform_memory_zero(void *block, u64 size) {
    ENGINE_UNUSED(size);
    return SDL_zero(block);
}

#pragma endregion
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

#pragma endregion
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

#pragma endregion
// =============================================================================
