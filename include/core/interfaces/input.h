#pragma once

typedef enum{
    KEY_ACTION_NONE,
    KEY_ACTION_UP,
    KEY_ACTION_DOWN,
    KEY_ACTION_LEFT,
    KEY_ACTION_RIGHT,
    KEY_ACTION_ENTER,
    KEY_ACTION_ESCAPE,
    KEY_ACTION_COPY,
    KEY_ACTION_TEXT,
    KEY_ACTION_BACKSPACE
} KeyAction;
typedef struct InputEvent{
    KeyAction action;
    char character;
}InputEvent;

InputEvent input_get_action(void);

void platform_sleep_ms(unsigned int milliseconds);
void platform_clear_screen(void);
void platform_input_init(void);
void platform_input_shutdown(void);
