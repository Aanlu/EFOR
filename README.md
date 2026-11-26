# EFOR (File Explorer)

![License](https://img.shields.io/github/license/Aanlu/EFOR)
![Language](https://img.shields.io/github/languages/top/Aanlu/EFOR)
![Last Commit](https://img.shields.io/github/last-commit/Aanlu/EFOR)

> **Status:** Interactive Alpha. The event-driven core, Win32 input backend,
> command engine and file-based logging are implemented. The UI is still
> static: dynamic resizing, scroll, footer feedback and command history remain
> open work in [issue #6](https://github.com/Aanlu/EFOR/issues/6).

A lightweight, modular terminal file explorer written in C. Designed with a
strict focus on system architecture, platform abstraction, and memory safety,
avoiding external library dependencies.

## Implemented features

- Directory listing through the Win32 backend (hidden, system and reparse-point
  entries are skipped)
- Navigation: arrow keys to move, LEFT to go back, RIGHT/ENTER to enter a
  directory, ESC to exit
- Commands: press `:` to open the command line, then use:
  - `:mkdir <name>` — create a directory
  - `:touch <name>` — create a file
  - `:delete <name>` — delete a file or directory (asks for `y`/`n` confirmation)
  - `:rename <old> <new>` — rename an item (asks for `y`/`n` confirmation)
  - `:cd <path>` — change directory
- Blocking, event-driven console input with no busy waiting
- Input failures and early exits restore the original console mode
- Timestamped, append-mode logging to `debug.log`
- 7 automated tests covering paths, logger, parser, app state, process
  lifecycle, input errors and redirected stdin

## Core Architecture

* **Platform-Agnostic Core:** Decoupled application logic and event handling from OS-specific file system APIs (POSIX/Win32) via abstract interfaces.
* **Centralized State Management:** Strict runtime controls for configuration, themes, and UI events via `app_controller` and `app_state`.
* **Custom Parsers:** Memory-safe, hand-written parser for command input with argument validation.
* **Low-Latency Event Engine:** Blocking `ReadConsoleInput` loop with controlled screen clearing to prevent terminal flickering and input lag.
* **File-Based Logging:** Timestamped append-only diagnostics written to `debug.log`.

## Project Structure

    include/          # Public API and component interfaces (.h)
    ├── core/         # State management, lifecycle, and main controller
    │   └── interfaces/  # Input and filesystem abstractions
    ├── parsers/      # Command parsing definitions
    src/              # Implementation details (.c)
    ├── core/         # Internal logic and event delegation
    ├── parsers/      # Command parsing algorithms
    ├── platform/     # Native implementations (currently Win32)
    │   └── windows/
    └── main.c        # Application entry point and initializer
    tests/            # Unit and integration tests

## Build Instructions

**Requirements:**

* CMake 3.10+
* C99 compiler (MSVC via Visual Studio in the current Windows environment)

**Build and test:**

```sh
cmake -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug
```

Run the application on Windows:

```sh
.\build\Debug\efor.exe
```

The `--config Debug` flag is needed with multi-config generators such as
Visual Studio. With single-config generators (for example Ninja), use
`cmake --build build` instead.

## Roadmap

Work for the v0.3.0 milestone is tracked in
[issue #6](https://github.com/Aanlu/EFOR/issues/6). Remaining items include a
dynamic, size-aware UI with scroll, a footer that renders status messages and
confirmation prompts, command history, CI on `windows-latest`, an English UI,
`CHANGELOG.md`, and the version bump to 0.3.0.

## Contributing

This project is in an early stage. Contributions are welcome: open an issue
first to agree on the scope, then send a focused pull request that references
it. See the roadmap above for the current work in progress.

## License

This project is licensed under the [MIT License](./LICENSE).
