#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "core/app_state.h"
#include "core/app_controller.h"
#include "core/interfaces/fs_reader.h"
#include "core/interfaces/input.h"

#define main efor_main_under_test
#include "../src/main.c"
#undef main

static bool input_init_succeeds;
static int directory_read_result;
static bool input_fails;
static int shutdown_calls;

bool platform_input_init(void) {
    return input_init_succeeds;
}

void platform_input_shutdown(void) {
    shutdown_calls++;
}

void fs_directory_content_init(FSDirectoryContent* content) {
    content->count = 0;
}

int fs_read_directory(const char* path, FSDirectoryContent* content) {
    (void)path;
    (void)content;
    return directory_read_result;
}

void fs_print_directory_content(
    AppState* state,
    const FSDirectoryContent* content,
    const char* current_path,
    int selected_index
) {
    (void)state;
    (void)content;
    (void)current_path;
    (void)selected_index;
}

bool handle_input(AppState* state, const FSDirectoryContent* content) {
    (void)content;
    state->input_failed = input_fails;
    state->running = 0;
    return false;
}

void app_sync_filesystem(AppState* state, FSDirectoryContent* content) {
    (void)state;
    (void)content;
}

static void reset_test_state(bool init_succeeds, int read_result, bool input_failure) {
    input_init_succeeds = init_succeeds;
    directory_read_result = read_result;
    input_fails = input_failure;
    shutdown_calls = 0;
}

static void test_failed_input_init_skips_shutdown(void) {
    reset_test_state(false, 0, false);

    assert(efor_main_under_test() == EXIT_FAILURE);
    assert(shutdown_calls == 0);
}

static void test_directory_read_failure_runs_shutdown(void) {
    reset_test_state(true, -1, false);

    assert(efor_main_under_test() != EXIT_SUCCESS);
    assert(shutdown_calls == 1);
}

static void test_input_failure_runs_shutdown_and_fails_process(void) {
    reset_test_state(true, 0, true);

    assert(efor_main_under_test() == EXIT_FAILURE);
    assert(shutdown_calls == 1);
}

static void test_normal_exit_restores_input(void) {
    reset_test_state(true, 0, false);

    assert(efor_main_under_test() == EXIT_SUCCESS);
    assert(shutdown_calls == 1);
}

int main(void) {
    test_failed_input_init_skips_shutdown();
    test_directory_read_failure_runs_shutdown();
    test_input_failure_runs_shutdown_and_fails_process();
    test_normal_exit_restores_input();
    return 0;
}
