#pragma once

#include "parsers/command_parser.h"

typedef struct AppState AppState;
void execute_command(AppState* state, const ParsedCommand* cmd);
