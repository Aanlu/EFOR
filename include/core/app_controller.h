#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <stdbool.h>
#include "core/app_state.h"
#include "platform/fs_reader.h"
#include "platform/input.h"

bool handle_input(AppState* state, const FSDirectoryContent* content);
void app_sync_filesystem(AppState* state, FSDirectoryContent* content);

#endif