#include "entity_manager.h"

void entity_manager_init(EntityManager *manager) { manager->nextId = 0; }

Entity entity_manager_create_entity(EntityManager *manager)
{
    if (manager->nextId >= MAX_ENTITIES)
    {
        // TODO: Handle error: Maximum entities reached.
        Entity invalid = {0};
        invalid.id = (unsigned int)-1;
        return invalid;
    }

    Entity entity = {0};
    entity.id = manager->nextId;
    manager->entities[manager->nextId++] = entity;
    return entity;
}

void entity_manager_destroy_entity(EntityManager *manager, Entity entity)
{
    if (entity.id >= MAX_ENTITIES)
    {
        // TODO: Handle error: Invalid entity.
        return;
    }

    manager->entities[entity.id].id = (unsigned int)-1;
}