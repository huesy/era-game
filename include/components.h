#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef struct {
  float x, y;
} PositionComponent;

typedef struct {
  float vx, vy;
} VelocityComponent;

typedef struct {
  // Placeholder for sprite data
  // TODO: Replace with actual sprite structure.
  char spritePath[256];
} RenderableComponent;

typedef struct {
  int currentHealth;
  int maxHealth;
} HealthComponent;

typedef struct {
  int damage;
} DamageComponent;

typedef struct {
  int speed;
} SpeedComponent;

#endif // COMPONENTS_H
