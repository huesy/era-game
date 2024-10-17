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

#include "application.h"
#include "defines.h"

// =============================================================================
#pragma region Constants

typedef struct EngineConfig {
    u64 memoryPoolSize;
} EngineConfig;

// =============================================================================
#pragma region Interface

/**
 * @brief Initializes the engine.
 *
 * @param config Engine configuration.
 * @param app A pointer to the Application struct to initialize.
 * @return ENGINE_SUCCESS if the engine was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult engine_init(const EngineConfig *config, Application *app);

/**
 * @brief Shuts down the engine and cleans up resources.
 *
 * @param app A pointer to the Application struct to shut down.
 * @return void
 */
ENGINE_API void engine_shutdown(Application *app);

/**
 * @brief Runs the main engine update loop.
 *
 * @param app A pointer to the Application struct to run.
 * @return void
 */
ENGINE_API void engine_run(Application *app);

#endif // ENGINE_H
