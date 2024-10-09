#include "engine/components/position.h"
#include "engine/components/velocity.h"
#include "engine/core/logging.h"
#include "engine/ecs/ecs.h"

void movement_system_update(ECSManager *ecs, f32 deltaTime) {
    log_info("Updating movement system. ECS: %p, DeltaTime: %f", ecs, deltaTime);
    // ComponentType positionType = /* retrieve Position component type */;
    // ComponentType velocityType = /* retrieve Velocity component type */;

    // ComponentArray *positions = &ecs->componentArrays[positionType];
    // ComponentArray *velocities = &ecs->componentArrays[velocityType];

    // // Iterate over entities that have both Position and Velocity.
    // for (u32 i = 0; i < positions->count; ++i) {
    //     Entity entity = positions->entities[i];
    //     Velocity *velocity = (Velocity *)ecs_get_component(ecs, entity, velocityType);
    //     Position *position = (Position *)ecs_get_component(ecs, entity, positionType);

    //     if (!velocity || !position) {
    //         log_warning("Entity %u is missing Position or Velocity component.", entity);
    //         continue;
    //     }

    //     // Update the position based on the velocity.
    //     position->x += velocity->vx * deltaTime;
    //     position->y += velocity->vy * deltaTime;
    //     position->z += velocity->vz * deltaTime;
    // }
}
