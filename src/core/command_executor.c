#include <stdio.h>
#include <string.h>

#include "core/app_state.h"
#include "core/command_executor.h"
#include "core/interfaces/fs.h"
#include "core/interfaces/fs_reader.h"
#include "core/logger.h"

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
    if (!state || !cmd) return;

    state->status_message[0] = '\0';
    state->status_is_error = false;

    char full_path[MAX_PATH_LENGTH];

    switch(cmd->type) {
        case CMD_TYPE_MKDIR:
            if (cmd->args[1][0] == '\0') {
                LOG_WARN("MKDIR command invoked without arguments");
                snprintf(state->status_message, sizeof(state->status_message), "Error: Missing folder name.");
                state->status_is_error = true;
                break;
            }
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            
            if (fs_create_directory(full_path)) {
                state->needs_reload = true;
                LOG_INFO("Directory created successfully: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Folder '%s' created.", cmd->args[1]);
            } else {
                LOG_ERROR("Failed to create directory: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Error: Could not create '%s'.", cmd->args[1]);
                state->status_is_error = true;
            }
            break;

        case CMD_TYPE_DELETE:
            if (cmd->args[1][0] == '\0') {
                LOG_WARN("DELETE command invoked without arguments");
                snprintf(state->status_message, sizeof(state->status_message), "Error: Missing target name.");
                state->status_is_error = true;
                break;
            }
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            
            if (fs_delete(full_path)) {
                state->needs_reload = true;
                LOG_INFO("Target deleted successfully: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Target '%s' deleted.", cmd->args[1]);
            } else {
                LOG_ERROR("Failed to delete target: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Error: Could not delete '%s'.", cmd->args[1]);
                state->status_is_error = true;
            }
            break;

        case CMD_TYPE_TOUCH:
            if (cmd->args[1][0] == '\0') {
                LOG_WARN("TOUCH command invoked without arguments");
                snprintf(state->status_message, sizeof(state->status_message), "Error: Missing file name.");
                state->status_is_error = true;
                break;
            }
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            
            if (fs_create_file(full_path)) {
                state->needs_reload = true;
                LOG_INFO("File created successfully: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "File '%s' created.", cmd->args[1]);
            } else {
                LOG_ERROR("Failed to create file: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Error: Could not create '%s'.", cmd->args[1]);
                state->status_is_error = true;
            }
            break;

        case CMD_TYPE_RENAME: {
            if (cmd->args[1][0] == '\0' || cmd->args[2][0] == '\0') {
                LOG_WARN("RENAME command invoked with missing arguments");
                snprintf(state->status_message, sizeof(state->status_message), "Error: Rename requires current and new name.");
                state->status_is_error = true;
                break;
            }
            
            char new_full_path[MAX_PATH_LENGTH];
            build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            build_full_path(new_full_path, sizeof(new_full_path), state->current_path, cmd->args[2]);

            if (fs_rename(full_path, new_full_path)) {
                state->needs_reload = true;
                LOG_INFO("Renamed '%s' to '%s'", full_path, new_full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Renamed to '%s'.", cmd->args[2]);
            } else {
                LOG_ERROR("Failed to rename '%s' to '%s'", full_path, new_full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Error: Could not rename '%s'.", cmd->args[1]);
                state->status_is_error = true;
            }
            break;
        }

        case CMD_TYPE_CD:
            if (cmd->args[1][0] == '\0') {
                LOG_WARN("CD command invoked without arguments");
                snprintf(state->status_message, sizeof(state->status_message), "Error: Missing directory path.");
                state->status_is_error = true;
                break;
            }

            if (cmd->args[1][1] == ':' || cmd->args[1][0] == '\\' || cmd->args[1][0] == '/') {
                snprintf(full_path, sizeof(full_path), "%s", cmd->args[1]);
            } else {
                build_full_path(full_path, sizeof(full_path), state->current_path, cmd->args[1]);
            }

            if (fs_is_directory(full_path)) {
                snprintf(state->target_path, sizeof(state->target_path), "%s", full_path);
                state->needs_sync = true;
                LOG_INFO("Changed directory to: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Directory changed.");
            } else {
                LOG_ERROR("Invalid directory path requested: %s", full_path);
                snprintf(state->status_message, sizeof(state->status_message), "Error: Invalid directory path.");
                state->status_is_error = true;
            }
            break;

        case CMD_TYPE_UNKNOWN:
        default:
            LOG_WARN("Unrecognized command executed");
            snprintf(state->status_message, sizeof(state->status_message), "Error: Unknown command.");
            state->status_is_error = true;
            break;
    }
}