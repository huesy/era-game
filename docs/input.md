# Input System

## Overview

The Input System handles user input from the keyboard, mouse, and potentially gamepads. It abstracts platform-specific input handling, providing a unified interface for the rest of the engine.

## Key Concepts

### Platform-Specific APIs

The input system uses different APIs for different platforms (e.g., Windows, macOS, Linux) to detect key presses and mouse movement.

### Input Mapping

The input system allows mapping of physical keys to in-game actions, making it easy to support configurable controls.

## Adding Platform-Specific Input

To add input support for a new platform, create an `input_<platform>.c` file and implement the required functions (`initialize`, `shutdown`, `update`, and `keyPressed`). Set the API using `input_set_api()`.
