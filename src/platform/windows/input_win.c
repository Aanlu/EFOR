#include <conio.h>
#include <windows.h>

#include "platform/input.h"

static KeyAction read_extended_key(void) {
    int key = _getch();
    switch (key) {
        case 72: return KEY_ACTION_UP;
        case 80: return KEY_ACTION_DOWN;
        case 75: return KEY_ACTION_LEFT;
        case 77: return KEY_ACTION_RIGHT;
        default: return KEY_ACTION_NONE;
    }
}

static KeyAction read_standard_key(int ch) {
    switch (ch) {
        case 'w': case 'W': return KEY_ACTION_UP;
        case 's': case 'S': return KEY_ACTION_DOWN;
        case 'a': case 'A': return KEY_ACTION_LEFT;
        case 'd': case 'D': return KEY_ACTION_RIGHT;
        case 13:            return KEY_ACTION_ENTER;
        case 8:             return KEY_ACTION_LEFT;
        case 27: case 'q': case 'Q': return KEY_ACTION_ESCAPE;
        default:            return KEY_ACTION_NONE;
    }
}

KeyAction input_get_action(void) {
    if (!_kbhit()) {
        return KEY_ACTION_NONE;
    }

    int ch = _getch();
    if (ch == 0 || ch == 224) {
        return read_extended_key();
    }

    return read_standard_key(ch);
}

void platform_sleep_ms(unsigned int milliseconds) {
    Sleep(milliseconds);
}