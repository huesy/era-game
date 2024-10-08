#include "ecs/component.h"
#include "core/logging.h"
#include "core/memory.h"

#define MAX_COMPONENT_TYPES 256
#define MAX_ENTITIES 1024

typedef struct Component {
    ComponentType type;
    void *data;
} Component;

static u32 registeredComponentCount = 0;
static Component *components[MAX_ENTITIES][MAX_COMPONENT_TYPES];
static u32 componentSizes[MAX_COMPONENT_TYPES];

ComponentType component_register(u32 size) {
    if (registeredComponentCount >= MAX_COMPONENT_TYPES) {
        log_error("Maximum number of component types reached.");
        return 0;
    }

    componentSizes[registeredComponentCount] = size;
    return registeredComponentCount++;
}

void component_add(Entity entity, ComponentType type, void *data) {
    // Check if the component type is registered.
    if (type >= registeredComponentCount) {
        log_error("Component type %u is not registered.", type);
        return;
    }

    // Check if the entity is in range.
    if (entity >= MAX_ENTITIES) {
        log_error("Entity ID %u is out of range.", entity);
    }

    // Check if the entity already has the component.
    if (components[entity][type]) {
        log_error("Entity %u already has component of type %u.", entity, type);
        return;
    }

    // Allocate memory for the component.
    Component *component = (Component *)memory_allocate(sizeof(Component));
    if (!component) {
        log_error("Failed to allocate memory for component data.");
        return;
    }

    component->type = type;
    component->data = memory_allocate(componentSizes[type]);
    if (!component->data) {
        log_error("Failed to allocate memory for component data.");
        memory_free(component);
        return;
    }

    // Copy the component data.
    memory_copy(component->data, data, componentSizes[type]);

    components[entity][type] = component;
}

void component_remove(Entity entity, ComponentType type) {
    if (entity >= MAX_ENTITIES || type >= registeredComponentCount) {
        log_error("Invalid entity or component type.");
        return;
    }

    Component *component = components[entity][type];
    if (component) {
        memory_free(component->data);
        memory_free(component);
        components[entity][type] = NULL;
    }
}

void *component_get(Entity entity, ComponentType type) {
    if (entity >= MAX_ENTITIES || type >= registeredComponentCount) {
        log_error("Invalid entity or component type.");
        return NULL;
    }

    Component *component = components[entity][type];
    if (component) {
        return component->data;
    }

    return NULL;
}
