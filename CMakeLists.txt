cmake_minimum_required(VERSION 3.10)

project(EFOR VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CORE_SOURCES
    src/main.c
    src/core/app_state.c
    src/core/app_controller.c
    src/core/command_executor.c
    src/parsers/command_parser.c
)

if(WIN32)
    set(PLATFORM_SOURCES
        src/platform/windows/fs_reader_win.c
        src/platform/windows/fs_win.c
        src/platform/windows/input_win.c
    )
else()
    set(PLATFORM_SOURCES "")
endif()

add_executable(efor ${CORE_SOURCES} ${PLATFORM_SOURCES})