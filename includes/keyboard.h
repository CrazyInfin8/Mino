#ifndef Keyboard_H
#define Keyboard_H

#include "types.h"

typedef struct Window Window;

// `Key` represents a keyboard key code. Each key code corresponds to a key on a
// keyboard.
typedef enum Key Key;

// `KeyModifier` represents modifier keys that could change or modify the
// behavior of other certain gestures.
typedef enum KeyModifier KeyModifier;

// `KeyPressed` returns true if the current key is pressed.
bool KeyPressed(Window* window, Key key);

// `KeyJustPressed` returns true if the specified key code has just been pressed
// since the last call to `WindowUpdate`
bool KeyJustPressed(Window* window, Key key);

// `KeyJustReleased` returns true if the specified key code has just been
// release since the last call to `WindowUpdate`.
bool KeyJustReleased(Window* window, Key key);

// `KeyModifierSet` checks that the modifier is currently enabled.
bool KeyModifierSet(Window* window, KeyModifier modifier);

// `KeyGetChar` returns the character that the combinations of pressed keys
// would produce.
rune KeyGetChar(Window* window);

enum PACK_ENUM KeyModifier {
    MOD_CONTROL = (1 << 0),
    MOD_SHIFT = (1 << 1),
    MOD_ALT = (1 << 2),
    MOD_WIN = (1 << 3),
    MOD_CAPS_LOCK = (1 << 4),
    MOD_SCROLL_LOCK = (1 << 5),
    MOD_NUM_LOCK = (1 << 6),
};

enum PACK_ENUM Key {
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_ALT,
    KEY_LEFT_OPTION = KEY_LEFT_ALT,
    KEY_RIGHT_ALT,
    KEY_RIGHT_OPTION = KEY_RIGHT_ALT,
    KEY_DOWN_ARROW,
    KEY_LEFT_ARROW,
    KEY_RIGHT_ARROW,
    KEY_UP_ARROW,
    KEY_TILDE,
    KEY_BACKSLASH,
    KEY_BACKSPACE,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_CAPS_LOCK,
    KEY_COMMA,
    KEY_CONTEXT_MENU,
    KEY_LEFT_CONTROL,
    KEY_RIGHT_CONTROL,
    KEY_DELETE,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_END,
    KEY_ENTER,
    KEY_EQUAL,
    KEY_ESCAPE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_HOME,
    KEY_INSERT,
    KEY_LEFT_WIN,
    KEY_LEFT_SUPER = KEY_LEFT_WIN,
    KEY_LEFT_COMMAND = KEY_LEFT_WIN,
    KEY_RIGHT_WIN,
    KEY_RIGHT_SUPER = KEY_RIGHT_WIN,
    KEY_RIGHT_COMMAND = KEY_RIGHT_WIN,
    KEY_MINUS,
    KEY_NUMLOCK,
    KEY_NP_0,
    KEY_NP_1,
    KEY_NP_2,
    KEY_NP_3,
    KEY_NP_4,
    KEY_NP_5,
    KEY_NP_6,
    KEY_NP_7,
    KEY_NP_8,
    KEY_NP_9,
    KEY_NP_ADD,
    KEY_NP_DECIMAL,
    KEY_NP_DIVIDE,
    KEY_NP_ENTER,
    KEY_NP_EQUAL,
    KEY_NP_MULTIPLY,
    KEY_NP_SUBTRACT,
    KEY_PAGE_DOWN,
    KEY_PAGE_UP,
    KEY_PAUSE,
    KEY_PERIOD,
    KEY_PRINT_SCREEN,
    KEY_QUOTE,
    KEY_SCROLL_LOCK,
    KEY_SEMICOLON,
    KEY_LEFT_SHIFT,
    KEY_RIGHT_SHIFT,
    KEY_SLASH,
    KEY_SPACE,
    KEY_TAB,
    KEY_ALT,
    KEY_OPTION = KEY_ALT,
    KEY_CONTROL,
    KEY_SHIFT,
    KEY_WIN,
    KEY_SUPER = KEY_WIN,
    KEY_COMMAND = KEY_WIN,
    // `KEY_COUNT` is not a key code. It denotes the number of key codes
    // available to check.
    KEY_COUNT,
    // Used internally to denote an unsupported key code.

    KEY_INVALID = ~0,
};

#endif  // Keyboard_H