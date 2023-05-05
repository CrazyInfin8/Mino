#include <minwindef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <utils.h>

#include <window.h>
#include <mouse.h>
#include <keyboard.h>

#if PLATFORM == Windows

#include <windows.h>
#include <winuser.h>

struct WindowNative {
    HWND windowHandle;
};

// clang-format off
const Key WinKey2MinoKey[256] = {['A'] = KEY_A, ['B'] = KEY_B, ['C'] = KEY_C, ['D'] = KEY_D, ['E'] = KEY_E, ['F'] = KEY_F, ['G'] = KEY_G, ['H'] = KEY_H, ['I'] = KEY_I, ['J'] = KEY_J, ['K'] = KEY_K, ['L'] = KEY_L, ['M'] = KEY_M, ['N'] = KEY_N, ['O'] = KEY_O, ['P'] = KEY_p, ['Q'] = KEY_Q, ['R'] = KEY_R, ['S'] = KEY_S, ['T'] = KEY_T, ['U'] = KEY_U, ['V'] = KEY_V, ['W'] = KEY_W, ['X'] = KEY_X, ['Y'] = KEY_Y, ['Z'] = KEY_Z, [VK_LMENU] = KEY_LEFT_ALT, [VK_RMENU] = KEY_RIGHT_ALT, [VK_DOWN] = KEY_DOWN_ARROW, [VK_LEFT] = KEY_LEFT_ARROW, [VK_RIGHT] = KEY_RIGHT_ARROW, [VK_UP] = KEY_UP_ARROW, [VK_OEM_3] = KEY_TILDE, [VK_OEM_5] = KEY_BACKSLASH, [VK_BACK] = KEY_BACKSPACE, [VK_OEM_4] = KEY_LEFT_BRACKET, [VK_OEM_6] = KEY_RIGHT_BRACKET, [VK_CAPITAL] = KEY_CAPS_LOCK, [VK_OEM_COMMA] = KEY_COMMA, [VK_APPS] = KEY_CONTEXT_MENU, [VK_LCONTROL] = KEY_LEFT_CONTROL, [VK_RCONTROL] = KEY_RIGHT_CONTROL, [VK_DELETE] = KEY_DELETE, ['0'] = KEY_0, ['1'] = KEY_1, ['2'] = KEY_2, ['3'] = KEY_3, ['4'] = KEY_4, ['5'] = KEY_5, ['6'] = KEY_6, ['7'] = KEY_7, ['8'] = KEY_8, ['9'] = KEY_9, [VK_END] = KEY_END, [VK_RETURN] = KEY_ENTER, [VK_OEM_PLUS] = KEY_EQUAL, [VK_ESCAPE] = KEY_ESCAPE, [VK_F1] = KEY_F1, [VK_F2] = KEY_F2, [VK_F3] = KEY_F3, [VK_F4] = KEY_F4, [VK_F5] = KEY_F5, [VK_F6] = KEY_F6, [VK_F7] = KEY_F7, [VK_F8] = KEY_F8, [VK_F9] = KEY_F9, [VK_F10] = KEY_F10, [VK_F11] = KEY_F11, [VK_F12] = KEY_F12, [VK_HOME] = KEY_HOME, [VK_INSERT] = KEY_INSERT, [VK_LWIN] = KEY_LEFT_WIN, [VK_RWIN] = KEY_RIGHT_WIN, [VK_OEM_MINUS] = KEY_MINUS, [VK_NUMLOCK] = KEY_NUMLOCK, [VK_NUMPAD0] = KEY_NP_0, [VK_NUMPAD1] = KEY_NP_1, [VK_NUMPAD2] = KEY_NP_2, [VK_NUMPAD3] = KEY_NP_3, [VK_NUMPAD4] = KEY_NP_4, [VK_NUMPAD5] = KEY_NP_5, [VK_NUMPAD6] = KEY_NP_6, [VK_NUMPAD7] = KEY_NP_7, [VK_NUMPAD8] = KEY_NP_8, [VK_NUMPAD9] = KEY_NP_9, [VK_ADD] = KEY_NP_ADD, [VK_DECIMAL] = KEY_NP_DECIMAL, [VK_DIVIDE] = KEY_NP_DIVIDE, [VK_SEPARATOR] = KEY_NP_ENTER, /*KEY_NP_EQUAL*/    [VK_MULTIPLY] = KEY_NP_MULTIPLY, [VK_SUBTRACT] = KEY_NP_SUBTRACT, [VK_NEXT] = KEY_PAGE_DOWN, [VK_PRIOR] = KEY_PAGE_UP, [VK_PAUSE] = KEY_PAUSE, [VK_OEM_PERIOD] = KEY_PERIOD, [VK_SNAPSHOT] = KEY_PRINT_SCREEN, [VK_OEM_7] = KEY_QUOTE, [VK_SCROLL] = KEY_SCROLL_LOCK, [VK_OEM_1] = KEY_SEMICOLON, [VK_LSHIFT] = KEY_LEFT_SHIFT, [VK_RSHIFT] = KEY_RIGHT_SHIFT, [VK_OEM_2] = KEY_SLASH, [VK_SPACE] = KEY_SPACE, [VK_TAB] = KEY_TAB, [VK_MENU] = KEY_ALT, [VK_CONTROL] = KEY_CONTROL, [VK_SHIFT] = KEY_SHIFT, /*KEY_WIN*/};
const int MinoKey2WinKey[KEY_COUNT] = {[KEY_A] = 'A', [KEY_B] = 'B', [KEY_C] = 'C', [KEY_D] = 'D', [KEY_E] = 'E', [KEY_F] = 'F', [KEY_G] = 'G', [KEY_H] = 'H', [KEY_I] = 'I', [KEY_J] = 'J', [KEY_K] = 'K', [KEY_L] = 'L', [KEY_M] = 'M', [KEY_N] = 'N', [KEY_O] = 'O', [KEY_p] = 'P', [KEY_Q] = 'Q', [KEY_R] = 'R', [KEY_S] = 'S', [KEY_T] = 'T', [KEY_U] = 'U', [KEY_V] = 'V', [KEY_W] = 'W', [KEY_X] = 'X', [KEY_Y] = 'Y', [KEY_Z] = 'Z', [KEY_LEFT_ALT] = VK_LMENU, [KEY_RIGHT_ALT] = VK_RMENU, [KEY_DOWN_ARROW] = VK_DOWN, [KEY_LEFT_ARROW] = VK_LEFT, [KEY_RIGHT_ARROW] = VK_RIGHT, [KEY_UP_ARROW] = VK_UP, [KEY_TILDE] = VK_OEM_3, [KEY_BACKSLASH] = VK_OEM_5, [KEY_BACKSPACE] = VK_BACK, [KEY_LEFT_BRACKET] = VK_OEM_4, [KEY_RIGHT_BRACKET] = VK_OEM_6, [KEY_CAPS_LOCK] = VK_CAPITAL, [KEY_COMMA] = VK_OEM_COMMA, [KEY_CONTEXT_MENU] = VK_APPS, [KEY_LEFT_CONTROL] = VK_LCONTROL, [KEY_RIGHT_CONTROL] = VK_RCONTROL, [KEY_DELETE] = VK_DELETE, [KEY_0] = '0', [KEY_1] = '1', [KEY_2] = '2', [KEY_3] = '3', [KEY_4] = '4', [KEY_5] = '5', [KEY_6] = '6', [KEY_7] = '7', [KEY_8] = '8', [KEY_9] = '9', [KEY_END] = VK_END, [KEY_ENTER] = VK_RETURN, [KEY_EQUAL] = VK_OEM_PLUS, [KEY_ESCAPE] = VK_ESCAPE, [KEY_F1] = VK_F1, [KEY_F2] = VK_F2, [KEY_F3] = VK_F3, [KEY_F4] = VK_F4, [KEY_F5] = VK_F5, [KEY_F6] = VK_F6, [KEY_F7] = VK_F7, [KEY_F8] = VK_F8, [KEY_F9] = VK_F9, [KEY_F10] = VK_F10, [KEY_F11] = VK_F11, [KEY_F12] = VK_F12, [KEY_HOME] = VK_HOME, [KEY_INSERT] = VK_INSERT, [KEY_LEFT_WIN] = VK_LWIN, [KEY_RIGHT_WIN] = VK_RWIN, [KEY_MINUS] = VK_OEM_MINUS, [KEY_NUMLOCK] = VK_NUMLOCK, [KEY_NP_0] = VK_NUMPAD0, [KEY_NP_1] = VK_NUMPAD1, [KEY_NP_2] = VK_NUMPAD2, [KEY_NP_3] = VK_NUMPAD3, [KEY_NP_4] = VK_NUMPAD4, [KEY_NP_5] = VK_NUMPAD5, [KEY_NP_6] = VK_NUMPAD6, [KEY_NP_7] = VK_NUMPAD7, [KEY_NP_8] = VK_NUMPAD8, [KEY_NP_9] = VK_NUMPAD9, [KEY_NP_ADD] = VK_ADD, [KEY_NP_DECIMAL] = VK_DECIMAL, [KEY_NP_DIVIDE] = VK_DIVIDE, [KEY_NP_ENTER] = VK_SEPARATOR, /*KEY_NP_EQUAL*/    [KEY_NP_MULTIPLY] = VK_MULTIPLY, [KEY_NP_SUBTRACT] = VK_SUBTRACT, [KEY_PAGE_DOWN] = VK_NEXT, [KEY_PAGE_UP] = VK_PRIOR, [KEY_PAUSE] = VK_PAUSE, [KEY_PERIOD] = VK_OEM_PERIOD, [KEY_PRINT_SCREEN] = VK_SNAPSHOT, [KEY_QUOTE] = VK_OEM_7, [KEY_SCROLL_LOCK] = VK_SCROLL, [KEY_SEMICOLON] = VK_OEM_1, [KEY_LEFT_SHIFT] = VK_LSHIFT, [KEY_RIGHT_SHIFT] = VK_RSHIFT, [KEY_SLASH] = VK_OEM_2, [KEY_SPACE] = VK_SPACE, [KEY_TAB] = VK_TAB, [KEY_ALT] = VK_MENU, [KEY_CONTROL] = VK_CONTROL, [KEY_SHIFT] = VK_SHIFT, /*KEY_WIN*/};
// clang-format on

static LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT msg, WPARAM wParam,
    LPARAM lParam) {
    Window *window = (Window *)GetWindowLongPtr(windowHandle, GWLP_USERDATA);

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(windowHandle, &ps);
            EndPaint(windowHandle, &ps);
        } break;

        case WM_CLOSE: {
            DestroyWindow(windowHandle);
        } break;

        case WM_LBUTTONDOWN: {
            setBit(window->mousePressed, MOUSE_LEFT);
        } break;

        case WM_LBUTTONUP: {
            unsetBit(window->mousePressed, MOUSE_LEFT);
        } break;

        case WM_RBUTTONDOWN: {
            setBit(window->mousePressed, MOUSE_RIGHT);
        } break;

        case WM_RBUTTONUP: {
            unsetBit(window->mousePressed, MOUSE_RIGHT);
        } break;

        case WM_MBUTTONDOWN: {
            setBit(window->mousePressed, MOUSE_MIDDLE);
        } break;

        case WM_MBUTTONUP: {
            unsetBit(window->mousePressed, MOUSE_MIDDLE);
        } break;

        case WM_XBUTTONDOWN: {
            setBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MOUSE_BACK : MOUSE_FORWARD);
        } break;

        case WM_XBUTTONUP: {
            unsetBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MOUSE_BACK : MOUSE_FORWARD);
        } break;

        case WM_MOUSEMOVE: {
            window->mouseX = LOWORD(lParam);
            window->mouseY = HIWORD(lParam);
        } break;

        case WM_KEYDOWN:
        case WM_KEYUP: {
            Key key = WinKey2MinoKey[wParam];
            window->keyPressed[key] = (msg == WM_KEYDOWN);

            // Check modifiers
            window->keyModifier =
                (GetKeyState(VK_CONTROL) & 0x8000 ? MOD_CONTROL : 0) |
                (GetKeyState(VK_SHIFT) & 0x8000 ? MOD_SHIFT : 0) |
                ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000 ? MOD_WIN : 0) |
                (GetKeyState(VK_CAPITAL) & 0x0001 ? MOD_CAPS_LOCK : 0) |
                (GetKeyState(VK_SCROLL) & 0x0001 ? MOD_SCROLL_LOCK : 0) |
                (GetKeyState(VK_NUMLOCK) & 0x0001 ? MOD_NUM_LOCK : 0);
        } break;

        case WM_CHAR: {
            window->keyChar = wParam;
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        default: {
            return DefWindowProc(windowHandle, msg, wParam, lParam);
        }
    }
    return 0;
}

bool WindowInit(Window *window, WindowConfig config) {
    HINSTANCE instance = GetModuleHandle(NULL);
    WNDCLASSEX windowClass = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_VREDRAW | CS_HREDRAW,
        .lpfnWndProc = WindowProcedure,
        .hInstance = instance,
        .lpszClassName = "Mino Window Class"};
    RegisterClassEx(&windowClass);

    WindowNative *native = window->native = allocate(WindowNative);

    native->windowHandle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        windowClass.lpszClassName,
        config.title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        config.width, config.height,
        NULL, NULL, instance, NULL);

    if (native->windowHandle == NULL) {
        return false;
    }

    SetWindowLongPtr(native->windowHandle, GWLP_USERDATA, (LONG_PTR)window);
    ShowWindow(native->windowHandle, SW_NORMAL);
    UpdateWindow(native->windowHandle);

    return true;
}

bool WindowUpdate(Window *window) {
    // Reset Keyboard and mouse info
    // memset(window->keyJustPressed, false, len(window->keyJustPressed, bool));
    // memset(window->keyJustReleased, false, len(window->keyJustReleased, bool));
    for (int i = 0; i < KEY_COUNT; i++) {
        window->pKeyPressed[i] = window->keyPressed[i];
    }
    window->pmouseX = window->mouseX;
    window->pmouseY = window->mouseY;
    window->pMousePressed = window->mousePressed;
    window->keyChar = 0;

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    InvalidateRect(window->native->windowHandle, NULL, FALSE);
    return true;
}

void WindowClose(Window *window) {
    CloseWindow(window->native->windowHandle);
    free(window->native);
    window->native = nil;
}

int64 WindowTime() {
    LARGE_INTEGER frequency, count;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&count);
    return (int64)(count.QuadPart * 1000.0 / frequency.QuadPart);
}

void WindowSleep(int64 milliseconds) {
    Sleep(milliseconds);
}

#elif PLATFORM == Linux

#endif

bool MousePressed(Window *window, MouseButton button) {
    return bitsSet(window->mousePressed, button);
}

bool MouseJustPressed(Window *window, MouseButton button) {
    return bitsSet(window->mousePressed, button) && bitsSet(window->mousePressed, button) != bitsSet(window->pMousePressed, button);
}

bool MouseJustReleased(Window *window, MouseButton button) {
    return bitsUnset(window->mousePressed, button) && bitsSet(window->mousePressed, button) != bitsSet(window->pMousePressed, button);
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