#ifndef ENGINE_SYSTEM_H
#define ENGINE_SYSTEM_H

#include "defines.h"
#include "ecs/entity.h"

/**
 * @brief Function signature for system updates.
 *
 * @param delta_time Time elapsed since the last update.
 */
typedef void (*SystemUpdateFn)(f32 delta_time);

/**
 * @brief Registers a new system in the ECS.
 *
 * @param update_fn The function to call during each system update.
 */
void system_register(SystemUpdateFn update_fn);

/**
 * @brief Updates all registered systems.
 *
 * @param delta_time Time elapsed since the last update.
 */
void system_update(f32 delta_time);

#endif // ENGINE_SYSTEM_H
