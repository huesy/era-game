#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "components.h"
#include "entity.h"
#include "entity_manager.h"

#define MAX_COMPONENTS MAX_ENTITIES

typedef struct {
  PositionComponent positions[MAX_COMPONENTS];
  VelocityComponent velocities[MAX_COMPONENTS];
  RenderableComponent renderables[MAX_COMPONENTS];
  HealthComponent health[MAX_COMPONENTS];
  DamageComponent damage[MAX_COMPONENTS];
  SpeedComponent speed[MAX_COMPONENTS];
} ComponentManager;

// ============================================================================
// Components Manager

/// @brief Initialize the component manager.
/// @param manager The component manager to initialize.
/// @note This function must be called before using the component manager.
void component_manager_init(ComponentManager *manager);

/// @brief Destroy the component manager.
/// @param manager The component manager to destroy.
void component_manager_destroy(ComponentManager *manager);

// ============================================================================
// Add Components

/// @brief Add a position component to an entity.
/// @param manager The component manager to add the component with.
/// @param entity The entity to add the component to.
/// @param component The position component to add.
void component_manager_add_position(ComponentManager *manager, Entity entity, PositionComponent position);

/// @brief Add a velocity component to an entity.
/// @param manager The component manager to add the component with.
/// @param entity The entity to add the component to.
/// @param velocity The velocity component to add.
void component_manager_add_velocity(ComponentManager *manager, Entity entity, VelocityComponent velocity);

/// @brief Add a renderable component to an entity.
/// @param manager The component manager to add the component with.
/// @param entity The entity to add the component to.
/// @param renderable The renderable component to add.
void component_manager_add_renderable(ComponentManager *manager, Entity entity, RenderableComponent renderable);

/// @brief Add a health component to an entity.
/// @param manager The component manager to add the component with.
/// @param entity The entity to add the component to.
/// @param health The health component to add.
void component_manager_add_health(ComponentManager *manager, Entity entity, HealthComponent health);

/// @brief Add a damage component to an entity.
/// @param manager The component manager to add the component with.
/// @param entity The entity to add the component to.
/// @param damage The damage component to add.
void component_manager_add_damage(ComponentManager *manager, Entity entity, DamageComponent damage);

/// @brief Add a speed component to an entity.
/// @param manager The component manager to add the component with.
/// @param entity The entity to add the component to.
/// @param speed The speed component to add.
void component_manager_add_speed(ComponentManager *manager, Entity entity, SpeedComponent speed);

// ============================================================================
// Get Components

/// @brief Get the position component of an entity.
/// @param manager The component manager to get the component from.
/// @param entity The entity to get the component from.
/// @return The position component of the entity.
PositionComponent* component_manager_get_position(ComponentManager *manager, Entity entity);

/// @brief Get the velocity component of an entity.
/// @param manager The component manager to get the component from.
/// @param entity The entity to get the component from.
/// @return The velocity component of the entity.
VelocityComponent* component_manager_get_velocity(ComponentManager *manager, Entity entity);

/// @brief Get the renderable component of an entity.
/// @param manager The component manager to get the component from.
/// @param entity The entity to get the component from.
/// @return The renderable component of the entity.
RenderableComponent* component_manager_get_renderable(ComponentManager *manager, Entity entity);

/// @brief Get the health component of an entity.
/// @param manager The component manager to get the component from.
/// @param entity The entity to get the component from.
/// @return The health component of the entity.
HealthComponent* component_manager_get_health(ComponentManager *manager, Entity entity);

/// @brief Get the damage component of an entity.
/// @param manager The component manager to get the component from.
/// @param entity The entity to get the component from.
/// @return The damage component of the entity.
DamageComponent* component_manager_get_damage(ComponentManager *manager, Entity entity);

/// @brief Get the speed component of an entity.
/// @param manager The component manager to get the component from.
/// @param entity The entity to get the component from.
/// @return The speed component of the entity.
SpeedComponent* component_manager_get_speed(ComponentManager *manager, Entity entity);

// ============================================================================
// Remove Components

/// @brief Remove the position component of an entity.
/// @param manager The component manager to remove the component from.
/// @param entity The entity to remove the component from.
void component_manager_remove_position(ComponentManager *manager, Entity entity);

/// @brief Remove the velocity component of an entity.
/// @param manager The component manager to remove the component from.
/// @param entity The entity to remove the component from.
void component_manager_remove_velocity(ComponentManager *manager, Entity entity);

/// @brief Remove the renderable component of an entity.
/// @param manager The component manager to remove the component from.
/// @param entity The entity to remove the component from.
void component_manager_remove_renderable(ComponentManager *manager, Entity entity);

/// @brief Remove the health component of an entity.
/// @param manager The component manager to remove the component from.
/// @param entity The entity to remove the component from.
void component_manager_remove_health(ComponentManager *manager, Entity entity);

/// @brief Remove the damage component of an entity.
/// @param manager The component manager to remove the component from.
/// @param entity The entity to remove the component from.
void component_manager_remove_damage(ComponentManager *manager, Entity entity);

/// @brief Remove the speed component of an entity.
/// @param manager The component manager to remove the component from.
/// @param entity The entity to remove the component from.
void component_manager_remove_speed(ComponentManager *manager, Entity entity);

#endif // COMPONENT_MANAGER_H
