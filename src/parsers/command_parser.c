#include <string.h>
#include <ctype.h>

#include "parsers/command_parser.h"

static char Token[MAX_CMD_ARGS][MAX_ARG_LEN];
static ParsedCommand detectCommandType(int arg_count);

ParsedCommand command_parser(const char* raw_buffer, size_t buffer_len){
    if(!raw_buffer) return (ParsedCommand){.type = CMD_TYPE_NONE };
    if (raw_buffer[1] == '\0') return (ParsedCommand){.type = CMD_TYPE_EMPTY};

    int args = 0;
    size_t character = 1;
    int characterToken = 0;

    memset(Token, 0, sizeof(Token));

    while(character < buffer_len){
        if(isspace((unsigned char)raw_buffer[character])){
            if (character > 1 && !isspace((unsigned char)raw_buffer[character-1])){
                Token[args][characterToken] = '\0';
                args++;
                characterToken = 0;
            }
            character++;
            continue;
        }
        
        if (args < MAX_CMD_ARGS && characterToken < MAX_ARG_LEN - 1) {
            Token[args][characterToken] = raw_buffer[character];
            characterToken++;
        }
        character++;
    }

    if (characterToken > 0 && args < MAX_CMD_ARGS) {
        Token[args][characterToken] = '\0';
        args++;
    }

    if (args > MAX_CMD_ARGS) return (ParsedCommand){.type = CMD_TYPE_TOO_MANY_ARGS};

    return detectCommandType(args);
}

static ParsedCommand detectCommandType(int arg_count) {
    ParsedCommand cmd;
    memset(&cmd, 0, sizeof(ParsedCommand));

    if (arg_count == 0) {
        cmd.type = CMD_TYPE_EMPTY;
        return cmd;
    }

    for (int i = 0; i < arg_count && i < MAX_CMD_ARGS; i++) {
        strncpy(cmd.args[i], Token[i], MAX_ARG_LEN - 1);
        cmd.args[i][MAX_ARG_LEN - 1] = '\0';
    }

    if (strcmp(Token[0], "mkdir") == 0) {
        cmd.type = CMD_TYPE_MKDIR;
    }
    else if (strcmp(Token[0], "cd") == 0) {
        cmd.type = CMD_TYPE_CD;
    }
    else if (strcmp(Token[0], "delete") == 0) {
        cmd.type = CMD_TYPE_DELETE;
        cmd.is_destructive = true;
    }
    else if (strcmp(Token[0], "rename") == 0) {
        cmd.type = CMD_TYPE_RENAME;
        cmd.is_destructive = true;
    }
    else if (strcmp(Token[0], "touch") == 0) {
        cmd.type = CMD_TYPE_TOUCH;
    }
    else {
        cmd.type = CMD_TYPE_UNKNOWN;
    }

    return cmd;
}