#ifndef Mouse_H
#define Mouse_H

#include "types.h"

typedef struct Window Window;

// `MouseButton` represents the buttons on a standard mouse.
//
// Most likely, there are 5 buttons at most on a mouse. Some gaming mice may
// have more buttons but they usually get mapped to keyboard keys or have other
// purposes like changing CPI/DPI instead
typedef enum MouseButton {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_BACK,
    MOUSE_FORWARD,
} MouseButton;

// `MousePressed` returns true if the specified mouse button is pressed.
bool MousePressed(Window* window, MouseButton button);

// `MouseJustPressed` returns true if the specified mouse button has just been
// pressed since the last call to `WindowUpdate`.
bool MouseJustPressed(Window* window, MouseButton button);

// `MouseJustReleased` returns true if the specified mouse button has just been
// released since the last call to `WindowUpdate`.
bool MouseJustReleased(Window* window, MouseButton button);

#endif  // Mouse_H