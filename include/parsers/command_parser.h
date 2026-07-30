#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_CMD_ARGS 3
#define MAX_ARG_LEN 256

typedef enum {
    CMD_TYPE_NONE,
    CMD_TYPE_EMPTY,
    CMD_TYPE_UNKNOWN,
    CMD_TYPE_TOO_MANY_ARGS,

    CMD_TYPE_MKDIR,
    CMD_TYPE_DELETE,
    CMD_TYPE_RENAME,
    CMD_TYPE_TOUCH,
    CMD_TYPE_CD,

    CMD_TYPE_HELP,
    CMD_TYPE_CLEAR,
    CMD_TYPE_EXIT
} CommandType;

typedef struct{
    CommandType type;
    char args[MAX_CMD_ARGS][MAX_ARG_LEN];
    int arg_count;
    bool is_destructive;
}ParsedCommand;

ParsedCommand command_parser(const char* raw_buffer, size_t buffer_len);

#endif