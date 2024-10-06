#ifndef ENGINE_ECS_H
#define ENGINE_ECS_H

#include "defines.h"

// Maximum limits
#define MAX_ENTITIES 4096
#define MAX_COMPONENTS 256
#define MAX_SYSTEMS 128

typedef u32 Entity;
typedef u32 ComponentType;

// Entity Manager
typedef struct {
    Entity nextEntity;
    Entity freeEntities[MAX_ENTITIES];
    u32 freeCount;
} EntityManager;

// Sparse Set for components.
typedef struct {
    Entity entities[MAX_ENTITIES];
    void *components[MAX_ENTITIES];
    u32 count;
} ComponentArray;

// ECS Manager
typedef struct {
    EntityManager entityManager;
    ComponentArray componentArrays[MAX_COMPONENTS];
    u32 registeredComponents;
} ECSManager;

/**
 * @brief Initializes the ECS system.
 *
 * @param ecs Pointer to the ECS manager to initialize.
 */
void ecs_init(ECSManager *ecs);

/**
 * @brief Shuts down the ECS system, cleaning up all resources.
 *
 * @param ecs Pointer to the ECS manager to shut down.
 */
void ecs_shutdown(ECSManager *ecs);

/**
 * @brief Creates a new entity.
 *
 * @param ecs Pointer to the ECS manager to create the entity with.
 * @return Entity The newly created entity.
 */
Entity ecs_create_entity(ECSManager *ecs);

/**
 * @brief Destroys an existing entity.
 *
 * @param ecs Pointer to the ECS manager to destroy the entity with.
 * @param entity The entity to destroy.
 */
void ecs_destroy_entity(ECSManager *ecs, Entity entity);

/**
 * @brief Registers a new component type with the ECS.
 *
 * @param ecs Pointer to the ECS manager to register the component with.
 * @param componentSize The size of the component in bytes.
 * @return ComponentType The unique identifier for the component type.
 */
ComponentType ecs_register_component(ECSManager *ecs, u32 componentSize);

/**
 * @brief Adds a component to an entity.
 *
 * @param ecs Pointer to the ECS manager to add the component with.
 * @param entity The entity to add the component to.
 * @param type The type of component to add.
 * @param componentData Pointer to the component data.
 */
void ecs_add_component(ECSManager *ecs, Entity entity, ComponentType type, void *componentData);

/**
 * @brief Removes a component from an entity.
 *
 * @param ecs Pointer to the ECS manager to remove the component with.
 * @param entity The entity to remove the component from.
 * @param type The type of component to remove.
 */
void ecs_remove_component(ECSManager *ecs, Entity entity, ComponentType type);

/**
 * @brief Retrieves a component from an entity.
 *
 * @param ecs Pointer to the ECS manager to retrieve the component from.
 * @param entity The entity to retrieve the component from.
 * @param type The type of component to retrieve.
 * @return void* Pointer to the component data, or NULL if not found.
 */
void *ecs_get_component(ECSManager *ecs, Entity entity, ComponentType type);

#endif // ENGINE_ECS_H
