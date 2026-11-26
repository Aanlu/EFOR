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
    src/core/path_utils.c
    src/core/logger.c
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

enable_testing()

add_executable(efor_tests tests/test_paths.c src/core/path_utils.c)
target_include_directories(efor_tests PRIVATE include)
add_test(NAME PathUtilsTest COMMAND $<TARGET_FILE:efor_tests>)

add_executable(efor_logger_tests tests/test_logger.c src/core/logger.c)
target_include_directories(efor_logger_tests PRIVATE include)
add_test(NAME LoggerTest COMMAND $<TARGET_FILE:efor_logger_tests>)

add_executable(efor_command_parser_tests tests/test_command_parser.c src/parsers/command_parser.c)
target_include_directories(efor_command_parser_tests PRIVATE include)
add_test(NAME CommandParserTest COMMAND $<TARGET_FILE:efor_command_parser_tests>)

add_executable(efor_app_state_tests tests/test_app_state.c src/core/app_state.c)
target_include_directories(efor_app_state_tests PRIVATE include)
add_test(NAME AppStateTest COMMAND $<TARGET_FILE:efor_app_state_tests>)

if(WIN32)
    add_executable(efor_main_lifecycle_tests
        tests/test_main_lifecycle.c
        src/core/app_state.c
    )
    target_include_directories(efor_main_lifecycle_tests PRIVATE include)
    add_test(NAME MainLifecycleTest COMMAND $<TARGET_FILE:efor_main_lifecycle_tests>)

    add_executable(efor_input_error_tests
        tests/test_input_error.c
        src/core/app_state.c
        src/core/app_controller.c
        src/core/command_executor.c
        src/core/path_utils.c
        src/core/logger.c
        src/parsers/command_parser.c
        src/platform/windows/fs_reader_win.c
        src/platform/windows/fs_win.c
    )
    target_include_directories(efor_input_error_tests PRIVATE include)
    add_test(NAME InputErrorControllerTest COMMAND $<TARGET_FILE:efor_input_error_tests>)

    add_test(
        NAME RedirectedInputTest
        COMMAND ${CMAKE_COMMAND}
            "-DEFOR_EXECUTABLE=$<TARGET_FILE:efor>"
            "-DTEST_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_redirected_input.cmake"
    )
endif()
