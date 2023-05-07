#ifndef Window_H
#define Window_H

#include <stdbool.h>
#include <types.h>
#include <keyboard.h>

typedef struct WindowNative WindowNative;

typedef struct WindowConfig {
    const char *title;
    const int width, height;
} WindowConfig;

typedef struct Window {
    bool keyPressed[KEY_COUNT];
    bool pKeyPressed[KEY_COUNT];
    byte keyModifier;
    rune keyChar;

    int mouseX, mouseY;
    int pmouseX, pmouseY;

    byte mousePressed;
    byte pMousePressed;

    WindowNative* native;
} Window;

typedef struct Window Window;

bool WindowInit(Window* window, WindowConfig config);

bool WindowUpdate(Window* window);

bool WindowUpdateNextFrame(Window *window, int framerate, float32 *deltaTime);

void WindowClose(Window* window);

int64 WindowTime();

void WindowSleep(int64 milliseconds);

#endif  // Window_H