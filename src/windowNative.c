#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// X11 also defines a `Window` type that conflicts with Mino. This hack
// basically renames Mino's `Window` typedef to MinoWindow only for the linux
// platform so that windowNative can compile without issue and we can maintain
// intuitive names without prefixes.
//
// I suppose you can't include both X11 and Mino's `<window.h>` in your source
// code without this hack. But Mino's goal is to make it so that you don't need
// to do that. Mino aims to provide everything you needing to make your game
// without any of the platform specific code or headers.
#if defined(PLATFORM_Linux)
#define Window MinoWindow
#endif

#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "types.h"
#include "utils.h"
#include "window.h"

static void resetInputState(Window *window) {
    for (int i = 0; i < KEY_COUNT; i++) {
        window->pKeyPressed[i] = window->keyPressed[i];
    }
    window->pMouseX = window->mouseX;
    window->pMouseY = window->mouseY;
    window->pMousePressed = window->mousePressed;
    window->keyChar = 0;
    window->pScrollX = window->scrollX;
    window->pScrollY = window->scrollY;
    window->scrollX = 0;
    window->scrollY = 0;
    Gamepad *gamepad;
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false) continue;
        gamepad->pButtons = gamepad->buttons;
        for (int j = 0; j < GAMEPAD_AXIS_COUNT; j++) {
            gamepad->pAxes[j] = gamepad->axes[j];
        }
    }
}

#if defined(PLATFORM_Windows)

#include <windows.h>
#include <winuser.h>
#include <GL/gl.h>
#include <GL/wgl.h>
#include <wingdi.h>
#include <xinput.h>

struct WindowNative {
    HWND windowHandle;
    HGLRC glContext;
    int64 lastTick;
};

const Key WinKey2MinoKey[256] = {['A'] = KEY_A, ['B'] = KEY_B, ['C'] = KEY_C, ['D'] = KEY_D, ['E'] = KEY_E, ['F'] = KEY_F, ['G'] = KEY_G, ['H'] = KEY_H, ['I'] = KEY_I, ['J'] = KEY_J, ['K'] = KEY_K, ['L'] = KEY_L, ['M'] = KEY_M, ['N'] = KEY_N, ['O'] = KEY_O, ['P'] = KEY_p, ['Q'] = KEY_Q, ['R'] = KEY_R, ['S'] = KEY_S, ['T'] = KEY_T, ['U'] = KEY_U, ['V'] = KEY_V, ['W'] = KEY_W, ['X'] = KEY_X, ['Y'] = KEY_Y, ['Z'] = KEY_Z, [VK_LMENU] = KEY_LEFT_ALT, [VK_RMENU] = KEY_RIGHT_ALT, [VK_DOWN] = KEY_DOWN_ARROW, [VK_LEFT] = KEY_LEFT_ARROW, [VK_RIGHT] = KEY_RIGHT_ARROW, [VK_UP] = KEY_UP_ARROW, [VK_OEM_3] = KEY_TILDE, [VK_OEM_5] = KEY_BACKSLASH, [VK_BACK] = KEY_BACKSPACE, [VK_OEM_4] = KEY_LEFT_BRACKET, [VK_OEM_6] = KEY_RIGHT_BRACKET, [VK_CAPITAL] = KEY_CAPS_LOCK, [VK_OEM_COMMA] = KEY_COMMA, [VK_APPS] = KEY_CONTEXT_MENU, [VK_LCONTROL] = KEY_LEFT_CONTROL, [VK_RCONTROL] = KEY_RIGHT_CONTROL, [VK_DELETE] = KEY_DELETE, ['0'] = KEY_0, ['1'] = KEY_1, ['2'] = KEY_2, ['3'] = KEY_3, ['4'] = KEY_4, ['5'] = KEY_5, ['6'] = KEY_6, ['7'] = KEY_7, ['8'] = KEY_8, ['9'] = KEY_9, [VK_END] = KEY_END, [VK_RETURN] = KEY_ENTER, [VK_OEM_PLUS] = KEY_EQUAL, [VK_ESCAPE] = KEY_ESCAPE, [VK_F1] = KEY_F1, [VK_F2] = KEY_F2, [VK_F3] = KEY_F3, [VK_F4] = KEY_F4, [VK_F5] = KEY_F5, [VK_F6] = KEY_F6, [VK_F7] = KEY_F7, [VK_F8] = KEY_F8, [VK_F9] = KEY_F9, [VK_F10] = KEY_F10, [VK_F11] = KEY_F11, [VK_F12] = KEY_F12, [VK_HOME] = KEY_HOME, [VK_INSERT] = KEY_INSERT, [VK_LWIN] = KEY_LEFT_WIN, [VK_RWIN] = KEY_RIGHT_WIN, [VK_OEM_MINUS] = KEY_MINUS, [VK_NUMLOCK] = KEY_NUMLOCK, [VK_NUMPAD0] = KEY_NP_0, [VK_NUMPAD1] = KEY_NP_1, [VK_NUMPAD2] = KEY_NP_2, [VK_NUMPAD3] = KEY_NP_3, [VK_NUMPAD4] = KEY_NP_4, [VK_NUMPAD5] = KEY_NP_5, [VK_NUMPAD6] = KEY_NP_6, [VK_NUMPAD7] = KEY_NP_7, [VK_NUMPAD8] = KEY_NP_8, [VK_NUMPAD9] = KEY_NP_9, [VK_ADD] = KEY_NP_ADD, [VK_DECIMAL] = KEY_NP_DECIMAL, [VK_DIVIDE] = KEY_NP_DIVIDE, [VK_SEPARATOR] = KEY_NP_ENTER, /*KEY_NP_EQUAL*/[VK_MULTIPLY] = KEY_NP_MULTIPLY, [VK_SUBTRACT] = KEY_NP_SUBTRACT, [VK_NEXT] = KEY_PAGE_DOWN, [VK_PRIOR] = KEY_PAGE_UP, [VK_PAUSE] = KEY_PAUSE, [VK_OEM_PERIOD] = KEY_PERIOD, [VK_SNAPSHOT] = KEY_PRINT_SCREEN, [VK_OEM_7] = KEY_QUOTE, [VK_SCROLL] = KEY_SCROLL_LOCK, [VK_OEM_1] = KEY_SEMICOLON, [VK_LSHIFT] = KEY_LEFT_SHIFT, [VK_RSHIFT] = KEY_RIGHT_SHIFT, [VK_OEM_2] = KEY_SLASH, [VK_SPACE] = KEY_SPACE, [VK_TAB] = KEY_TAB, [VK_MENU] = KEY_ALT, [VK_CONTROL] = KEY_CONTROL, [VK_SHIFT] = KEY_SHIFT /*, KEY_WIN*/};
const int MinoKey2WinKey[KEY_COUNT] = {[KEY_A] = 'A', [KEY_B] = 'B', [KEY_C] = 'C', [KEY_D] = 'D', [KEY_E] = 'E', [KEY_F] = 'F', [KEY_G] = 'G', [KEY_H] = 'H', [KEY_I] = 'I', [KEY_J] = 'J', [KEY_K] = 'K', [KEY_L] = 'L', [KEY_M] = 'M', [KEY_N] = 'N', [KEY_O] = 'O', [KEY_p] = 'P', [KEY_Q] = 'Q', [KEY_R] = 'R', [KEY_S] = 'S', [KEY_T] = 'T', [KEY_U] = 'U', [KEY_V] = 'V', [KEY_W] = 'W', [KEY_X] = 'X', [KEY_Y] = 'Y', [KEY_Z] = 'Z', [KEY_LEFT_ALT] = VK_LMENU, [KEY_RIGHT_ALT] = VK_RMENU, [KEY_DOWN_ARROW] = VK_DOWN, [KEY_LEFT_ARROW] = VK_LEFT, [KEY_RIGHT_ARROW] = VK_RIGHT, [KEY_UP_ARROW] = VK_UP, [KEY_TILDE] = VK_OEM_3, [KEY_BACKSLASH] = VK_OEM_5, [KEY_BACKSPACE] = VK_BACK, [KEY_LEFT_BRACKET] = VK_OEM_4, [KEY_RIGHT_BRACKET] = VK_OEM_6, [KEY_CAPS_LOCK] = VK_CAPITAL, [KEY_COMMA] = VK_OEM_COMMA, [KEY_CONTEXT_MENU] = VK_APPS, [KEY_LEFT_CONTROL] = VK_LCONTROL, [KEY_RIGHT_CONTROL] = VK_RCONTROL, [KEY_DELETE] = VK_DELETE, [KEY_0] = '0', [KEY_1] = '1', [KEY_2] = '2', [KEY_3] = '3', [KEY_4] = '4', [KEY_5] = '5', [KEY_6] = '6', [KEY_7] = '7', [KEY_8] = '8', [KEY_9] = '9', [KEY_END] = VK_END, [KEY_ENTER] = VK_RETURN, [KEY_EQUAL] = VK_OEM_PLUS, [KEY_ESCAPE] = VK_ESCAPE, [KEY_F1] = VK_F1, [KEY_F2] = VK_F2, [KEY_F3] = VK_F3, [KEY_F4] = VK_F4, [KEY_F5] = VK_F5, [KEY_F6] = VK_F6, [KEY_F7] = VK_F7, [KEY_F8] = VK_F8, [KEY_F9] = VK_F9, [KEY_F10] = VK_F10, [KEY_F11] = VK_F11, [KEY_F12] = VK_F12, [KEY_HOME] = VK_HOME, [KEY_INSERT] = VK_INSERT, [KEY_LEFT_WIN] = VK_LWIN, [KEY_RIGHT_WIN] = VK_RWIN, [KEY_MINUS] = VK_OEM_MINUS, [KEY_NUMLOCK] = VK_NUMLOCK, [KEY_NP_0] = VK_NUMPAD0, [KEY_NP_1] = VK_NUMPAD1, [KEY_NP_2] = VK_NUMPAD2, [KEY_NP_3] = VK_NUMPAD3, [KEY_NP_4] = VK_NUMPAD4, [KEY_NP_5] = VK_NUMPAD5, [KEY_NP_6] = VK_NUMPAD6, [KEY_NP_7] = VK_NUMPAD7, [KEY_NP_8] = VK_NUMPAD8, [KEY_NP_9] = VK_NUMPAD9, [KEY_NP_ADD] = VK_ADD, [KEY_NP_DECIMAL] = VK_DECIMAL, [KEY_NP_DIVIDE] = VK_DIVIDE, [KEY_NP_ENTER] = VK_SEPARATOR, /*KEY_NP_EQUAL*/[KEY_NP_MULTIPLY] = VK_MULTIPLY, [KEY_NP_SUBTRACT] = VK_SUBTRACT, [KEY_PAGE_DOWN] = VK_NEXT, [KEY_PAGE_UP] = VK_PRIOR, [KEY_PAUSE] = VK_PAUSE, [KEY_PERIOD] = VK_OEM_PERIOD, [KEY_PRINT_SCREEN] = VK_SNAPSHOT, [KEY_QUOTE] = VK_OEM_7, [KEY_SCROLL_LOCK] = VK_SCROLL, [KEY_SEMICOLON] = VK_OEM_1, [KEY_LEFT_SHIFT] = VK_LSHIFT, [KEY_RIGHT_SHIFT] = VK_RSHIFT, [KEY_SLASH] = VK_OEM_2, [KEY_SPACE] = VK_SPACE, [KEY_TAB] = VK_TAB, [KEY_ALT] = VK_MENU, [KEY_CONTROL] = VK_CONTROL, [KEY_SHIFT] = VK_SHIFT /*, KEY_WIN*/};

static LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT msg, WPARAM wParam,
    LPARAM lParam) {
    Window *window = (Window *)GetWindowLongPtr(windowHandle, GWLP_USERDATA);

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = GetDC(windowHandle);
            BeginPaint(windowHandle, &ps);
            SwapBuffers(deviceContext);
            EndPaint(windowHandle, &ps);
            ReleaseDC(windowHandle, deviceContext);
        } break;

        case WM_CLOSE: {
            DestroyWindow(windowHandle);
        } break;

        case WM_LBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MOUSE_LEFT);
        } break;

        case WM_LBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MOUSE_LEFT);
        } break;

        case WM_RBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MOUSE_RIGHT);
        } break;

        case WM_RBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MOUSE_RIGHT);
        } break;

        case WM_MBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MOUSE_MIDDLE);
        } break;

        case WM_MBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MOUSE_MIDDLE);
        } break;

        case WM_XBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MOUSE_BACK : MOUSE_FORWARD);
        } break;

        case WM_XBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MOUSE_BACK : MOUSE_FORWARD);
        } break;

        case WM_MOUSEMOVE: {
            window->mouseX = LOWORD(lParam);
            window->mouseY = HIWORD(lParam);
        } break;

        case WM_MOUSEWHEEL: {
            window->scrollY += GET_WHEEL_DELTA_WPARAM(wParam);
        } break;

        case WM_MOUSEHWHEEL: {
            window->scrollX += GET_WHEEL_DELTA_WPARAM(wParam);
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

        case WM_ACTIVATEAPP: {
            XInputEnable(wParam);
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
    HINSTANCE instance = GetModuleHandle(nil);
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
        nil, nil, instance, nil);

    if (native->windowHandle == nil) {
        free(native);
        return false;
    }

    Gamepad *gamepads = window->gamepads = allocateN(Gamepad, 4);
    for (int i = 0; i < 4; i++) {
        gamepads[i].playerID = i;
    }

    SetWindowLongPtr(native->windowHandle, GWLP_USERDATA, (LONG_PTR)window);
    ShowWindow(native->windowHandle, SW_NORMAL);
    UpdateWindow(native->windowHandle);

    return true;
}

const int MinoGamepadButton2XinputButton[] = {[GAMEPAD_UP] = XINPUT_GAMEPAD_DPAD_UP, [GAMEPAD_DOWN] = XINPUT_GAMEPAD_DPAD_DOWN, [GAMEPAD_LEFT] = XINPUT_GAMEPAD_DPAD_LEFT, [GAMEPAD_RIGHT] = XINPUT_GAMEPAD_DPAD_RIGHT, [GAMEPAD_A] = XINPUT_GAMEPAD_A, [GAMEPAD_B] = XINPUT_GAMEPAD_B, [GAMEPAD_X] = XINPUT_GAMEPAD_X, [GAMEPAD_Y] = XINPUT_GAMEPAD_Y, [GAMEPAD_L1] = XINPUT_GAMEPAD_LEFT_SHOULDER, [GAMEPAD_L3] = XINPUT_GAMEPAD_LEFT_THUMB, [GAMEPAD_R1] = XINPUT_GAMEPAD_RIGHT_SHOULDER, [GAMEPAD_R3] = XINPUT_GAMEPAD_RIGHT_THUMB, [GAMEPAD_START] = XINPUT_GAMEPAD_START, [GAMEPAD_SELECT] = XINPUT_GAMEPAD_BACK};

static void updateGamepads(Window *window) {
    XINPUT_STATE xInputState;
    Gamepad *gamepad;

    for (int i = 0; i < 4; i++) {
        gamepad = &window->gamepads[i];
        gamepad->connected = XInputGetState(i, &xInputState) == ERROR_SUCCESS;
        if (window->gamepads[i].connected == false) {
            continue;
        }

        if (window->firstGamepad == nil) {
            window->firstGamepad = gamepad;
        }

        for (int j = 0; j < (int)len(MinoGamepadButton2XinputButton, int); j++) {
            int XinputButton = MinoGamepadButton2XinputButton[j];
            if ((xInputState.Gamepad.wButtons & XinputButton) == XinputButton) {
                gamepad->buttons = setBit(gamepad->buttons, j);
            } else {
                gamepad->buttons = unsetBit(gamepad->buttons, j);
            }
        }

        gamepad->axes[GAMEPAD_AXIS_LEFT_STICK_X] = clamp((float32)xInputState.Gamepad.sThumbLX / 0x7FFF, -1, 1);
        gamepad->axes[GAMEPAD_AXIS_LEFT_STICK_Y] = clamp((float32)xInputState.Gamepad.sThumbLY / 0x7FFF, -1, 1);
        gamepad->axes[GAMEPAD_AXIS_RIGHT_STICK_X] = clamp((float32)xInputState.Gamepad.sThumbRX / 0x7FFF, -1, 1);
        gamepad->axes[GAMEPAD_AXIS_RIGHT_STICK_Y] = clamp((float32)xInputState.Gamepad.sThumbRY / 0x7FFF, -1, 1);
        gamepad->axes[GAMEPAD_AXIS_LEFT_TRIGGER] = clamp((float32)xInputState.Gamepad.bLeftTrigger / 0xFF, 0, 1);
        gamepad->axes[GAMEPAD_AXIS_RIGHT_TRIGGER] = clamp((float32)xInputState.Gamepad.bRightTrigger / 0xFF, 0, 1);

        if (xInputState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            gamepad->buttons = (uint32)unsetBit(gamepad->buttons, GAMEPAD_L2);
        };
        if (xInputState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            gamepad->buttons = (uint32)unsetBit(gamepad->buttons, GAMEPAD_R2);
        };

        XINPUT_VIBRATION vibrations = {
            .wLeftMotorSpeed = (uint16)(gamepad->leftMotor * 0xFFFF),
            .wRightMotorSpeed = (uint16)(gamepad->rightMotor * 0xFFFF),
        };
        XInputSetState(i, &vibrations);
    }
}

bool WindowUpdate(Window *window) {
    resetInputState(window);
    updateGamepads(window);

    MSG message;
    while (PeekMessage(&message, nil, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    InvalidateRect(window->native->windowHandle, nil, false);
    return true;
}

void WindowClose(Window *window) {
    if (window->gamepads) {
        free(window->gamepads);
        window->gamepads = nil;
    }

    if (window->native == nil) return;

    if (window->native->windowHandle) {
        CloseWindow(window->native->windowHandle);
    }

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

bool GraphicsInit(Window *window) {
    if (window->native->glContext) {
        return false;
    }
    HDC deviceContext = GetDC(window->native->windowHandle);
    PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,

        .cRedBits = 8,
        .cRedShift = 16,
        .cAccumRedBits = 8,

        .cGreenBits = 8,
        .cGreenShift = 8,
        .cAccumGreenBits = 8,

        .cBlueBits = 8,
        .cBlueShift = 0,
        .cAccumBlueBits = 8,

        .cAlphaBits = 8,
        .cAlphaShift = 24,
        .cAccumAlphaBits = 8,

        .cDepthBits = 8,
        .cStencilBits = 8,
    };

    int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
    if (pixelFormat == 0) {
        ReleaseDC(window->native->windowHandle, deviceContext);
        return false;
    }

    if (SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor) == false) {
        ReleaseDC(window->native->windowHandle, deviceContext);
        return false;
    }

    DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor);

    window->native->glContext = wglCreateContext(deviceContext);
    if (window->native->glContext == nil) {
        ReleaseDC(window->native->windowHandle, deviceContext);
        return false;
    }
    ReleaseDC(window->native->windowHandle, deviceContext);

    glEnable(GL_DEPTH_TEST);
    return true;
}

void GraphicsClose(Window *window) {
    if (window->native->glContext == nil) return;
    wglDeleteContext(window->native->glContext);
    window->native->glContext = nil;
}

void GraphicsClear(Window *window) {
    HDC deviceContext = GetDC(window->native->windowHandle);
    wglMakeCurrent(deviceContext, window->native->glContext);
    ReleaseDC(window->native->windowHandle, deviceContext);
    glClear(GL_ALL_ATTRIB_BITS);
}

void GraphicsBegin(Window *window) {
    HDC deviceContext = GetDC(window->native->windowHandle);
    wglMakeCurrent(deviceContext, window->native->glContext);
    ReleaseDC(window->native->windowHandle, deviceContext);
    glBegin(GL_TRIANGLES);
}

void GraphicsEnd() {
    glEnd();
};

void GraphicsAddVertex(float32 vertex[3]) {
    glVertex3fv(vertex);
}

void GraphicsAddColor(Color color) {
    glColor4ub(color.R, color.G, color.B, color.A);
}

#elif defined(PLATFORM_Linux)

// Gotta clean this up so X11 can work. Now `MinoWindow` is the Window for Mino
// and `Window` is for X11's window ID.
#undef Window
#include <X11/Xutil.h>

static Key XKey2MinoKey(int key) {
    switch (key) {
        case XK_a: return KEY_A;
        case XK_b: return KEY_B;
        case XK_c: return KEY_C;
        case XK_d: return KEY_D;
        case XK_e: return KEY_E;
        case XK_f: return KEY_F;
        case XK_g: return KEY_G;
        case XK_h: return KEY_H;
        case XK_i: return KEY_I;
        case XK_j: return KEY_J;
        case XK_k: return KEY_K;
        case XK_l: return KEY_L;
        case XK_m: return KEY_M;
        case XK_n: return KEY_N;
        case XK_o: return KEY_O;
        case XK_p: return KEY_P;
        case XK_q: return KEY_Q;
        case XK_r: return KEY_R;
        case XK_s: return KEY_S;
        case XK_t: return KEY_T;
        case XK_u: return KEY_U;
        case XK_v: return KEY_V;
        case XK_w: return KEY_W;
        case XK_x: return KEY_X;
        case XK_y: return KEY_Y;
        case XK_z: return KEY_Z;
        case XK_Alt_L: return KEY_LEFT_ALT;
        case XK_Alt_R: return KEY_RIGHT_ALT;
        case XK_Down: return KEY_DOWN_ARROW;
        case XK_Left: return KEY_LEFT_ARROW;
        case XK_Right: return KEY_RIGHT_ARROW;
        case XK_Up: return KEY_UP_ARROW;
        case XK_grave: return KEY_TILDE;
        case XK_backslash: return KEY_BACKSLASH;
        case XK_BackSpace: return KEY_BACKSPACE;
        case XK_bracketleft: return KEY_LEFT_BRACKET;
        case XK_bracketright: return KEY_RIGHT_BRACKET;
        case XK_Caps_Lock: return KEY_CAPS_LOCK;
        case XK_comma: return KEY_COMMA;
        case XK_Menu: return KEY_CONTEXT_MENU;
        case XK_Control_L: return KEY_LEFT_CONTROL;
        case XK_Control_R: return KEY_RIGHT_CONTROL;
        case XK_Delete: return KEY_DELETE;
        case XK_0: return KEY_0;
        case XK_1: return KEY_1;
        case XK_2: return KEY_2;
        case XK_3: return KEY_3;
        case XK_4: return KEY_4;
        case XK_5: return KEY_5;
        case XK_6: return KEY_6;
        case XK_7: return KEY_7;
        case XK_8: return KEY_8;
        case XK_9: return KEY_9;
        case XK_End: return KEY_END;
        case XK_Return: return KEY_ENTER;
        case XK_equal: return KEY_EQUAL;
        case XK_Escape: return KEY_ESCAPE;
        case XK_F1: return KEY_F1;
        case XK_F2: return KEY_F2;
        case XK_F3: return KEY_F3;
        case XK_F4: return KEY_F4;
        case XK_F5: return KEY_F5;
        case XK_F6: return KEY_F6;
        case XK_F7: return KEY_F7;
        case XK_F8: return KEY_F8;
        case XK_F9: return KEY_F9;
        case XK_F10: return KEY_F10;
        case XK_F11: return KEY_F11;
        case XK_F12: return KEY_F12;
        case XK_Home: return KEY_HOME;
        case XK_Insert: return KEY_INSERT;
        case XK_Super_L: return KEY_LEFT_WIN;
        case XK_Super_R: return KEY_RIGHT_WIN;
        case XK_minus: return KEY_MINUS;
        case XK_Num_Lock: return KEY_NUMLOCK;
        case XK_KP_Insert: return KEY_NP_0;
        case XK_KP_End: return KEY_NP_1;
        case XK_KP_Down: return KEY_NP_2;
        case XK_KP_Page_Down: return KEY_NP_3;
        case XK_KP_Left: return KEY_NP_4;
        case XK_KP_Begin: return KEY_NP_5;
        case XK_KP_Right: return KEY_NP_6;
        case XK_KP_Home: return KEY_NP_7;
        case XK_KP_Up: return KEY_NP_8;
        case XK_KP_Page_Up: return KEY_NP_9;
        case XK_KP_Add: return KEY_NP_ADD;
        case XK_KP_Delete: return KEY_NP_DECIMAL;
        case XK_KP_Divide: return KEY_NP_DIVIDE;
        case XK_KP_Enter: return KEY_NP_ENTER;
        case XK_KP_Equal: return KEY_NP_EQUAL;
        case XK_KP_Multiply: return KEY_NP_MULTIPLY;
        case XK_KP_Subtract: return KEY_NP_SUBTRACT;
        case XK_Page_Down: return KEY_PAGE_DOWN;
        case XK_Page_Up: return KEY_PAGE_UP;
        case XK_Pause: return KEY_PAUSE;
        case XK_period: return KEY_PERIOD;
        case XK_Print: return KEY_PRINT_SCREEN;
        case XK_apostrophe: return KEY_QUOTE;
        case XK_Scroll_Lock: return KEY_SCROLL_LOCK;
        case XK_semicolon: return KEY_SEMICOLON;
        case XK_Shift_L: return KEY_LEFT_SHIFT;
        case XK_Shift_R: return KEY_RIGHT_SHIFT;
        case XK_slash: return KEY_SLASH;
        case XK_space: return KEY_SPACE;
        case XK_Tab: return KEY_TAB;
        default: return KEY_INVALID;
    }
}

// `XKey2MinoKey` needs to be before `#include <linux/input.h>` because it contains defines that collide with Mino.
#include <linux/input.h>
#include <linux/types.h>
#include <fcntl.h>
#include <libudev.h>
#include <string.h>
#include <unistd.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <sys/ioctl.h>

struct WindowNative {
    Display *xDisplay;
    Window xWindow;
    Atom deleteWindow;
    struct udev *udev;
    struct udev_monitor *monitor;
};

struct GamepadNative {
    int fileDescriptor;
    char *devicePath;
    struct input_absinfo analogInfos[GAMEPAD_AXIS_COUNT];
    struct ff_effect rumble;
};

const int minoAxis2EvdevAxis[GAMEPAD_AXIS_COUNT] = {
    [GAMEPAD_AXIS_LEFT_STICK_X] = ABS_X,
    [GAMEPAD_AXIS_LEFT_STICK_Y] = ABS_Y,
    [GAMEPAD_AXIS_LEFT_TRIGGER] = ABS_Z,
    [GAMEPAD_AXIS_RIGHT_STICK_X] = ABS_RX,
    [GAMEPAD_AXIS_RIGHT_STICK_Y] = ABS_RY,
    [GAMEPAD_AXIS_RIGHT_TRIGGER] = ABS_RZ,
};

const int evdevAxis2MinoAxis[GAMEPAD_AXIS_COUNT] = {
    [ABS_X] = GAMEPAD_AXIS_LEFT_STICK_X,
    [ABS_Y] = GAMEPAD_AXIS_LEFT_STICK_Y,
    [ABS_Z] = GAMEPAD_AXIS_LEFT_TRIGGER,
    [ABS_RX] = GAMEPAD_AXIS_RIGHT_STICK_X,
    [ABS_RY] = GAMEPAD_AXIS_RIGHT_STICK_Y,
    [ABS_RZ] = GAMEPAD_AXIS_RIGHT_TRIGGER,
};

GamepadButton evdevButton2MinoButton(int button) {
    switch (button) {
        case BTN_A: return GAMEPAD_A;
        case BTN_B: return GAMEPAD_B;
        case BTN_X: return GAMEPAD_X;
        case BTN_Y: return GAMEPAD_Y;
        case BTN_START: return GAMEPAD_START;
        case BTN_SELECT: return GAMEPAD_SELECT;
        case BTN_TL: return GAMEPAD_L1;
        case BTN_TR: return GAMEPAD_R1;
        case BTN_TL2: return GAMEPAD_L2;
        case BTN_TR2: return GAMEPAD_R2;
        case BTN_THUMBL: return GAMEPAD_L3;
        case BTN_THUMBR: return GAMEPAD_R3;
        case BTN_MODE: return GAMEPAD_HOME;
        default: return GAMEPAD_BUTTON_INVALID;
    }
};

const struct input_absinfo fallbackInfos = {
    .minimum = -1,
    .maximum = 1,
};

static void connectController(MinoWindow *window, const char *devicePath) {
    const int devicePathLen = strlen(devicePath) + 1;
    Gamepad *gamepad = nil;
    int fd = open(devicePath, O_RDWR | O_NONBLOCK);
    if (fd < 0) return;

    // Attempt to reconnect controller with matching file name.
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false && strcmp(gamepad->native->devicePath, devicePath) == 0) {
            goto foundGamepad;
        }
    }

    // If no controllers have this file name, attempt to reconnect
    // with any disconnected controller.
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false) {
            free(gamepad->native->devicePath);
            goto replaceGamepad;
        };
    }

    // Finally, if no controllers are disconnected, just connect a
    // new controller.
    {
        GamepadListPush(&window->gamepads, (Gamepad){});
        gamepad = GamepadListGet(&window->gamepads, window->gamepads.len - 1);
        gamepad->native = malloc(sizeof(GamepadNative));
        memset(gamepad->native, 0, sizeof(GamepadNative));
    }

replaceGamepad:
    gamepad->native->devicePath = malloc(devicePathLen + 1);
    gamepad->native->devicePath[devicePathLen] = '\0';
    gamepad->native->rumble = (struct ff_effect){
        .id = -1,
        .type = FF_RUMBLE,
    };

    memcpy(gamepad->native->devicePath, devicePath, devicePathLen);

foundGamepad:;
    for (GamepadAxis i = 0; i < GAMEPAD_AXIS_COUNT; i++) {
        if (ioctl(fd, EVIOCGABS(minoAxis2EvdevAxis[i]), &gamepad->native->analogInfos[i])) {
            gamepad->native->analogInfos[i] = fallbackInfos;
        };
    }
    gamepad->native->fileDescriptor = fd;
    gamepad->connected = true;
    
}

static void disconnectController(MinoWindow *window, const char *devicePath) {
    Gamepad *gamepad = nil;
    for (int i = 0; i < window->gamepads.len; i++) {
        Gamepad *temp = GamepadListGet(&window->gamepads, i);
        if (temp->connected == true && strcmp(temp->native->devicePath, devicePath) == 0) {
            gamepad = temp;
            break;
        }
    }
    if (gamepad == nil) return;

    close(gamepad->native->fileDescriptor);
    gamepad->native->fileDescriptor = 0;
    gamepad->connected = false;
    // just zero these out to be safe so, when a controller reconnects, none of
    // the buttons or sticks are still held 
    gamepad->buttons = 0;
    for (int i = 0; i < GAMEPAD_AXIS_COUNT; i++) {
        gamepad->axes[i] = 0;
    }
}

bool WindowInit(MinoWindow *window, WindowConfig config) {
    Display *xDisplay = XOpenDisplay(nil);
    if (xDisplay == nil) {
        return false;
    }

    int screenID = DefaultScreen(xDisplay);

    Window xWindow = XCreateSimpleWindow(
        xDisplay,
        RootWindow(xDisplay, screenID),
        10, 10, config.width, config.height, 1,
        BlackPixel(xDisplay, screenID),
        WhitePixel(xDisplay, screenID));

    Atom deleteWindowAtom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(xDisplay, xWindow, &deleteWindowAtom, 1);

    XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

    XMapWindow(xDisplay, xWindow);
    XSync(xDisplay, xWindow);

    struct udev *udev = udev_new();
    if (udev == nil) {
        println("Warning: unable to open udev");
        XDestroyWindow(xDisplay, xWindow);
        return false;
    }

    window->native = allocate(WindowNative);
    *window->native = (WindowNative){
        .xDisplay = xDisplay,
        .xWindow = xWindow,
        .deleteWindow = deleteWindowAtom,
        .udev = udev,
    };
    GamepadListInit(&window->gamepads, 0, 4);

    struct udev_enumerate *devices = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(devices, "input");
    udev_enumerate_add_match_property(devices, "ID_INPUT_JOYSTICK", "1");
    udev_enumerate_scan_devices(devices);
    struct udev_list_entry *entry, *listEntry = udev_enumerate_get_list_entry(devices);
    struct udev_device *device = NULL;
    udev_list_entry_foreach(entry, listEntry) {
        const char *sysPath = udev_list_entry_get_name(entry);
        device = udev_device_new_from_syspath(udev, sysPath);

        const char *devicePath = udev_device_get_devnode(device);
        if (hasPrefix(devicePath, "/dev/input/event") == false) goto end;

        connectController(window, devicePath);
    end:
        udev_device_unref(device);
    }
    udev_enumerate_unref(devices);

    struct udev_monitor *monitor = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "input", nil);
    udev_monitor_enable_receiving(monitor);
    window->native->monitor = monitor;

    return true;
}

static void refreshControllers(MinoWindow *window) {
    fd_set fileDescriptors;
    FD_ZERO(&fileDescriptors);
    FD_SET(udev_monitor_get_fd(window->native->monitor), &fileDescriptors);
    while (true) {
        if (FD_ISSET(udev_monitor_get_fd(window->native->monitor), &fileDescriptors)) {
            struct udev_device *device;
            const char *action;
            device = udev_monitor_receive_device(window->native->monitor);
            if (device == nil) {
                return;
            }

            const char *isJoystick = udev_device_get_property_value(device, "ID_INPUT_JOYSTICK");
            if (isJoystick == nil || strcmp(isJoystick, "1") != 0) goto end;

            const char *devicePath = udev_device_get_devnode(device);
            if (hasPrefix(devicePath, "/dev/input/event") == false) {
                goto end;
            }

            action = udev_device_get_action(device);
            if (strcmp(action, "add") == 0) {
                connectController(window, devicePath);
            } else if (strcmp(action, "remove") == 0) {
                disconnectController(window, devicePath);
            }
        end:
            udev_device_unref(device);
        }
    }
}

static void setAxis(Gamepad *gamepad, struct input_event event) {
    if (event.code == ABS_HAT0X) {
        if (event.value >= 1) {
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_LEFT);
            gamepad->buttons = setBit(gamepad->buttons, GAMEPAD_RIGHT);
        } else if (event.value <= -1) {
            gamepad->buttons = setBit(gamepad->buttons, GAMEPAD_LEFT);
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_RIGHT);
        } else {
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_LEFT);
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_RIGHT);
        }
        return;
    } else if (event.code == ABS_HAT0Y) {
        if (event.value >= 1) {
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_DOWN);
            gamepad->buttons = setBit(gamepad->buttons, GAMEPAD_UP);
        } else if (event.value <= -1) {
            gamepad->buttons = setBit(gamepad->buttons, GAMEPAD_DOWN);
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_UP);
        } else {
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_DOWN);
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_UP);
        }
        return;
    }
    if (event.code >= GAMEPAD_AXIS_COUNT) return;  // unsupported axis.
    GamepadAxis axis = evdevAxis2MinoAxis[event.code];
    struct input_absinfo info = gamepad->native->analogInfos[axis];
    if (axis == GAMEPAD_AXIS_LEFT_STICK_Y || axis == GAMEPAD_AXIS_RIGHT_STICK_Y) event.value *= -1;
    if (axis == GAMEPAD_AXIS_LEFT_TRIGGER || axis == GAMEPAD_AXIS_RIGHT_TRIGGER) {
        gamepad->axes[axis] = clamp((float32)(event.value - info.minimum) / (float32)(info.maximum - info.minimum), 0, 1);
    } else {
        gamepad->axes[axis] = clamp((float32)(event.value - info.minimum) / (float32)(info.maximum - info.minimum) * 2.0 - 1.0, -1, 1);
    }
    if (axis == GAMEPAD_AXIS_LEFT_TRIGGER) {
        if (event.value > info.flat)
            gamepad->buttons = setBit(gamepad->buttons, GAMEPAD_L2);
        else
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_L2);
    } else if (axis == GAMEPAD_AXIS_RIGHT_TRIGGER) {
        if (event.value > info.flat)
            gamepad->buttons = setBit(gamepad->buttons, GAMEPAD_R2);
        else
            gamepad->buttons = unsetBit(gamepad->buttons, GAMEPAD_R2);
    }
}

static void setButton(Gamepad *gamepad, struct input_event event) {
    GamepadButton button = evdevButton2MinoButton(event.code);
    if (button == GAMEPAD_BUTTON_INVALID) return;
    if (event.value > 0) {
        gamepad->buttons = setBit(gamepad->buttons, button);
    } else {
        gamepad->buttons = unsetBit(gamepad->buttons, button);
    }
}

static void setRumble(Gamepad *gamepad) {
    gamepad->native->rumble.type = FF_RUMBLE;
    gamepad->native->rumble.replay.length = 0xFFFF;
    gamepad->native->rumble.u.rumble = (struct ff_rumble_effect){
        .strong_magnitude = (int32)(gamepad->leftMotor * 0xFFFF),
        .weak_magnitude = (int32)(gamepad->rightMotor * 0xFFFF),
    };
    if (ioctl(gamepad->native->fileDescriptor, EVIOCSFF, &gamepad->native->rumble) < 0) {
        gamepad->native->rumble.id = -1;
        if (ioctl(gamepad->native->fileDescriptor, EVIOCSFF, &gamepad->native->rumble) < 0) {
            return;
        }
    };
    struct input_event event = {
        .type = EV_FF,
        .code = gamepad->native->rumble.id,
        .value = 1,
    };
    if(write(gamepad->native->fileDescriptor, &event, sizeof(event)) < 0) {
        // This if statement is to remove a warning. We don't really need to do
        // anything if write fails anyway. We'll just ignore it.
    };
}

static void updateControllers(MinoWindow *window) {
    Gamepad *gamepad;
    struct input_event events[8];
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false) continue;
        while (true) {
            int32 bytesRead = read(gamepad->native->fileDescriptor, &events, sizeof(events));
            if (bytesRead == -1) break;
            for (int32 i = 0; (i + 1) * (int32)sizeof(struct input_event) < bytesRead; i++) {
                switch (events[i].type) {
                    case EV_ABS: {
                        setAxis(gamepad, events[i]);
                    } break;
                    case EV_KEY: {
                        setButton(gamepad, events[i]);
                    } break;
                }
            }
        }
        if (gamepad->leftMotor != gamepad->pLeftMotor || gamepad->rightMotor != gamepad->pRightMotor) {
            setRumble(gamepad);
            gamepad->pLeftMotor = gamepad->leftMotor;
            gamepad->pRightMotor = gamepad->rightMotor;
        }
    }
}

bool WindowUpdate(MinoWindow *window) {
    XEvent event;
    WindowNative *native = window->native;

    resetInputState(window);
    refreshControllers(window);
    updateControllers(window);
    while (XPending(native->xDisplay)) {
        XNextEvent(native->xDisplay, &event);
        switch (event.type) {
            case MotionNotify: {
                window->mouseX = event.xmotion.x;
                window->mouseY = event.xmotion.y;
            } break;

            case ButtonPress: {
                switch (event.xbutton.button) {
                    case 1:
                        window->mousePressed = setBit(window->mousePressed, MOUSE_LEFT);
                        break;
                    case 2:
                        window->mousePressed = setBit(window->mousePressed, MOUSE_MIDDLE);
                        break;
                    case 3:
                        window->mousePressed = setBit(window->mousePressed, MOUSE_RIGHT);
                        break;
                    case 4:
                        window->scrollY++;
                        break;
                    case 5:
                        window->scrollY--;
                        break;
                    case 6:
                        window->scrollX--;
                        break;
                    case 7:
                        window->scrollX++;
                        break;
                    case 8:
                        window->mousePressed = setBit(window->mousePressed, MOUSE_BACK);
                        break;
                    case 9:
                        window->mousePressed = setBit(window->mousePressed, MOUSE_FORWARD);
                        break;
                }
                break;
            }

            case ButtonRelease: {
                switch (event.xbutton.button) {
                    case 1:
                        window->mousePressed = unsetBit(window->mousePressed, MOUSE_LEFT);
                        break;
                    case 2:
                        window->mousePressed = unsetBit(window->mousePressed, MOUSE_MIDDLE);
                        break;
                    case 3:
                        window->mousePressed = unsetBit(window->mousePressed, MOUSE_RIGHT);
                        break;
                    case 8:
                        window->mousePressed = unsetBit(window->mousePressed, MOUSE_BACK);
                        break;
                    case 9:
                        window->mousePressed = unsetBit(window->mousePressed, MOUSE_FORWARD);
                        break;
                }
            } break;

            case KeyPress:
            case KeyRelease: {
                KeySym keySymbol = XkbKeycodeToKeysym(native->xDisplay, event.xkey.keycode, 0, 0);
                Key key = XKey2MinoKey(keySymbol);
                if (key == KEY_INVALID) break;

                window->keyPressed[key] = event.type == KeyPress;

                if (key == KEY_LEFT_CONTROL || key == KEY_RIGHT_CONTROL)
                    window->keyPressed[KEY_CONTROL] = (window->keyPressed[KEY_LEFT_CONTROL] || window->keyPressed[KEY_RIGHT_CONTROL]);
                else if (key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT)
                    window->keyPressed[KEY_SHIFT] = (window->keyPressed[KEY_LEFT_SHIFT] || window->keyPressed[KEY_RIGHT_SHIFT]);
                else if (key == KEY_LEFT_WIN || key == KEY_RIGHT_WIN)
                    window->keyPressed[KEY_WIN] = (window->keyPressed[KEY_LEFT_WIN] || window->keyPressed[KEY_RIGHT_WIN]);
                else if (key == KEY_LEFT_ALT || key == KEY_RIGHT_ALT)
                    window->keyPressed[KEY_ALT] = (window->keyPressed[KEY_LEFT_ALT] || window->keyPressed[KEY_RIGHT_ALT]);

                window->keyModifier =
                    (window->keyPressed[KEY_WIN] ? MOD_WIN : 0) |
                    (window->keyPressed[KEY_ALT] ? MOD_ALT : 0) |
                    (window->keyPressed[KEY_SHIFT] ? MOD_SHIFT : 0) |
                    (window->keyPressed[KEY_CONTROL] ? MOD_CONTROL : 0) |
                    (bitSet(event.xkey.state, LockMapIndex) ? MOD_CAPS_LOCK : 0) |
                    (bitSet(event.xkey.state, Mod2MapIndex) ? MOD_NUM_LOCK : 0) |
                    (bitSet(event.xkey.state, Mod5MapIndex) ? MOD_SCROLL_LOCK : 0);

                if (event.type == KeyPress) {
                    char buffer[sizeof(rune)];
                    if (XLookupString(&event.xkey, buffer, sizeof(buffer), &keySymbol, NULL) > 0) {
                        mbstowcs(&window->keyChar, buffer, 1);
                    };
                }
            } break;

            case ClientMessage: {
                if (event.xclient.data.l[0] == (long)native->deleteWindow) {
                    return false;
                }
            } break;
        }
    }
    return true;
}

void WindowClose(MinoWindow *window) {
    XDestroyWindow(window->native->xDisplay, window->native->xWindow);
    XCloseDisplay(window->native->xDisplay);
    Gamepad *gamepad;
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected) {
            close(gamepad->native->fileDescriptor);
        }
        if (gamepad->native != nil) {
            if (gamepad->native->devicePath != nil) {
                free(gamepad->native->devicePath);
                gamepad->native->devicePath = nil;
            }
            free(gamepad->native);
            gamepad->native = nil;
        }
    }
    GamepadListFree(&window->gamepads);
    udev_monitor_unref(window->native->monitor);
    udev_unref(window->native->udev);
    free(window->native);
}

#include <time.h>

int64 WindowTime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + (time.tv_nsec / 1000000);
}

void WindowSleep(int64 milliseconds) {
    struct timespec time;
    time.tv_sec = milliseconds / 1000;
    time.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&time, nil);
}

#endif
