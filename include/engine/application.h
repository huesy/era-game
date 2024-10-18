#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "engine/defines.h"
#include "engine/renderer.h"
#include "engine/window.h"

// Forward declarations.
typedef struct Engine Engine;
typedef struct Application Application;
typedef struct MemoryPool MemoryPool;
typedef struct Platform Platform;

// Callback function types.
typedef void (*AppUpdateFunc)(f32 deltaTime);
typedef void (*AppRenderFunc)(void);

/**
 * @brief Application configuration structure for initialization.
 */
typedef struct ApplicationConfig {
    WindowConfig window;     /**< Window configuration. */
    RendererConfig renderer; /**< Renderer configuration. */
    AppUpdateFunc update;    /**< Update callback function. */
    AppRenderFunc render;    /**< Render callback function. */
    // TODO: Add other callbacks.
} ApplicationConfig;

/**
 * @brief Application structure to represent the application instance.
 */
typedef struct Application {
    Renderer *renderer;     /**< Pointer to the renderer instance. */
    Window *window;         /**< Pointer to the window instance. */
    Platform *platform;     /**< Pointer to the platform instance. */
    AppUpdateFunc update;   /**< Update callback function. */
    AppRenderFunc render;   /**< Render callback function. */
    MemoryPool *memoryPool; /**< Pointer to the memory pool instance. */
    // TODO: Add other internal state (i.e. input, audio, etc.).
} Application;

/**
 * @brief Initializes the application with the given configuration.
 *
 * @param engine A pointer to the engine structure.
 * @param config A pointer to the application configuration structure.
 * @param app A pointer to the application structure.
 * @return ENGINE_SUCCESS if the application was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult application_init(Engine *engine, const ApplicationConfig *config, Application *app);

/**
 * @brief Runs the application's main loop.
 *
 * @param app A pointer to the application structure.
 * @return void
 */
ENGINE_API void application_run(Application *app);

/**
 * @brief Shuts down the application and frees resources.
 *
 * @param app A pointer to the application structure.
 * @return void
 */
ENGINE_API void application_shutdown(Application *app);

#endif // ENGINE_APPLICATION_H
