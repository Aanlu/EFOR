#include <assert.h>

#include "core/app_controller.h"
#include "core/app_state.h"
#include "core/interfaces/fs_reader.h"
#include "core/interfaces/input.h"

InputEvent input_get_action(void) {
    return (InputEvent){ .action = KEY_ACTION_ERROR, .character = '\0' };
}

void platform_clear_screen(void) {
}

int main(void) {
    AppState state;
    FSDirectoryContent content;

    init_app_state(&state);
    fs_directory_content_init(&content);

    bool needs_redraw = handle_input(&state, &content);

    assert(!needs_redraw);
    assert(!state.running);
    assert(state.input_failed);
    assert(state.status_is_error);
    return 0;
}
