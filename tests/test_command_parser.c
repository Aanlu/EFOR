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

int main(void) {
    test_accepts_maximum_arguments();
    test_rejects_extra_final_argument_without_trailing_space();
    test_rejects_extra_argument_with_trailing_space();
    return 0;
}
