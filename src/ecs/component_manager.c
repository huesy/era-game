#include "component_manager.h"

// ============================================================================
// Components Manager

void component_manager_init(ComponentManager *manager)
{
  memset(manager, 0, sizeof(ComponentManager));
}

void component_manager_destroy(ComponentManager *manager)
{
  memset(manager, 0, sizeof(ComponentManager));
}

// ============================================================================
// Add Components

void component_manager_add_position(
    ComponentManager *manager,
    Entity entity,
    PositionComponent position
)
{
  manager->positions[entity.id] = position;
}

void component_manager_add_velocity(
    ComponentManager *manager,
    Entity entity,
    VelocityComponent velocity
)
{
  manager->velocities[entity.id] = velocity;
}

void component_manager_add_renderable(
    ComponentManager *manager,
    Entity entity,
    RenderableComponent renderable
)
{
  manager->renderables[entity.id] = renderable;
}

void component_manager_add_health(
    ComponentManager *manager,
    Entity entity,
    HealthComponent health
)
{
  manager->health[entity.id] = health;
}

void component_manager_add_damage(
    ComponentManager *manager,
    Entity entity,
    DamageComponent damage
)
{
  manager->damage[entity.id] = damage;
}

void component_manager_add_speed(
    ComponentManager *manager,
    Entity entity,
    SpeedComponent speed
)
{
  manager->speed[entity.id] = speed;
}

// ============================================================================
// Get Components

PositionComponent *component_manager_get_position(
    ComponentManager *manager,
    Entity entity
)
{
  return &manager->positions[entity.id];
}

VelocityComponent *component_manager_get_velocity(
    ComponentManager *manager,
    Entity entity
);

RenderableComponent *component_manager_get_renderable(
    ComponentManager *manager,
    Entity entity
);

HealthComponent *component_manager_get_health(
    ComponentManager *manager,
    Entity entity
);

DamageComponent *component_manager_get_damage(
    ComponentManager *manager,
    Entity entity
);

SpeedComponent *component_manager_get_speed(
    ComponentManager *manager,
    Entity entity
);

// ============================================================================
// Remove Components

void component_manager_remove_position(
    ComponentManager *manager,
    Entity entity
);

void component_manager_remove_velocity(
    ComponentManager *manager,
    Entity entity
);

void component_manager_remove_renderable(
    ComponentManager *manager,
    Entity entity
);

void component_manager_remove_health(ComponentManager *manager, Entity entity);

void component_manager_remove_damage(ComponentManager *manager, Entity entity);

void component_manager_remove_speed(ComponentManager *manager, Entity entity);
