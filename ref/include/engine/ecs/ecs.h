#ifndef ENGINE_ECS_H
#define ENGINE_ECS_H

#include "engine/defines.h"

#define INVALID_ENTITY MAX_U32
#define INVALID_COMPONENT_TYPE MAX_U32

// Maximum limits
#define MAX_ENTITIES 4096
#define MAX_COMPONENTS 256
#define MAX_SYSTEMS 128

typedef u32 Entity;
typedef u32 ComponentType;

/** @brief Entity manager structure. */
typedef struct EntityManager {
    /** @brief The next entity ID to assign. */
    Entity nextEntity;
    /** @brief List of entity IDs that were previously destroyed and can be reused. */
    Entity freeEntities[MAX_ENTITIES];
    /** @brief The number of previously destroyed entities that can be reused. */
    u32 freeCount;
} EntityManager;

/** @brief Component array structure. */
typedef struct ComponentArray {
    /** @brief Dense array of entities with this component. */
    Entity entities[MAX_ENTITIES];
    /** @brief Dense array of component data. */
    void *components[MAX_ENTITIES];
    /** @brief The number of entities that have this component. */
    u32 count;
    /** @brief The size of the component in bytes. */
    u32 size;
    /** @brief Sparse array that maps Entity IDs to dense array indices. */
    u32 sparse[MAX_ENTITIES];
} ComponentArray;

/** @brief ECS manager structure. */
typedef struct ECSManager {
    /** @brief The entity manager. */
    EntityManager entityManager;
    /** @brief Array of components. */
    ComponentArray componentArrays[MAX_COMPONENTS];
    /** @brief The number of registered component types. */
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
