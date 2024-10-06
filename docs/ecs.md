# ECS (Entity Component System)

## Overview

The Entity Component System (ECS) is the code architectural pattern used for organising game objects in the engine. The ECS allows for flexible, reusable, and scalable game objects by decoupling data (components) from behaviour (systems).

## Key Concepts

### Entity

An identifier for a game object. Entities are simple, unique IDs that represent objects in the game world but do not contain any data or behaviour themselves.

### Component

Data that is attached to an entity. Components represent individual aspects of an entity, such as position, velocity, or health.

### System

A function or set of functions that operate on entities with specific components. Systems contain the logic and behaviour for entities, making ECS highly scalable.

## How to Register a New Component

To register a new component type, use the `component_register()` function and provide the size of the component. For example: This registers a new component type and returns a unique identifier for that type, which is then used when adding or retrieving the component from an entity. Once registered, this component type can be added to any entity.

## Adding and Removing Components

### Adding a Component

To add a component to an entity, use `component_add()`. You must provide the entity ID, component type, and the data to initialise the component.

### Removing a Component

To remove a component from an entity, use `component_remove()`.

## Getting a Component

To retrieve a pointer to a component on an entity, use `component_get()`.

## Extending the ECS

To extend the ECS, simply define new component structures, register them using `component_register()`, and add/remove them from entities as needed. Systems can then iterate over entities with specific components to implement game logic.
