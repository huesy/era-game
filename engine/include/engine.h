#ifndef ENGINE_H
#define ENGINE_H

#include "core/logging.h"
#include "defines.h"

typedef struct WindowConfig {
    u32 width;         /**< The width of the window. */
    u32 height;        /**< The height of the window. */
    const char *title; /**< The title of the window. */
} WindowConfig;

typedef struct ApplicationConfig {
    const char *name;    /**< The name of the application. */
    WindowConfig window; /**< The configuration for the application window. */
} ApplicationConfig;

typedef struct Application {
    /** @brief The configuration for the application. */
    ApplicationConfig config;
    /** @brief The function to call to init the application. */
    void (*onInit)(void);
    /** @brief The function to call to shut down the application. */
    void (*onShutdown)(void);
    /** @brief The function to call to update the application. */
    void (*onUpdate)(f32 deltaTime);
    /** @brief The function to call to render the application. */
    void (*onRender)(void);
} Application;

/**
 * @brief Registers the application lifecycle with the engine.
 *
 * @param app A pointer to the application structure.
 */
ENGINE_API void engine_register_application(Application *app);

#endif // ENGINE_H
