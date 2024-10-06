#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "defines.h"

typedef struct RendererAPI {
    void (*initialize)();
    void (*shutdown)();
    void (*draw)();
} RendererAPI;

/**
 * @brief Sets the renderer API to use.
 *
 * @param api The renderer API to use.
 */
void renderer_set_api(RendererAPI *api);

/**
 * @brief Initializes the renderer and sets up any necessary resources.
 */
void renderer_initialize(void);

/**
 * @brief Shuts down the renderer and cleans up any resources.
 */
void renderer_shutdown(void);

/**
 * @brief Draws the current frame.
 */
void renderer_draw(void);

#endif // ENGINE_RENDERER_H
