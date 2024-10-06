#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "defines.h"

typedef u32 Entity;

/**
 * @brief Creates a new entity.
 *
 * @return Entity The new entity.
 */
Entity entity_create(void);

/**
 * @brief Destroys an entity.
 *
 * @param entity The entity to destroy.
 */
void entity_destroy(Entity entity);

#endif // ENGINE_ENTITY_H
