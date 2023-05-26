#ifndef Keyboard_H
#define Keyboard_H

#include "types.h"

typedef struct Window Window;

// `Key` represents a keyboard key code. Each key code corresponds to a key on a
// keyboard.
typedef enum Key Key;

// `KeyMod` represents modifier keys that could change or modify the
// behavior of other certain gestures.
typedef enum KeyMod KeyMod;

// `KeyPressed` returns true if the current key is pressed.
bool KeyPressed(Window* window, Key key);

// `KeyJustPressed` returns true if the specified key code has just been pressed
// since the last call to `WindowUpdate`
bool KeyJustPressed(Window* window, Key key);

// `KeyJustReleased` returns true if the specified key code has just been
// release since the last call to `WindowUpdate`.
bool KeyJustReleased(Window* window, Key key);

// `KeyModSet` checks that the modifier is currently enabled.
bool KeyModSet(Window* window, KeyMod modifier);

// `KeyGetChar` returns the character that the combinations of pressed keys
// would produce.
rune KeyGetChar(Window* window);

enum PACK_ENUM KeyMod {
    KeyMod_Ctrl = (1 << 0),
    KeyMod_Shift = (1 << 1),
    KeyMod_Alt = (1 << 2),
    KeyMod_Win = (1 << 3),
    KeyMod_CapsLock = (1 << 4),
    KeyMod_ScrollLock = (1 << 5),
    KeyMod_NumLock = (1 << 6),
};

enum PACK_ENUM Key {
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,
    Key_LeftAlt,
    Key_LeftOption = Key_LeftAlt,
    Key_RightAlt,
    Key_RightOption = Key_RightAlt,
    Key_DownArrow,
    Key_LeftArrow,
    Key_RightArrow,
    Key_UpArrow,
    Key_Tilde,
    Key_Backslash,
    Key_Backspace,
    Key_LeftBracket,
    Key_RightBracket,
    Key_CapsLock,
    Key_Comma,
    Key_Menu,
    Key_LeftCtrl,
    Key_RightCtrl,
    Key_Delete,
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,
    Key_End,
    Key_Enter,
    Key_Equal,
    Key_Escape,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_Home,
    Key_Insert,
    Key_LeftWin,
    Key_LeftSuper = Key_LeftWin,
    Key_LeftCommand = Key_LeftWin,
    Key_RightWin,
    Key_RightSuper = Key_RightWin,
    Key_RightCommand = Key_RightWin,
    Key_Minus,
    Key_NumLock,
    Key_NumPad0,
    Key_NumPad1,
    Key_NumPad2,
    Key_NumPad3,
    Key_NumPad4,
    Key_NumPad5,
    Key_NumPad6,
    Key_NumPad7,
    Key_NumPad8,
    Key_NumPad9,
    Key_NumPadAdd,
    Key_NumPadDecimal,
    Key_NumPadDivide,
    Key_NumPadEnter,
    Key_NumPadEqual,
    Key_NumPadMultiply,
    Key_NumPadSubtract,
    Key_PageDown,
    Key_PageUp,
    Key_Pause,
    Key_Period,
    Key_PrintScreen,
    Key_Quote,
    Key_ScrollLock,
    Key_Semicolon,
    Key_LeftShift,
    Key_RightShift,
    Key_Slash,
    Key_Space,
    Key_Tab,
    Key_Alt,
    Key_Option = Key_Alt,
    Key_Ctrl,
    Key_Shift,
    Key_Win,
    Key_Super = Key_Win,
    Key_Command = Key_Win,

    // `Key_COUNT` is not a key code. It denotes the number of key codes
    // available to check.
    Key_Count,

    // Used internally to denote an unsupported key code.
    Key_Invalid = ~0,
};

#endif  // Keyboard_H