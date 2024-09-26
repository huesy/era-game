#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entity.h"

#define MAX_ENTITIES 1024

typedef struct {
  Entity entities[MAX_ENTITIES];
  unsigned int nextId;
} EntityManager;

/// @brief Initialize the entity manager.
/// @param manager The entity manager to initialize.
/// @note This function must be called before using the entity manager.
void entity_manager_init(EntityManager *manager);

/// @brief Create a new entity.
/// @param manager The entity manager to create the entity with.
/// @return The newly created entity.
Entity entity_manager_create_entity(EntityManager *manager);

/// @brief Destroy an entity.
/// @param manager The entity manager to destroy the entity with.
/// @param entity The entity to destroy.
void entity_manager_destroy_entity(EntityManager *manager, Entity entity);

#endif // ENTITY_MANAGER_H
