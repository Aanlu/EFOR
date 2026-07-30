#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "core/app_state.h"
#include "parsers/command_parser.h"

void execute_command(AppState* state, const ParsedCommand* cmd);

#endif