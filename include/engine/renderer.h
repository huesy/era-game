#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "engine/defines.h"
#include "engine/memory.h"
#include "engine/platform.h"

/**
 * @brief Configuration structure for initializing a new renderer.
 */
typedef struct RendererConfig {
    const char *name; /**< The name of the renderer to create. */
} RendererConfig;

/**
 * @brief Renderer structure to represent a renderer instance.
 */
typedef struct Renderer {
    RendererConfig config; /**< Configuration for the renderer. */
} Renderer;

// =============================================================================

/**
 * @brief Initializes the renderer
 *
 * @param pool A pointer to the memory pool to allocate the renderer from.
 * @param config A pointer to the renderer configuration structure.
 * @param renderer A pointer to the renderer to create.
 * @param platform A pointer to the platform to create the renderer on.
 * @return ENGINE_SUCCESS if the renderer was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult renderer_init(MemoryPool *pool, const RendererConfig *config, Renderer *renderer, Platform *platform);

/**
 * @brief Shuts down the renderer and frees resources.
 *
 * @param renderer A pointer to the renderer to destroy.
 * @param platform A pointer to the platform to destroy the renderer on.
 * @return void
 */
ENGINE_API void renderer_shutdown(Renderer *renderer, Platform *platform);

/**
 * @brief Presents the renderer (e.g. updates the screen).
 *
 * @param renderer A pointer to the renderer to present.
 * @param platform A pointer to the platform to present the renderer on.
 * @return void
 */
ENGINE_API void renderer_present(Renderer *renderer, Platform *platform);

/**
 * @brief Clears the renderer (e.g. clears the screen).
 *
 * @param renderer A pointer to the renderer to clear.
 * @param platform A pointer to the platform to clear the renderer on.
 * @return void
 */
ENGINE_API void renderer_clear(Renderer *renderer, Platform *platform);

#endif // ENGINE_RENDERER_H
