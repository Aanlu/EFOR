#include "core/command_executor.h"
#include "fs/fs.h"
#include "platform/fs_reader.h"
#include "platform/input.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

    if (cmd->is_destructive) {
        printf("\n [ALERTA] ¿Estas seguro de aplicar este cambio? (y/n): ");
        fflush(stdout);

        bool confirmed = false;
        while (true) {
            InputEvent ev = input_get_action();
            
            if (ev.action == KEY_ACTION_TEXT) {
                char c = (char)tolower((unsigned char)ev.character);
                if (c == 'y') {
                    confirmed = true;
                    break;
                } else if (c == 'n') {
                    break;
                }
            } else if (ev.action == KEY_ACTION_ESCAPE) {
                break;
            }
            
            platform_sleep_ms(10);
        }

        if (!confirmed) {
            return;
        }
    }

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
                strncpy(full_path, cmd->args[1], sizeof(full_path) - 1);
                full_path[sizeof(full_path) - 1] = '\0';
            } else {
                build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            }

            if (fs_is_directory(full_path)) {
                strncpy(state->target_path, full_path, sizeof(state->target_path) - 1);
                state->target_path[sizeof(state->target_path) - 1] = '\0';
                state->needs_sync = true;
            }
            break;

        case CMD_TYPE_UNKNOWN:
        default:
            break;
    }
}