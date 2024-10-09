#include "engine/ecs/entity.h"

#include "engine/core/logging.h"

static Entity next_entity_id = 1;

Entity entity_create(void) {
    return next_entity_id++;
}

void entity_destroy(Entity entity) {
    log_debug("Entity %u destroyed.", entity);
    // TODO: Implement entity destruction.
}
