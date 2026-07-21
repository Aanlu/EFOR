#ifndef INPUT_H
#define INPUT_H


typedef enum{
    KEY_ACTION_NONE,
    KEY_ACTION_UP,
    KEY_ACTION_DOWN,
    KEY_ACTION_LEFT,
    KEY_ACTION_RIGHT,
    KEY_ACTION_ENTER,
    KEY_ACTION_ESCAPE
} KeyAction;

KeyAction input_get_action(void);

void platform_sleep_ms(unsigned int milliseconds);

#endif