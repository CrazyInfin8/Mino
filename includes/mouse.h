#ifndef Mouse_H
#define Mouse_H

#include <types.h>

typedef struct Window Window;

typedef enum MouseButton {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_BACK,
    MOUSE_FORWARD,
} MouseButton;

bool MousePressed(Window* window, MouseButton button);

bool MouseJustPressed(Window* window, MouseButton button);

bool MouseJustReleased(Window* window, MouseButton button);

#endif  // Mouse_H