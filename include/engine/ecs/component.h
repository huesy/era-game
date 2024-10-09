#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

#include "engine/defines.h"
#include "engine/ecs/entity.h"

typedef u32 ComponentType;

/**
 * @brief Registers a new component type.
 *
 * @param size The size of the new component in bytes.
 * @return The type of the new component.
 */
ComponentType component_register(u32 size);

/**
 * @brief Adds a component to an entity.
 *
 * @param entity The entity to add the component to.
 * @param type The component type to add.
 * @param data Pointer to the component data.
 */
void component_add(Entity entity, ComponentType type, void *data);

/**
 * @brief Removes a component from an entity
 *
 * @param entity The entity to remove the component from.
 * @param type The component type to remove.
 */
void component_remove(Entity entity, ComponentType type);

/**
 * @brief Gets a pointer to a component on an entity.
 *
 * @param entity The entity to get the component from.
 * @param type The component type to get.
 * @return Pointer to the component data.
 */
void *component_get(Entity entity, ComponentType type);

#endif // ENGINE_COMPONENT_H
