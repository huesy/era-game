/**
 * @file editor.h
 * @author Andrew Hughes (a.hughes@gmail.com)
 * @brief Editor specific functionalities.
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Era Engine is Copyright (c) Andrew Hughes 2024
 */

#ifndef EDITOR_H
#define EDITOR_H

#include "engine/defines.h"

/**
 * @brief Initializes the editor.
 *
 * @return EngineResult ENGINE_SUCCESS if the editor was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult editor_init(void);

/**
 * @brief Shuts down the editor and cleans up resources.
 */
ENGINE_API void editor_shutdown(void);

/**
 * @brief Updates the editor.
 *
 * @param deltaTime
 */
ENGINE_API void editor_update(f32 deltaTime);

/**
 * @brief Renders the editor.
 */
ENGINE_API void editor_render(void);

// Add other editor-specific functions here.
// For example:
// ENGINE_API void editor_load_scene(const char *path);
// ENGINE_API void editor_save_scene(const char *path);
// ENGINE_API void editor_select_entity(u32 id);
// ENGINE_API void editor_deselect_entity(u32 id);
// ENGINE_API void editor_move_entity(u32 id, f32 x, f32 y, f32 z);
// ENGINE_API void editor_rotate_entity(u32 id, f32 x, f32 y, f32 z);
// ENGINE_API void editor_scale_entity(u32 id, f32 x, f32 y, f32 z);
// ...

#endif // EDITOR_H
