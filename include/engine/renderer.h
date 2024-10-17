#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "engine/defines.h"
#include "engine/platform.h"

// Renderer configuration structure.
typedef struct RendererConfig {

} RendererConfig;

// Renderer structure.
typedef struct Renderer {
    PlatformRenderer *platformRenderer;
} Renderer;

// =============================================================================
// Renderer

/**
 * @brief Creates a new renderer.
 *
 * @param config A pointer to the renderer configuration structure.
 * @param renderer A pointer to the renderer to create.
 * @param platform A pointer to the platform to create the renderer on.
 * @return ENGINE_SUCCESS if the renderer was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult
renderer_create(RendererConfig *config, Renderer *renderer, Platform *platform);

/**
 * @brief Destroys a renderer.
 *
 * @param renderer A pointer to the renderer to destroy.
 * @param platform A pointer to the platform to destroy the renderer on.
 * @return void
 */
ENGINE_API void
renderer_destroy(Renderer *renderer, Platform *platform);

/**
 * @brief Presents the renderer.
 *
 * @param renderer A pointer to the renderer to present.
 * @param platform A pointer to the platform to present the renderer on.
 * @return void
 */
ENGINE_API void
renderer_present(Renderer *renderer, Platform *platform);

/**
 * @brief Sets the draw color for the renderer.
 *
 * @param renderer A pointer to the renderer to set the draw color on.
 * @param r The red component of the color.
 * @param g The green component of the color.
 * @param b The blue component of the color.
 * @param a The alpha component of the color.
 * @return void
 */
ENGINE_API void
renderer_set_draw_color(Renderer *renderer, u8 r, u8 g, u8 b, u8 a);

/**
 * @brief Clears the renderer.
 *
 * @param renderer A pointer to the renderer to clear.
 * @param platform A pointer to the platform to clear the renderer on.
 * @return void
 */
ENGINE_API void
renderer_clear(Renderer *renderer, Platform *platform);

#endif // ENGINE_RENDERER_H
