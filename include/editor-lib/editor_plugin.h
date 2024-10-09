/**
 * @file editor_plugin.h
 * @author Andrew Hughes (a.hughes@gmail.com)
 * @brief Editor plugin interface.
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Era Engine Copyright (c) Andrew Hughes 2024
 *
 */

#ifndef EDITOR_PLUGIN_H
#define EDITOR_PLUGIN_H

#include "engine/defines.h"

typedef struct EditorApplication {
    b8 isRunning;
    f32 lastTime;
} EditorApplication;

/**
 * @brief Loads the editor plugin.
 */
void editor_plugin_load(void);

#endif // EDITOR_PLUGIN_H
