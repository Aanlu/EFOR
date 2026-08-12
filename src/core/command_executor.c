#include <stdio.h>
#include <string.h>

#include "core/app_state.h"
#include "core/command_executor.h"
#include "core/interfaces/fs.h"
#include "core/interfaces/fs_reader.h"

static void build_full_path(char* dest, size_t dest_size, const char* current_path, const char* arg) {
    if (!dest || dest_size == 0 || !current_path || !arg) return;

    size_t len = strlen(current_path);

    if (len > 0 && current_path[len - 1] != '\\') {
        snprintf(dest, dest_size, "%s\\%s", current_path, arg);
    } else {
        snprintf(dest, dest_size, "%s%s", current_path, arg);
    }
}

void execute_command(AppState* state, const ParsedCommand* cmd) {
    if (!state || !cmd || cmd->args[1][0] == '\0') return;

    char full_path[MAX_PATH_LENGTH];

    switch(cmd->type) {
        case CMD_TYPE_MKDIR:
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            if (fs_create_directory(full_path)) {
                state->needs_reload = true;
            }
            break;

        case CMD_TYPE_DELETE:
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            if (fs_delete(full_path)) {
                state->needs_reload = true;
            }
            break;

        case CMD_TYPE_TOUCH:
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            if (fs_create_file(full_path)) {
                state->needs_reload = true;
            }
            break;

        case CMD_TYPE_RENAME: {
            if (cmd->args[2][0] == '\0') break;
            
            char new_full_path[MAX_PATH_LENGTH];
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            build_full_path(new_full_path, sizeof(new_full_path), state->current_path, cmd->args[2]);

            if (fs_rename(full_path, new_full_path)) {
                state->needs_reload = true;
            }
            break;
        }

        case CMD_TYPE_CD:
            if (cmd->args[1][1] == ':' || cmd->args[1][0] == '\\' || cmd->args[1][0] == '/') {
                snprintf(full_path, sizeof(full_path), "%s", cmd->args[1]);
            } else {
                build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            }

            if (fs_is_directory(full_path)) {
                snprintf(state->target_path, sizeof(state->target_path), "%s", full_path);
                state->needs_sync = true;
            }
            break;

        case CMD_TYPE_UNKNOWN:
        default:
            break;
    }
}
