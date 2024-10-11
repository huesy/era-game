/**
 * @file game.h
 * @author Andrew Hughes (a.hughes@gmail.com)
 * @brief Game specific functionalities.
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Era Engine is Copyright (c) Andrew Hughes 2024
 */

#ifndef GAME_H
#define GAME_H

#include "engine/defines.h"

typedef struct GameState {
    b8 isRunning;
} GameState;

/**
 * @brief Initializes the game.
 *
 * @return b8 True if the game was initialized successfully, otherwise false.
 */
ENGINE_API b8 game_init(void);

/**
 * @brief Shuts down the game and cleans up resources.
 */
ENGINE_API void game_shutdown(void);

/**
 * @brief Updates the game.
 *
 * @param deltaTime
 */
ENGINE_API void game_update(f32 deltaTime);

/**
 * @brief Renders the game.
 */
ENGINE_API void game_render(void);

// Add other game-specific functions here.
// For example:
// ENGINE_API void game_load_scene(const char *path);
// ENGINE_API void game_save_scene(const char *path);
// ...

#endif // GAME_H
