/**
 * @file engine.h
 * @author Andrew Hughes (a.hughes@gmail.com)
 * @brief Core engine header file providing initialization, update, and shutdown
 * interfaces.
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Era Engine is Copyright (c) Andrew Hughes 2024
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "engine/defines.h"
#include "engine/memory.h"
#include "engine/platform.h"

// Forward declarations.
typedef struct Engine Engine;
typedef struct EngineConfig EngineConfig;
typedef struct Application Application;

// =============================================================================

/**
 * @brief Configuration structure for initializing the engine.
 */
typedef struct EngineConfig {
    u64 memoryPoolSize; /**< Size of the memory pool in bytes. */
    // TODO: Other configuration params as needed.
} EngineConfig;

/**
 * @brief State structure for the engine, encapsulating all major systems.
 */
typedef struct Engine {
    MemoryPool memoryPool; /**< Memory pool for allocations. */
    Platform platform;     /**< Platform interface. */
    // TODO: Add other core systems (i.e. renderer, audio, input, physics, etc.).
} Engine;

// =============================================================================
#pragma region Interface

/**
 * @brief Initializes the engine with the given configuration.
 *
 * @param config A pointer to the configuration parameters for the engine.
 * @param engine A pointer to the Engine structure to initialize.
 * @return ENGINE_SUCCESS if the engine was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult engine_init(const EngineConfig *config, Engine *engine);

/**
 * @brief Shuts down the engine and frees all resources.
 *
 * @param engine A pointer to the Engine state.
 * @return void
 */
ENGINE_API void engine_shutdown(Engine *engine);

/**
 * @brief Runs the main engine update loop.
 *
 * @param engine A pointer to the Engine struct to run.
 * @param app A pointer to the Application struct to run.
 * @return void
 */
ENGINE_API void engine_run(Engine *engine, Application *app);

#endif // ENGINE_H
