# Rendering System

## Overview

The Rendering System is reponsible for drawing all visual elements in the game, such as sprites, backgrounds, and UI components. It is designed to be extendable and can support multiple rendering backends, starting with OpenGL, and potentially adding Vulkan in the future.

## Key Concepts

### Renderer API Abstraction

The rendering system is abstracted to allow different rendering backends. The current implementation uses an OpenGL backend, but the structure is designed to support switching to Vulkan or other APIs with minimal changes.

### Drawing Sprites

The rendering system provides functions to draw 2D sprites, handle transparency, and apply basic transformations like translation, rotation, and scaling.

## Setting Up the Renderer

### Initialisation

Call `renderer_initialize()` to set up the rendering context. The specific backend (e.g., OpenGL) will be initialised based on the platform.

### Drawing

Use `renderer_draw()` to render all elements in the current frame.

### Shutdown

Once rendering is no longer needed, use `renderer_shutdown()` to clean up resources.

## Extending the Rendering System

To extend the rendering system, add new drawing functions or shaders to support additional visual effects. For example, you could add support for particle systems or post-processing effects like bloom or motion blur.
