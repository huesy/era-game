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

typedef struct EngineConfig {
    u64 memoryPoolSize;
} EngineConfig;

/**
 * @brief Initializes the engine.
 *
 * @param config Engine configuration.
 * @return ENGINE_SUCCESS if the engine was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult engine_init(EngineConfig *config);

/**
 * @brief Shuts down the engine and cleans up resources.
 */
ENGINE_API void engine_shutdown(void);

/**
 * @brief Runs the main engine update loop.
 */
ENGINE_API void engine_run(void);

#endif // ENGINE_H
