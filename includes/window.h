#ifndef Window_H
#define Window_H

#include "gamepad.h"
#include "keyboard.h"
#include "types.h"

// `WindowNative` is the platforms native implementation of a window.
//
// It is not meant to be interacted with directly.
typedef struct WindowNative WindowNative;

// `WindowConfig` specifies settings used to create and initialize the window.
typedef struct WindowConfig {
    const char* title;
    const int width, height;
} WindowConfig;

// `Window` is the primary way to draw content to the screen and process user
// input.
typedef struct Window {
    bool keyPressed[KEY_COUNT];
    bool pKeyPressed[KEY_COUNT];
    byte keyModifier;
    rune keyChar;

    int mouseX, mouseY;
    int pMouseX, pMouseY;

    int scrollX, scrollY;
    int pScrollX, pScrollY;

    byte mousePressed;
    byte pMousePressed;

    Gamepad* firstGamepad;
    Gamepad* gamepads;
    int gamepadCount;

    WindowNative* native;
} Window;

// `WindowInit` Initializes and creates a new window with the settings in
// `config`
//
// Remember to close finished `Window`s with `WindowClose`
bool WindowInit(Window* window, WindowConfig config);

// `WindowUpdate` processes user input as well as updates the content of the
// window with previous graphic draw calls. This should be called every frame.
bool WindowUpdate(Window* window);

// `WindowClose` cleans up this window.
//
// It should be called when you are finished using this `Window`.
void WindowClose(Window* window);

// `WindowTime` milliseconds since the platforms start time.
//
// You can take readings of time from the start and end of a frame to figure out
// how long that frame has taken to finish.
int64 WindowTime();

// `WindowSleep` sleeps for the specified number of milliseconds.
//
// Currently, `WindowUpdate` does not wait until the next frame to return.
// Instead it may run without delay which can be unnecessarily taxing on your
// computer's resources. Finding the correct amount of time to sleep and calling
// this function can help prevent the computer from doing extra unnecessary
// work by sleeping.
void WindowSleep(int64 milliseconds);

#endif  // Window_H