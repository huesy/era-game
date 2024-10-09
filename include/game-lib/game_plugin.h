/**
 * @file game_plugin.h
 * @author Andrew Hughes (a.hughes@gmail.com)
 * @brief Game plugin interface.
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Era Engine Copyright (c) Andrew Hughes 2024
 *
 */

#ifndef GAME_PLUGIN_H
#define GAME_PLUGIN_H

#include "engine/defines.h"

typedef struct EditorApplication {
    b8 isRunning;
    f32 lastTime;
} EditorApplication;

/**
 * @brief Loads the game plugin.
 */
void game_plugin_load(void);

#endif // GAME_PLUGIN_H
