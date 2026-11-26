#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "core/app_state.h"

static void test_index_equal_to_count_clamps_to_last(void) {
    int result = app_state_clamp_index(2, 2);

    assert(result == 1);
}

static void test_index_far_past_end_clamps_to_last(void) {
    int result = app_state_clamp_index(4, 2);

    assert(result == 1);
}

static void test_last_valid_index_is_unchanged(void) {
    int result = app_state_clamp_index(1, 2);

    assert(result == 1);
}

static void test_valid_index_is_unchanged(void) {
    int result = app_state_clamp_index(3, 5);

    assert(result == 3);
}

static void test_empty_directory_returns_zero(void) {
    int result = app_state_clamp_index(7, 0);

    assert(result == 0);
}

static void test_negative_index_returns_zero(void) {
    int result = app_state_clamp_index(-3, 5);

    assert(result == 0);
}

static void test_default_footer_without_message(void) {
    AppState s;
    init_app_state(&s);

    assert(strcmp(app_state_footer_text(&s),
                  "Presione ':' para ingresar un comando") == 0);
}

static void test_footer_shows_status_message(void) {
    AppState s;
    init_app_state(&s);

    snprintf(s.status_message, sizeof(s.status_message), "Folder 'x' created.");

    assert(strcmp(app_state_footer_text(&s), "Folder 'x' created.") == 0);
}

static void test_footer_shows_then_clears_message(void) {
    AppState s;
    init_app_state(&s);

    snprintf(s.status_message, sizeof(s.status_message), "Folder 'x' created.");
    assert(strcmp(app_state_footer_text(&s), "Folder 'x' created.") == 0);

    s.status_message[0] = '\0';

    assert(strcmp(app_state_footer_text(&s),
                  "Presione ':' para ingresar un comando") == 0);
}

static void test_footer_shows_command_buffer(void) {
    AppState s;
    init_app_state(&s);
    app_state_set_mode(&s, STATE_COMMAND_INPUT);

    snprintf(s.command_buffer, sizeof(s.command_buffer), ":mkdir prueba");

    assert(strcmp(app_state_footer_text(&s), ":mkdir prueba") == 0);
}

static void test_footer_shows_confirmation_prompt(void) {
    AppState s;
    init_app_state(&s);
    app_state_set_mode(&s, STATE_CONFIRMATION);

    assert(strstr(app_state_footer_text(&s), "Y/N") != NULL);
}

static void test_null_state_returns_empty_string(void) {
    assert(app_state_footer_text(NULL)[0] == '\0');
}

int main(void) {
    test_index_equal_to_count_clamps_to_last();
    test_index_far_past_end_clamps_to_last();
    test_last_valid_index_is_unchanged();
    test_valid_index_is_unchanged();
    test_empty_directory_returns_zero();
    test_negative_index_returns_zero();
    test_default_footer_without_message();
    test_footer_shows_status_message();
    test_footer_shows_then_clears_message();
    test_footer_shows_command_buffer();
    test_footer_shows_confirmation_prompt();
    test_null_state_returns_empty_string();
    return 0;
}
