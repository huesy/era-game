#ifndef ENGINE_SCENE_MANAGER_H
#define ENGINE_SCENE_MANAGER_H

/**
 * @brief Initializes the scene manager.
 */
void scene_manager_load_scene(const char *scene_name);

/**
 * @brief Shuts down the scene manager.
 */
void scene_manager_unload_scene(void);

#endif // ENGINE_SCENE_MANAGER_H
