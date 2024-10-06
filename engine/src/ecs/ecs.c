#include "ecs/ecs.h"
#include "core/logging.h"
#include "core/memory.h"

void ecs_init(ECSManager *ecs) {
    ecs->entityManager.nextEntity = 1; // Start entities from 1.
    ecs->entityManager.freeCount = 0;
    // Zero out the component arrays.
    memory_zero(ecs->componentArrays, sizeof(ecs->componentArrays));
    ecs->registeredComponents = 0;
    log_info("ECS initialised");
}

void ecs_shutdown(ECSManager *ecs) {
    // Free all component data.
    for (ComponentType type = 0; type < ecs->registeredComponents; ++type) {
        ComponentArray *array = &ecs->componentArrays[type];
        for (u32 i = 0; i < array->count; ++i) {
            memory_free(array->components[i]);
        }
        array->count = 0;
    }

    ecs->entityManager.nextEntity = 1;
    ecs->entityManager.freeCount = 0;
    log_info("ECS shutdown");
}

Entity ecs_create_entity(ECSManager *ecs) {
    if (ecs->entityManager.freeCount > 0) {
        // Reuse a free entity ID.
        return ecs->entityManager.freeEntities[--ecs->entityManager.freeCount];
    }

    if (ecs->entityManager.nextEntity >= MAX_ENTITIES) {
        log_error("Maximum number of entities reached.");
        return 0;
    }

    return ecs->entityManager.nextEntity++;
}

void ecs_destroy_entity(ECSManager *ecs, Entity entity) {
    if (entity == 0 || entity >= ecs->entityManager.nextEntity) {
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
        return 0;
    }

    // Initialize component array.
    ecs->componentArrays[ecs->registeredComponents].count = 0;
    ecs->componentArrays[ecs->registeredComponents].components[0] = NULL;
    ecs->registeredComponents++;
    log_info("Registered component type %u with size %u", ecs->registeredComponents - 1, componentSize);
    return ecs->registeredComponents - 1;
}

void ecs_add_component(ECSManager *ecs, Entity entity, ComponentType type, void *componentData) {
    if (type >= ecs->registeredComponents) {
        log_error("Component type %u is not registered.", type);
        return;
    }

    if (entity == 0 || entity >= ecs->entityManager.nextEntity) {
        log_error("Invalid entity ID: %u", entity);
        return;
    }

    ComponentArray *array = &ecs->componentArrays[type];
    // Check if entity already has the component.
    for (u32 i = 0; i < array->count; ++i) {
        if (array->entities[i] == entity) {
            log_warning("Entity %u already has component type %u.", entity, type);
            return;
        }
    }

    // TODO: Revisit
    // Allocate and copy component data to the array.
    u32 componentSize = sizeof(componentData);
    void *data = memory_allocate(componentSize);
    if (!data) {
        log_error("Failed to allocate memory for component type %u.", type);
        return;
    }

    // Copy component data using memory_copy
    memory_copy(data, componentData, componentSize);

    // Add to component array.
    array->entities[array->count] = entity;
    array->components[array->count] = data;
    array->count++;
}

void ecs_remove_component(ECSManager *ecs, Entity entity, ComponentType type) {
    if (type >= ecs->registeredComponents) {
        log_error("Component type %u is not registered.", type);
        return;
    }

    if (entity == 0 || entity >= ecs->entityManager.nextEntity) {
        log_error("Invalid entity ID: %u", entity);
        return;
    }

    ComponentArray *array = &ecs->componentArrays[type];
    for (u32 i = 0; i < array->count; ++i) {
        if (array->entities[i] == entity) {
            // Freee component memory.
            memory_free(array->components[i]);

            // Swap with the last component.
            array->entities[i] = array->entities[array->count - 1];
            array->components[i] = array->components[array->count - 1];
            array->count--;
            log_debug("Removed component type %u from entity %u.", type, entity);
            return;
        }
    }

    log_warning("Entity %u does not have component type %u.", entity, type);
}

void *ecs_get_component(ECSManager *ecs, Entity entity, ComponentType type) {
    if (type >= ecs->registeredComponents) {
        log_error("Component type %u is not registered.", type);
        return NULL;
    }

    if (entity == 0 || entity >= ecs->entityManager.nextEntity) {
        log_error("Invalid entity ID: %u", entity);
        return NULL;
    }

    ComponentArray *array = &ecs->componentArrays[type];
    for (u32 i = 0; i < array->count; ++i) {
        if (array->entities[i] == entity) {
            return array->components[i];
        }
    }

    return NULL;
}
