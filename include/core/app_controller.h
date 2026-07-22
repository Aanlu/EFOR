#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "core/app_state.h"
#include "platform/fs_reader.h"
#include "platform/input.h"

bool handle_input(AppState* state, const FSDirectoryContent* content);

#endif

