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

add_executable(efor 
    src/main.c
    src/core/app_state.c
    src/platform/windows/fs_reader_win.c
    src/platform/windows/input_win.c
    src/core/app_controller.c
    
)