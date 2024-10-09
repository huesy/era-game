#include "engine/ecs/ecs.h"
#include "engine/logging.h"
#include "engine/memory.h"

void ecs_init(ECSManager *ecs) {
    ecs->entityManager.nextEntity = 1; // Start entities from 1.
    ecs->entityManager.freeCount = 0;

    // Initialise all ComponentArrays
    for (u32 i = 0; i < MAX_COMPONENTS; ++i) {
        ecs->componentArrays[i].count = 0;
        ecs->componentArrays[i].size = 0;
        for (u32 j = 0; j < MAX_ENTITIES; ++j) {
            ecs->componentArrays[i].sparse[j] = INVALID_ID_U32;
            ecs->componentArrays[i].components[j] = NULL;
            ecs->componentArrays[i].entities[j] = INVALID_ENTITY;
        }
    }

    ecs->registeredComponents = 0;
    log_info("ECS initialized");
}

void ecs_shutdown(ECSManager *ecs) {
    // Free all component data.
    for (ComponentType type = 0; type < ecs->registeredComponents; ++type) {
        ComponentArray *componentArray = &ecs->componentArrays[type];
        for (u32 i = 0; i < componentArray->count; ++i) {
            memory_free(componentArray->components[i], componentArray->size, MEMORY_TAG_COMPONENT);
        }
        componentArray->count = 0;
        componentArray->size = 0;

        // Reset sparse and dense arrays.
        for (u32 j = 0; j < MAX_ENTITIES; ++j) {
            componentArray->sparse[j] = INVALID_ID_U32;
            componentArray->entities[j] = INVALID_ENTITY;
            componentArray->components[j] = NULL;
        }
    }

    // Reset entity manager.
    ecs->entityManager.nextEntity = 1;
    ecs->entityManager.freeCount = 0;

    // Reset component registration.
    ecs->registeredComponents = 0;

    log_info("ECS shutdown");
}

Entity ecs_create_entity(ECSManager *ecs) {
    if (ecs->entityManager.freeCount > 0) {
        // Reuse a free entity ID.
        return ecs->entityManager.freeEntities[--ecs->entityManager.freeCount];
    }

    if (ecs->entityManager.nextEntity >= MAX_ENTITIES) {
        log_error("Maximum number of entities reached.");
        return INVALID_ENTITY;
    }

    return ecs->entityManager.nextEntity++;
}

void ecs_destroy_entity(ECSManager *ecs, Entity entity) {

    if (entity == INVALID_ENTITY || entity >= ecs->entityManager.nextEntity) {
        log_warning("Attemped to destroy invalid entity: %u", entity);
        return;
    }

    // Remove all components associated with the entity.
    for (ComponentType type = 0; type < ecs->registeredComponents; ++type) {
        ecs_remove_component(ecs, entity, type);
    }

    // Add entity to the free list.
    ecs->entityManager.freeEntities[ecs->entityManager.freeCount++] = entity;
}

ComponentType ecs_register_component(ECSManager *ecs, u32 componentSize) {
    if (ecs->registeredComponents >= MAX_COMPONENTS) {
        log_error("Maximum number of components reached.");
        return INVALID_COMPONENT_TYPE; // Clearly indicates an invalid component type.
    }

    // Initialize component array.
    u32 index = ecs->registeredComponents;
    ComponentArray *componentArray = &ecs->componentArrays[index];
    componentArray->count = 0;
    componentArray->size = componentSize;

    // Initialise sparse array to invalid entity IDs.
    for (u32 i = 0; i < MAX_ENTITIES; ++i) {
        componentArray->sparse[i] = INVALID_ID_U32;
        componentArray->entities[i] = INVALID_ENTITY;
        componentArray->components[i] = NULL;
    }

    ecs->registeredComponents++;
    log_info("Registered component type %u with size %u", index, componentSize);
    return index;
}

void ecs_add_component(ECSManager *ecs, Entity entity, ComponentType type, void *componentData) {
    if (type == INVALID_COMPONENT_TYPE || type >= ecs->registeredComponents) {
        log_error("Component type %u is not registered.", type);
        return;
    }

    if (entity == INVALID_ENTITY || entity >= ecs->entityManager.nextEntity) {
        log_error("Invalid entity ID: %u", entity);
        return;
    }

    ComponentArray *componentArray = &ecs->componentArrays[type];

    // Check if entity already has the component.
    if (componentArray->sparse[entity] != INVALID_ID_U32) {
        log_warning("Entity %u already has component type %u.", entity, type);
        return;
    }

    if (componentArray->count >= MAX_ENTITIES) {
        log_error("Maximum number of components reached for type %u.", type);
        return;
    }

    // Allocate and copy component data to the component array.
    void *data = memory_allocate(componentArray->size, MEMORY_TAG_COMPONENT);
    if (!data) {
        log_error("Failed to allocate memory for component type %u.", type);
        return;
    }

    memory_copy(data, componentData, componentArray->size);

    // Add to dense array.
    componentArray->entities[componentArray->count] = entity;
    componentArray->components[componentArray->count] = data;

    // Update sparse array.
    componentArray->sparse[entity] = componentArray->count;

    componentArray->count++;
}

void ecs_remove_component(ECSManager *ecs, Entity entity, ComponentType type) {
    if (type == INVALID_COMPONENT_TYPE || type >= ecs->registeredComponents) {
        log_error("Component type %u is not registered.", type);
        return;
    }

    if (entity == INVALID_ENTITY || entity >= ecs->entityManager.nextEntity) {
        log_error("Invalid entity ID: %u", entity);
        return;
    }

    ComponentArray *componentArray = &ecs->componentArrays[type];
    u32 index = componentArray->sparse[entity];

    if (index == INVALID_ID_U32) {
        log_warning("Entity %u does not have component type %u.", entity, type);
        return;
    }

    // Free component memory.
    memory_free(componentArray->components[index], componentArray->size, MEMORY_TAG_COMPONENT);

    // Swap with the last component in the dense array.
    u32 lastIndex = componentArray->count - 1;
    Entity lastEntity = componentArray->entities[lastIndex];
    void *lastComponent = componentArray->components[lastIndex];

    componentArray->entities[index] = lastEntity;
    componentArray->components[index] = lastComponent;

    // Update sparse array for the moved entity.
    componentArray->sparse[lastEntity] = index;

    // Mark the removed entity as invalid.
    componentArray->sparse[entity] = INVALID_ID_U32;
    componentArray->entities[lastIndex] = INVALID_ENTITY;
    componentArray->components[lastIndex] = NULL;

    componentArray->count--;

    log_debug("Removed component type %u from entity %u.", type, entity);
}

void *ecs_get_component(ECSManager *ecs, Entity entity, ComponentType type) {
    if (type == INVALID_COMPONENT_TYPE || type >= ecs->registeredComponents) {
        log_error("Component type %u is not registered.", type);
        return NULL;
    }

    if (entity == INVALID_ENTITY || entity >= ecs->entityManager.nextEntity) {
        log_error("Invalid entity ID: %u", entity);
        return NULL;
    }

    ComponentArray *componentArray = &ecs->componentArrays[type];
    u32 index = componentArray->sparse[entity];

    if (index == INVALID_ID_U32) {
        log_warning("Entity %u does not have component type %u.", entity, type);
        return NULL;
    }

    return componentArray->components[index];
}
