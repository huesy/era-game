#ifndef ENGINE_OPENGL_RENDERER_H
#define ENGINE_OPENGL_RENDERER_H

#define GL_SILENCE_DEPRECATION

/**
 * @brief Initialises the OpenGL renderer API.
 */
void opengl_renderer_init(void);

/**
 * @brief Shuts down the OpenGL renderer API.
 */
void opengl_renderer_shutdown(void);

/**
 * @brief Draws the current frame using OpenGL.
 */
void opengl_renderer_draw(void);

#endif // ENGINE_OPENGL_RENDERER_H
