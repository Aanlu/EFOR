# EFOR (File Explorer)

![License](https://img.shields.io/github/license/Aanlu/EFOR)
![Language](https://img.shields.io/github/languages/top/Aanlu/EFOR)
![Last Commit](https://img.shields.io/github/last-commit/Aanlu/EFOR)

> 🚀 **Status:** Interactive Alpha phase. The core architecture is defined, and the basic TUI (Text User Interface) rendering engine is functional. Directory reading and UI navigation via the Windows backend are operational.

A lightweight, modular terminal file explorer written in C. Designed with a strict focus on system architecture, platform abstraction, and memory safety, avoiding external library dependencies.

## Core Architecture

* **Platform-Agnostic Core:** Decoupled application logic and event handling from OS-specific file system APIs (POSIX/Win32) via abstract interfaces.
* **Centralized State Management:** Strict runtime controls for configuration, themes, and UI events via `app_controller` and `app_state`.
* **Custom Parsers (WIP):** Memory-safe, hand-written text parsers for initialization (.init) and configuration (.cfg) files.
* **Low-Latency Event Engine:** Input capture loop with controlled screen clearing to prevent terminal flickering and input lag.

## Project Structure

    include/          # Public API and component interfaces (.h)
    ├── core/         # State management, lifecycle, and main controller
    ├── parsers/      # Text parsing definitions
    └── platform/     # OS abstraction layers (Input & File System)
    src/              # Implementation details (.c)
    ├── core/         # Internal logic and event delegation
    ├── parsers/      # Configuration parsing algorithms
    ├── platform/     # Native implementations (Currently Win32 WinAPI)
    └── main.c        # Application entry point and initializer
    config/           # User configuration profiles and localization data

## Build Instructions

**Requirements:**
* CMake (3.10+)
* C Compiler (MSVC for the current Windows environment, extensible to GCC/Clang)

**Build Commands (Terminal):**

1. Configure the build directory:
    ```bash
    cmake -B build
    ```
2. Build the project:
    ```bash
    cmake --build build
    ```
3. Run the application (Windows):
    ```bash
    .\build\Debug\efor.exe
    ```
    *(Note: The exact path to the executable may vary depending on your local CMake generator).*

## Contributing

This project is in an early stage. See [CONTRIBUTING.md](./CONTRIBUTING.md) for details on the current state and how to get involved once the architecture stabilizes.

## License

This project is licensed under the [MIT License](./LICENSE).