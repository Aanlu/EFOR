#include <assert.h>

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

int main(void) {
    test_index_equal_to_count_clamps_to_last();
    test_index_far_past_end_clamps_to_last();
    test_last_valid_index_is_unchanged();
    test_valid_index_is_unchanged();
    test_empty_directory_returns_zero();
    test_negative_index_returns_zero();
    return 0;
}
