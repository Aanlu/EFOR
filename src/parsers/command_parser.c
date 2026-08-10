#include <string.h>
#include <ctype.h>

#include "parsers/command_parser.h"

static ParsedCommand detectCommandType(char tokens[MAX_CMD_ARGS][MAX_ARG_LEN], int arg_count);

ParsedCommand command_parser(const char* raw_buffer, size_t buffer_len) {
    char token[MAX_CMD_ARGS][MAX_ARG_LEN];
    if (!raw_buffer) return (ParsedCommand){ .type = CMD_TYPE_NONE };
    if (raw_buffer[1] == '\0') return (ParsedCommand){ .type = CMD_TYPE_EMPTY };

    int args = 0;
    size_t character = 1;
    int character_token = 0;

    memset(token, 0, sizeof(token));

    while (character < buffer_len) {
        if (isspace((unsigned char)raw_buffer[character])) {
            if (character > 1 && !isspace((unsigned char)raw_buffer[character - 1])) {
                token[args][character_token] = '\0';
                args++;
                character_token = 0;
            }
            character++;
            continue;
        }

        if (args < MAX_CMD_ARGS && character_token < MAX_ARG_LEN - 1) {
            token[args][character_token] = raw_buffer[character];
            character_token++;
        }
        character++;
    }

    if (character_token > 0 && args < MAX_CMD_ARGS) {
        token[args][character_token] = '\0';
        args++;
    }

    if (args > MAX_CMD_ARGS) return (ParsedCommand){ .type = CMD_TYPE_TOO_MANY_ARGS };

    return detectCommandType(token, args);
}

static ParsedCommand detectCommandType(char tokens[MAX_CMD_ARGS][MAX_ARG_LEN], int arg_count) {
    ParsedCommand cmd;
    memset(&cmd, 0, sizeof(ParsedCommand));

    if (arg_count == 0) {
        cmd.type = CMD_TYPE_EMPTY;
        return cmd;
    }

    cmd.arg_count = arg_count;

    for (int i = 0; i < arg_count && i < MAX_CMD_ARGS; i++) {
        strncpy(cmd.args[i], tokens[i], MAX_ARG_LEN - 1);
        cmd.args[i][MAX_ARG_LEN - 1] = '\0';
    }

    if (strcmp(tokens[0], "mkdir") == 0) {
        cmd.type = CMD_TYPE_MKDIR;
    } else if (strcmp(tokens[0], "cd") == 0) {
        cmd.type = CMD_TYPE_CD;
    } else if (strcmp(tokens[0], "delete") == 0) {
        cmd.type = CMD_TYPE_DELETE;
        cmd.is_destructive = true;
    } else if (strcmp(tokens[0], "rename") == 0) {
        cmd.type = CMD_TYPE_RENAME;
        cmd.is_destructive = true;
    } else if (strcmp(tokens[0], "touch") == 0) {
        cmd.type = CMD_TYPE_TOUCH;
    } else {
        cmd.type = CMD_TYPE_UNKNOWN;
    }

    return cmd;
}