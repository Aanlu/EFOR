#pragma once

#include <stdbool.h>

typedef struct AppState AppState;
typedef struct FSDirectoryContent FSDirectoryContent;

bool handle_input(AppState* state, const FSDirectoryContent* content);
void app_sync_filesystem(AppState* state, FSDirectoryContent* content);