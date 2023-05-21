#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "types.h"
#include "utils.h"
#include "window.h"

extern inline Color ColorHex(const uint32 color) {
    return (Color){
        (uint8)(color >> 16),
        (uint8)(color >> 8),
        (uint8)(color >> 0),
        (uint8)(color >> 24),
    };
}

bool MousePressed(Window *window, MouseButton button) {
    return bitSet(window->mousePressed, button);
}

bool MouseJustPressed(Window *window, MouseButton button) {
    bool pressed = bitSet(window->mousePressed, button);
    return pressed && pressed != bitSet(window->pMousePressed, button);
}

bool MouseJustReleased(Window *window, MouseButton button) {
    bool released = bitUnset(window->mousePressed, button);
    return released && released != bitUnset(window->pMousePressed, button);
}

bool KeyPressed(Window *window, Key key) {
    return window->keyPressed[key];
}

bool KeyJustPressed(Window *window, Key key) {
    return window->keyPressed[key] && window->keyPressed[key] != window->pKeyPressed[key];
}

bool KeyJustReleased(Window *window, Key key) {
    return window->keyPressed[key] == false && window->keyPressed[key] != window->pKeyPressed[key];
}

bool KeyModifierSet(Window *window, KeyModifier modifier) {
    return ((window->keyModifier | modifier) == modifier);
}

rune KeyGetChar(Window *window) {
    return window->keyChar;
}

Gamepad *WindowGetGamepad(Window *window, int playerID) {
    return GamepadListGet(&window->gamepads, playerID);
}

extern inline Gamepad *WindowGetFirstConnectedGamepad(Window *window) {
    return GamepadListGet(&window->gamepads, 0);
}
