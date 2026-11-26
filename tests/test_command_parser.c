#include <assert.h>
#include <string.h>

#include "parsers/command_parser.h"

static void test_accepts_maximum_arguments(void) {
    const char* input = ":rename origen destino";
    ParsedCommand result = command_parser(input, strlen(input));

    assert(result.type == CMD_TYPE_RENAME);
    assert(result.arg_count == MAX_CMD_ARGS);
}

static void test_rejects_extra_final_argument_without_trailing_space(void) {
    const char* input = ":rename origen destino extra";
    ParsedCommand result = command_parser(input, strlen(input));

    assert(result.type == CMD_TYPE_TOO_MANY_ARGS);
}

static void test_rejects_extra_argument_with_trailing_space(void) {
    const char* input = ":rename origen destino extra ";
    ParsedCommand result = command_parser(input, strlen(input));

    assert(result.type == CMD_TYPE_TOO_MANY_ARGS);
}

static void test_empty_string_returns_empty(void) {
    const char* input = "";
    ParsedCommand result = command_parser(input, strlen(input));

    assert(result.type == CMD_TYPE_EMPTY);
}

static void test_colon_only_returns_empty(void) {
    const char* input = ":";
    ParsedCommand result = command_parser(input, strlen(input));

    assert(result.type == CMD_TYPE_EMPTY);
}

static void test_unterminated_single_char_returns_empty(void) {
    char buf[1]= { ':' };
    ParsedCommand result = command_parser(buf, 1);

    assert(result.type == CMD_TYPE_EMPTY);
}

static void test_null_returns_none(void) {
    ParsedCommand result = command_parser(NULL, 0);

    assert(result.type == CMD_TYPE_NONE);
}

int main(void) {
    test_accepts_maximum_arguments();
    test_rejects_extra_final_argument_without_trailing_space();
    test_rejects_extra_argument_with_trailing_space();
    test_empty_string_returns_empty();
    test_colon_only_returns_empty();
    test_unterminated_single_char_returns_empty();
    test_null_returns_none();
    return 0;
}
