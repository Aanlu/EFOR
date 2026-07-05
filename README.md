# EFOR (File Explorer)

A lightweight, modular terminal file explorer written in C. Designed with a strict focus on system architecture, platform abstraction, and memory safety, avoiding external library dependencies.

## Core Architecture

* **Platform-Agnostic Core:** Decoupled application logic from OS-specific file system APIs (POSIX/Win32) via abstract interfaces.
* **Centralized State Management:** Strict runtime controls for configuration, themes, and UI events.
* **Custom Parsers:** Memory-safe, hand-written text parsers for initialization (.init) and configuration (.cfg) files.
* **Dynamic Customization:** Support for hot-swapping localization profiles and UI color schemes.

## Project Structure

include/          # Public API  and component interfaces (.h)
├── core/         # State management and application lifecycle
├── parsers/      # Text parsing definitions
└── platform/     # OS abstraction layers
src/              # Implementation details (.c)
├── core/         # Internal logic and event buffers
├── parsers/      # Parsing algorithms and data structures
├── platform/     # Native implementations (Linux POSIX / Windows Win32)
└── main.c        # Application entry point
config/           # User configuration profiles and localization data

## Roadmap (Early Stages)

1. **Core Data Structures & Parsers:** Implementation of safe string manipulation, hash maps, and the baseline text parser for configuration files.
2. **Platform Abstraction Layer:** Defining the generic file system interface and implementing the POSIX (Linux/WSL) backend for directory reading.
3. **Event-Driven Input:** Creating non-blocking circular buffers for keyboard events and state synchronization.

## Build Instructions

* **Build System:** CMake (3.10+)
* **Compiler Flags:** -Wall -Wextra -Werror