# Physics System

## Overview

The Physics System is responsible for simulating physical interactions between entities, including movement, collisions, gravity, and other forces. It uses a simple 2D physics engine suitable for games with basic physical interactions.

## Key Concepts

### Rigidbodies

The physics system uses `Rigidbody` components to track the physical state of entities, including position, velocity, and mass.

### Collision Detection

The system includes basic collision detection for axis-aligned bounding boxes (AABB), which can be extended to support more complex shapes.

## Adding a Rigidbody Component

To make an entity subject to physics, add a `Rigidbody` component.

## Extending the Physics System

To extend the physics system, you can add more sophisticated collision detection, such as support for circular or polygonal shapes. You can also implement additional forces, such as drag or springs.
