#include "gamepad.h"
#include "types.h"
#include "utils.h"
#include "window.h"

bool GamepadConnected(Gamepad* gamepad) {
    return gamepad->connected;
}

bool GamepadButtonPressed(Gamepad* gamepad, GamepadButton button) {
    if (GamepadConnected(gamepad)) {
        return bitSet(gamepad->buttons, button);
    }
    return false;
}

bool GamepadButtonJustPressed(Gamepad* gamepad, GamepadButton button) {
    if (GamepadConnected(gamepad)) {
        bool pressed = bitSet(gamepad->buttons, button);
        return pressed && (pressed != bitSet(gamepad->pButtons, button));
    }
    return false;
}

bool GamepadButtonJustReleased(Gamepad* gamepad, GamepadButton button) {
    if (GamepadConnected(gamepad)) {
        bool released = bitUnset(gamepad->buttons, button);
        return released && (released != bitUnset(gamepad->pButtons, button));
    }
    return false;
}

float32 GamepadAxisValue(Gamepad* gamepad, GamepadAxis axis) {
    if (GamepadConnected(gamepad)) {
        return gamepad->axes[axis];
    }
    return false;
}