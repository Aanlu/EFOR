# EFOR — Event-driven File Explorer

![CI](https://github.com/Aanlu/EFOR/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/github/license/Aanlu/EFOR)
![Language](https://img.shields.io/github/languages/top/Aanlu/EFOR)
![Last Commit](https://img.shields.io/github/last-commit/Aanlu/EFOR)

> **Status:** Interactive Alpha. The event-driven core, Win32 input backend,
> command engine, file-based logging, footer feedback and CI are implemented.
> Dynamic resizing, scroll, command history and release packaging remain open
> work in [issue #6](https://github.com/Aanlu/EFOR/issues/6).

EFOR is a lightweight, modular terminal file explorer written in C. It keeps a
platform-agnostic core, avoids external library dependencies, and is meant to be
a small, readable example of an event-driven console application on Windows.

## Why EFOR

- **Readable C99:** small functions, explicit state, no hidden framework.
- **Clear boundaries:** the core does not call Win32 directly; it uses interfaces.
- **Event-driven input:** blocking console reads, no busy-waiting loop.
- **Testable logic:** rules live in pure helpers whenever possible.

## Implemented features

- Directory listing through the Win32 backend; hidden, system and reparse-point
  entries are skipped.
- Navigation: arrow keys to move, LEFT to go back, RIGHT or ENTER to enter a
  directory, ESC to exit.
- A footer that shows command results and an explicit `y/n` prompt for
  destructive commands.
- Input failures and early exits restore the original console mode.
- Timestamped, append-mode logging to `debug.log`.
- 7 automated tests covering paths, logger, parser, app state, process
  lifecycle, input errors and redirected stdin.

## Commands

Press `:` to open the command line, then use:

```text
:mkdir <name>          create a directory
:touch <name>          create a file
:delete <name>         delete a file or directory (asks for confirmation)
:rename <old> <new>    rename an item (asks for confirmation)
:cd <path>             change directory
```

## Architecture

- **Platform-agnostic core:** application logic and event handling are separate
  from OS-specific filesystem APIs.
- **Centralized state:** `app_controller` and `app_state` own runtime state and
  mode transitions.
- **Custom parser:** a small, memory-safe hand-written command parser.
- **Rendering boundary:** `app_state_footer_text()` decides the footer text;
  the Win32 renderer only prints it.
- **Logging:** timestamped, append-only diagnostics written to `debug.log`.

## Project structure

```text
include/              Public API and interfaces (.h)
├── core/             State, lifecycle and main controller
│   └── interfaces/   Input and filesystem abstractions
├── parsers/          Command parsing definitions
src/                  Implementations (.c)
├── core/             Internal logic and event delegation
├── parsers/          Command parsing algorithms
├── platform/         Native implementations (currently Win32)
│   └── windows/
└── main.c            Application entry point
tests/                Unit and integration tests
```

## Build and test

Requirements: CMake 3.10+ and a C99 compiler. On Windows the current setup uses
MSVC through Visual Studio.

```sh
cmake -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug
```

Run the application:

```sh
.\build\Debug\efor.exe
```

The `--config Debug` flag is needed with multi-config generators such as Visual
Studio. With single-config generators such as Ninja, use `cmake --build build`.

## Roadmap

Work for the v0.3.0 milestone is tracked in
[issue #6](https://github.com/Aanlu/EFOR/issues/6). Remaining items include a
dynamic, size-aware UI with scroll, command history, CI on `windows-latest`, an
English UI, `CHANGELOG.md`, and the version bump to 0.3.0.

## Documentation

- [Contributing guide](./CONTRIBUTING.md)
- [Code of conduct](./CODE_OF_CONDUCT.md)
- [Issue templates](./.github/ISSUE_TEMPLATE)
- [Pull request template](./.github/pull_request_template.md)

## License

This project is licensed under the [MIT License](./LICENSE).
