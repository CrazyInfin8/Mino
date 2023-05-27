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
    for (int i = 0; i < Key_Count; i++) {
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
        for (int j = 0; j < GamepadAxis_Count; j++) {
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

const Key WinKey2MinoKey[256] = {['A'] = Key_A, ['B'] = Key_B, ['C'] = Key_C, ['D'] = Key_D, ['E'] = Key_E, ['F'] = Key_F, ['G'] = Key_G, ['H'] = Key_H, ['I'] = Key_I, ['J'] = Key_J, ['K'] = Key_K, ['L'] = Key_L, ['M'] = Key_M, ['N'] = Key_N, ['O'] = Key_O, ['P'] = Key_P, ['Q'] = Key_Q, ['R'] = Key_R, ['S'] = Key_S, ['T'] = Key_T, ['U'] = Key_U, ['V'] = Key_V, ['W'] = Key_W, ['X'] = Key_X, ['Y'] = Key_Y, ['Z'] = Key_Z, [VK_LMENU] = Key_LeftAlt, [VK_RMENU] = Key_RightAlt, [VK_DOWN] = Key_DownArrow, [VK_LEFT] = Key_LeftArrow, [VK_RIGHT] = Key_RightArrow, [VK_UP] = Key_UpArrow, [VK_OEM_3] = Key_Tilde, [VK_OEM_5] = Key_Backslash, [VK_BACK] = Key_Backspace, [VK_OEM_4] = Key_LeftBracket, [VK_OEM_6] = Key_RightBracket, [VK_CAPITAL] = Key_CapsLock, [VK_OEM_COMMA] = Key_Comma, [VK_APPS] = Key_Menu, [VK_LCONTROL] = Key_LeftCtrl, [VK_RCONTROL] = Key_RightCtrl, [VK_DELETE] = Key_Delete, ['0'] = Key_0, ['1'] = Key_1, ['2'] = Key_2, ['3'] = Key_3, ['4'] = Key_4, ['5'] = Key_5, ['6'] = Key_6, ['7'] = Key_7, ['8'] = Key_8, ['9'] = Key_9, [VK_END] = Key_End, [VK_RETURN] = Key_Enter, [VK_OEM_PLUS] = Key_Equal, [VK_ESCAPE] = Key_Escape, [VK_F1] = Key_F1, [VK_F2] = Key_F2, [VK_F3] = Key_F3, [VK_F4] = Key_F4, [VK_F5] = Key_F5, [VK_F6] = Key_F6, [VK_F7] = Key_F7, [VK_F8] = Key_F8, [VK_F9] = Key_F9, [VK_F10] = Key_F10, [VK_F11] = Key_F11, [VK_F12] = Key_F12, [VK_HOME] = Key_Home, [VK_INSERT] = Key_Insert, [VK_LWIN] = Key_LeftWin, [VK_RWIN] = Key_RightWin, [VK_OEM_MINUS] = Key_Minus, [VK_NUMLOCK] = Key_NumLock, [VK_NUMPAD0] = Key_NumPad0, [VK_NUMPAD1] = Key_NumPad1, [VK_NUMPAD2] = Key_NumPad2, [VK_NUMPAD3] = Key_NumPad3, [VK_NUMPAD4] = Key_NumPad4, [VK_NUMPAD5] = Key_NumPad5, [VK_NUMPAD6] = Key_NumPad6, [VK_NUMPAD7] = Key_NumPad7, [VK_NUMPAD8] = Key_NumPad8, [VK_NUMPAD9] = Key_NumPad9, [VK_ADD] = Key_NumPadAdd, [VK_DECIMAL] = Key_NumPadDecimal, [VK_DIVIDE] = Key_NumPadDivide, [VK_SEPARATOR] = Key_NumPadEnter, /*Key_NP_EQUAL*/[VK_MULTIPLY] = Key_NumPadMultiply, [VK_SUBTRACT] = Key_NumPadSubtract, [VK_NEXT] = Key_PageDown, [VK_PRIOR] = Key_PageUp, [VK_PAUSE] = Key_Pause, [VK_OEM_PERIOD] = Key_Period, [VK_SNAPSHOT] = Key_PrintScreen, [VK_OEM_7] = Key_Quote, [VK_SCROLL] = Key_ScrollLock, [VK_OEM_1] = Key_Semicolon, [VK_LSHIFT] = Key_LeftShift, [VK_RSHIFT] = Key_RightShift, [VK_OEM_2] = Key_Slash, [VK_SPACE] = Key_Space, [VK_TAB] = Key_Tab, [VK_MENU] = Key_Alt, [VK_CONTROL] = Key_Ctrl, [VK_SHIFT] = Key_Shift /*, Key_WIN*/};
const int MinoKey2WinKey[Key_Count] = {[Key_A] = 'A', [Key_B] = 'B', [Key_C] = 'C', [Key_D] = 'D', [Key_E] = 'E', [Key_F] = 'F', [Key_G] = 'G', [Key_H] = 'H', [Key_I] = 'I', [Key_J] = 'J', [Key_K] = 'K', [Key_L] = 'L', [Key_M] = 'M', [Key_N] = 'N', [Key_O] = 'O', [Key_P] = 'P', [Key_Q] = 'Q', [Key_R] = 'R', [Key_S] = 'S', [Key_T] = 'T', [Key_U] = 'U', [Key_V] = 'V', [Key_W] = 'W', [Key_X] = 'X', [Key_Y] = 'Y', [Key_Z] = 'Z', [Key_LeftAlt] = VK_LMENU, [Key_RightAlt] = VK_RMENU, [Key_DownArrow] = VK_DOWN, [Key_LeftArrow] = VK_LEFT, [Key_RightArrow] = VK_RIGHT, [Key_UpArrow] = VK_UP, [Key_Tilde] = VK_OEM_3, [Key_Backslash] = VK_OEM_5, [Key_Backspace] = VK_BACK, [Key_LeftBracket] = VK_OEM_4, [Key_RightBracket] = VK_OEM_6, [Key_CapsLock] = VK_CAPITAL, [Key_Comma] = VK_OEM_COMMA, [Key_Menu] = VK_APPS, [Key_LeftCtrl] = VK_LCONTROL, [Key_RightCtrl] = VK_RCONTROL, [Key_Delete] = VK_DELETE, [Key_0] = '0', [Key_1] = '1', [Key_2] = '2', [Key_3] = '3', [Key_4] = '4', [Key_5] = '5', [Key_6] = '6', [Key_7] = '7', [Key_8] = '8', [Key_9] = '9', [Key_End] = VK_END, [Key_Enter] = VK_RETURN, [Key_Equal] = VK_OEM_PLUS, [Key_Escape] = VK_ESCAPE, [Key_F1] = VK_F1, [Key_F2] = VK_F2, [Key_F3] = VK_F3, [Key_F4] = VK_F4, [Key_F5] = VK_F5, [Key_F6] = VK_F6, [Key_F7] = VK_F7, [Key_F8] = VK_F8, [Key_F9] = VK_F9, [Key_F10] = VK_F10, [Key_F11] = VK_F11, [Key_F12] = VK_F12, [Key_Home] = VK_HOME, [Key_Insert] = VK_INSERT, [Key_LeftWin] = VK_LWIN, [Key_RightWin] = VK_RWIN, [Key_Minus] = VK_OEM_MINUS, [Key_NumLock] = VK_NUMLOCK, [Key_NumPad0] = VK_NUMPAD0, [Key_NumPad1] = VK_NUMPAD1, [Key_NumPad2] = VK_NUMPAD2, [Key_NumPad3] = VK_NUMPAD3, [Key_NumPad4] = VK_NUMPAD4, [Key_NumPad5] = VK_NUMPAD5, [Key_NumPad6] = VK_NUMPAD6, [Key_NumPad7] = VK_NUMPAD7, [Key_NumPad8] = VK_NUMPAD8, [Key_NumPad9] = VK_NUMPAD9, [Key_NumPadAdd] = VK_ADD, [Key_NumPadDecimal] = VK_DECIMAL, [Key_NumPadDivide] = VK_DIVIDE, [Key_NumPadEnter] = VK_SEPARATOR, /*Key_NP_EQUAL*/[Key_NumPadMultiply] = VK_MULTIPLY, [Key_NumPadSubtract] = VK_SUBTRACT, [Key_PageDown] = VK_NEXT, [Key_PageUp] = VK_PRIOR, [Key_Pause] = VK_PAUSE, [Key_Period] = VK_OEM_PERIOD, [Key_PrintScreen] = VK_SNAPSHOT, [Key_Quote] = VK_OEM_7, [Key_ScrollLock] = VK_SCROLL, [Key_Semicolon] = VK_OEM_1, [Key_LeftShift] = VK_LSHIFT, [Key_RightShift] = VK_RSHIFT, [Key_Slash] = VK_OEM_2, [Key_Space] = VK_SPACE, [Key_Tab] = VK_TAB, [Key_Alt] = VK_MENU, [Key_Ctrl] = VK_CONTROL, [Key_Shift] = VK_SHIFT /*, Key_WIN*/};

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

        case WM_SIZE: {
            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);

            if (window->native->glContext) {
                GraphicsMakeCurrent(window);

                glViewport(
                    0, 0,
                    window->width,
                    window->height);
            }
        } break;

        case WM_CLOSE: {
            DestroyWindow(windowHandle);
        } break;

        case WM_LBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Left);
        } break;

        case WM_LBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MouseButton_Left);
        } break;

        case WM_RBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Right);
        } break;

        case WM_RBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MouseButton_Right);
        } break;

        case WM_MBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Middle);
        } break;

        case WM_MBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MouseButton_Middle);
        } break;

        case WM_XBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MouseButton_Back : MouseButton_Forward);
        } break;

        case WM_XBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MouseButton_Back : MouseButton_Forward);
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
            window->KeyMod =
                (GetKeyState(VK_CONTROL) & 0x8000 ? KeyMod_Ctrl : 0) |
                (GetKeyState(VK_SHIFT) & 0x8000 ? KeyMod_Shift : 0) |
                (GetKeyState(VK_MENU) & 0x8000 ? KeyMod_Alt : 0) |
                ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000 ? KeyMod_Win : 0) |
                (GetKeyState(VK_CAPITAL) & 0x0001 ? KeyMod_CapsLock : 0) |
                (GetKeyState(VK_SCROLL) & 0x0001 ? KeyMod_ScrollLock : 0) |
                (GetKeyState(VK_NUMLOCK) & 0x0001 ? KeyMod_NumLock : 0);
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

int64 minPeriod = 0;
bool WindowInit(Window *window, WindowConfig config) {
    if (minPeriod == 0) {
        TIMECAPS timeCapabilities;
        if (timeGetDevCaps(&timeCapabilities, sizeof(TIMECAPS)) == MMSYSERR_NOERROR) {
            minPeriod = timeCapabilities.wPeriodMin;
        };
    }

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

    GamepadListInit(&window->gamepads, 4, 4);
    for (int i = 0; i < window->gamepads.len; i++) {
        Gamepad *gamepad = GamepadListGet(&window->gamepads, i);
        gamepad->playerID = i;
    }

    SetWindowLongPtr(native->windowHandle, GWLP_USERDATA, (LONG_PTR)window);
    ShowWindow(native->windowHandle, SW_NORMAL);
    UpdateWindow(native->windowHandle);

    return true;
}

const int MinoGamepadButton2XinputButton[] = {[GamepadButton_Up] = XINPUT_GAMEPAD_DPAD_UP, [GamepadButton_Down] = XINPUT_GAMEPAD_DPAD_DOWN, [GamepadButton_Left] = XINPUT_GAMEPAD_DPAD_LEFT, [GamepadButton_Right] = XINPUT_GAMEPAD_DPAD_RIGHT, [GamepadButton_A] = XINPUT_GAMEPAD_A, [GamepadButton_B] = XINPUT_GAMEPAD_B, [GamepadButton_X] = XINPUT_GAMEPAD_X, [GamepadButton_Y] = XINPUT_GAMEPAD_Y, [GamepadButton_L1] = XINPUT_GAMEPAD_LEFT_SHOULDER, [GamepadButton_L3] = XINPUT_GAMEPAD_LEFT_THUMB, [GamepadButton_R1] = XINPUT_GAMEPAD_RIGHT_SHOULDER, [GamepadButton_R3] = XINPUT_GAMEPAD_RIGHT_THUMB, [GamepadButton_Start] = XINPUT_GAMEPAD_START, [GamepadButton_Select] = XINPUT_GAMEPAD_BACK};

static void updateGamepads(Window *window) {
    XINPUT_STATE xInputState;
    Gamepad *gamepad;

    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        gamepad->connected = XInputGetState(i, &xInputState) == ERROR_SUCCESS;
        if (gamepad->connected == false) {
            continue;
        }

        for (int j = 0; j < (int)len(MinoGamepadButton2XinputButton, int); j++) {
            int XinputButton = MinoGamepadButton2XinputButton[j];
            if ((xInputState.Gamepad.wButtons & XinputButton) == XinputButton) {
                gamepad->buttons = setBit(gamepad->buttons, j);
            } else {
                gamepad->buttons = unsetBit(gamepad->buttons, j);
            }
        }

        gamepad->axes[GamepadAxis_LeftStickX] = clamp((float32)xInputState.Gamepad.sThumbLX / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_LeftStickY] = clamp((float32)xInputState.Gamepad.sThumbLY / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_RightStickX] = clamp((float32)xInputState.Gamepad.sThumbRX / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_RightStickY] = clamp((float32)xInputState.Gamepad.sThumbRY / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_LeftTrigger] = clamp((float32)xInputState.Gamepad.bLeftTrigger / 0xFF, 0, 1);
        gamepad->axes[GamepadAxis_RightTrigger] = clamp((float32)xInputState.Gamepad.bRightTrigger / 0xFF, 0, 1);

        if (xInputState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            gamepad->buttons = (uint32)unsetBit(gamepad->buttons, GamepadButton_L2);
        };
        if (xInputState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            gamepad->buttons = (uint32)unsetBit(gamepad->buttons, GamepadButton_R2);
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
    GamepadListFree(&window->gamepads);
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
    const int64 startTime = WindowTime();
    if (milliseconds == 0) return;
    if (minPeriod > 0 && milliseconds - 1 > minPeriod) {
        timeBeginPeriod(minPeriod);
        Sleep(milliseconds - 1);
        timeEndPeriod(minPeriod);
    }
    while (WindowTime() < startTime + milliseconds) {
        // Spin-lock rest of time to precisely match milliseconds.
    }
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

void GraphicsMakeCurrent(Window *window) {
    HDC deviceContext = GetDC(window->native->windowHandle);
    wglMakeCurrent(deviceContext, window->native->glContext);
    ReleaseDC(window->native->windowHandle, deviceContext);
}

#elif defined(PLATFORM_Linux)

// Gotta clean this up so X11 can work. Now `MinoWindow` is the Window for Mino
// and `Window` is for X11's window ID.
#undef Window
#include <X11/Xutil.h>

static Key XKey2MinoKey(int key) {
    switch (key) {
        case XK_a: return Key_A;
        case XK_b: return Key_B;
        case XK_c: return Key_C;
        case XK_d: return Key_D;
        case XK_e: return Key_E;
        case XK_f: return Key_F;
        case XK_g: return Key_G;
        case XK_h: return Key_H;
        case XK_i: return Key_I;
        case XK_j: return Key_J;
        case XK_k: return Key_K;
        case XK_l: return Key_L;
        case XK_m: return Key_M;
        case XK_n: return Key_N;
        case XK_o: return Key_O;
        case XK_p: return Key_P;
        case XK_q: return Key_Q;
        case XK_r: return Key_R;
        case XK_s: return Key_S;
        case XK_t: return Key_T;
        case XK_u: return Key_U;
        case XK_v: return Key_V;
        case XK_w: return Key_W;
        case XK_x: return Key_X;
        case XK_y: return Key_Y;
        case XK_z: return Key_Z;
        case XK_Alt_L: return Key_LeftAlt;
        case XK_Alt_R: return Key_RightAlt;
        case XK_Down: return Key_DownArrow;
        case XK_Left: return Key_LeftArrow;
        case XK_Right: return Key_RightArrow;
        case XK_Up: return Key_UpArrow;
        case XK_grave: return Key_Tilde;
        case XK_backslash: return Key_Backslash;
        case XK_BackSpace: return Key_Backspace;
        case XK_bracketleft: return Key_LeftBracket;
        case XK_bracketright: return Key_RightBracket;
        case XK_Caps_Lock: return Key_CapsLock;
        case XK_comma: return Key_Comma;
        case XK_Menu: return Key_Menu;
        case XK_Control_L: return Key_LeftCtrl;
        case XK_Control_R: return Key_RightCtrl;
        case XK_Delete: return Key_Delete;
        case XK_0: return Key_0;
        case XK_1: return Key_1;
        case XK_2: return Key_2;
        case XK_3: return Key_3;
        case XK_4: return Key_4;
        case XK_5: return Key_5;
        case XK_6: return Key_6;
        case XK_7: return Key_7;
        case XK_8: return Key_8;
        case XK_9: return Key_9;
        case XK_End: return Key_End;
        case XK_Return: return Key_Enter;
        case XK_equal: return Key_Equal;
        case XK_Escape: return Key_Escape;
        case XK_F1: return Key_F1;
        case XK_F2: return Key_F2;
        case XK_F3: return Key_F3;
        case XK_F4: return Key_F4;
        case XK_F5: return Key_F5;
        case XK_F6: return Key_F6;
        case XK_F7: return Key_F7;
        case XK_F8: return Key_F8;
        case XK_F9: return Key_F9;
        case XK_F10: return Key_F10;
        case XK_F11: return Key_F11;
        case XK_F12: return Key_F12;
        case XK_Home: return Key_Home;
        case XK_Insert: return Key_Insert;
        case XK_Super_L: return Key_LeftWin;
        case XK_Super_R: return Key_RightWin;
        case XK_minus: return Key_Minus;
        case XK_Num_Lock: return Key_NumLock;
        case XK_KP_Insert: return Key_NumPad0;
        case XK_KP_End: return Key_NumPad1;
        case XK_KP_Down: return Key_NumPad2;
        case XK_KP_Page_Down: return Key_NumPad3;
        case XK_KP_Left: return Key_NumPad4;
        case XK_KP_Begin: return Key_NumPad5;
        case XK_KP_Right: return Key_NumPad6;
        case XK_KP_Home: return Key_NumPad7;
        case XK_KP_Up: return Key_NumPad8;
        case XK_KP_Page_Up: return Key_NumPad9;
        case XK_KP_Add: return Key_NumPadAdd;
        case XK_KP_Delete: return Key_NumPadDecimal;
        case XK_KP_Divide: return Key_NumPadDivide;
        case XK_KP_Enter: return Key_NumPadEnter;
        case XK_KP_Equal: return Key_NumPadEqual;
        case XK_KP_Multiply: return Key_NumPadMultiply;
        case XK_KP_Subtract: return Key_NumPadSubtract;
        case XK_Page_Down: return Key_PageDown;
        case XK_Page_Up: return Key_PageUp;
        case XK_Pause: return Key_Pause;
        case XK_period: return Key_Period;
        case XK_Print: return Key_PrintScreen;
        case XK_apostrophe: return Key_Quote;
        case XK_Scroll_Lock: return Key_ScrollLock;
        case XK_semicolon: return Key_Semicolon;
        case XK_Shift_L: return Key_LeftShift;
        case XK_Shift_R: return Key_RightShift;
        case XK_slash: return Key_Slash;
        case XK_space: return Key_Space;
        case XK_Tab: return Key_Tab;
        default: return Key_Invalid;
    }
}

// `XKey2MinoKey` needs to be before `#include <linux/input.h>` because it contains defines that collide with Mino.
#include <fcntl.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <libudev.h>
#include <linux/input.h>
#include <linux/types.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>

struct WindowNative {
    Display *xDisplay;
    Window xWindow;
    Atom deleteWindow;
    XVisualInfo *visualInfo;
    struct udev *udev;
    struct udev_monitor *monitor;

    GLXContext glContext;
};

struct GamepadNative {
    int fileDescriptor;
    char *devicePath;
    struct input_absinfo analogInfos[GamepadAxis_Count];
    struct ff_effect rumble;
};

const int minoAxis2EvdevAxis[GamepadAxis_Count] = {
    [GamepadAxis_LeftStickX] = ABS_X,
    [GamepadAxis_LeftStickY] = ABS_Y,
    [GamepadAxis_LeftTrigger] = ABS_Z,
    [GamepadAxis_RightStickX] = ABS_RX,
    [GamepadAxis_RightStickY] = ABS_RY,
    [GamepadAxis_RightTrigger] = ABS_RZ,
};

const int evdevAxis2MinoAxis[GamepadAxis_Count] = {
    [ABS_X] = GamepadAxis_LeftStickX,
    [ABS_Y] = GamepadAxis_LeftStickY,
    [ABS_Z] = GamepadAxis_LeftTrigger,
    [ABS_RX] = GamepadAxis_RightStickX,
    [ABS_RY] = GamepadAxis_RightStickY,
    [ABS_RZ] = GamepadAxis_RightTrigger,
};

GamepadButton evdevButton2MinoButton(int button) {
    switch (button) {
        case BTN_A: return GamepadButton_A;
        case BTN_B: return GamepadButton_B;
        case BTN_X: return GamepadButton_X;
        case BTN_Y: return GamepadButton_Y;
        case BTN_START: return GamepadButton_Start;
        case BTN_SELECT: return GamepadButton_Select;
        case BTN_TL: return GamepadButton_L1;
        case BTN_TR: return GamepadButton_R1;
        case BTN_TL2: return GamepadButton_L2;
        case BTN_TR2: return GamepadButton_R2;
        case BTN_THUMBL: return GamepadButton_L3;
        case BTN_THUMBR: return GamepadButton_R3;
        case BTN_MODE: return GamepadButton_Home;
        default: return GamepadButton_Invalid;
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
    for (GamepadAxis i = 0; i < GamepadAxis_Count; i++) {
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
    for (int i = 0; i < GamepadAxis_Count; i++) {
        gamepad->axes[i] = 0;
    }
}

bool WindowInit(MinoWindow *window, WindowConfig config) {
    Display *xDisplay = XOpenDisplay(nil);
    if (xDisplay == nil) return false;

    XVisualInfo *visualInfo = glXChooseVisual(
        xDisplay,
        0,
        (GLint[]){
            GLX_RGBA,
            GLX_DEPTH_SIZE,
            24,
            GLX_DOUBLEBUFFER,
            None,
        });

    if (visualInfo == nil) return false;

    Window root = DefaultRootWindow(xDisplay);

    Colormap colormap = XCreateColormap(xDisplay, root, visualInfo->visual, AllocNone);

    int screenID = DefaultScreen(xDisplay);

    XSetWindowAttributes windowAttributes = {
        .colormap = colormap,
        .event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
        .background_pixel = WhitePixel(xDisplay, screenID),
        .border_pixel = BlackPixel(xDisplay, screenID),
        .override_redirect = True,
    };

    Window xWindow = XCreateWindow(
        xDisplay,
        root,
        0, 0, config.width, config.height,
        0,
        visualInfo->depth,
        InputOutput,
        visualInfo->visual,
        CWColormap | CWEventMask,
        &windowAttributes);

    if (xWindow == 0) return false;

    Atom deleteWindowAtom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(xDisplay, xWindow, &deleteWindowAtom, 1);

    XMapWindow(xDisplay, xWindow);
    XSync(xDisplay, xWindow);

    XStoreName(xDisplay, xWindow, config.title);

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
        .visualInfo = visualInfo,
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
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Left);
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Right);
        } else if (event.value <= -1) {
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Left);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Right);
        } else {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Left);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Right);
        }
        return;
    } else if (event.code == ABS_HAT0Y) {
        if (event.value >= 1) {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Down);
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Up);
        } else if (event.value <= -1) {
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Down);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Up);
        } else {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Down);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Up);
        }
        return;
    }
    if (event.code >= GamepadAxis_Count) return;  // unsupported axis.
    GamepadAxis axis = evdevAxis2MinoAxis[event.code];
    struct input_absinfo info = gamepad->native->analogInfos[axis];
    if (axis == GamepadAxis_LeftStickY || axis == GamepadAxis_RightStickY) event.value *= -1;
    if (axis == GamepadAxis_LeftTrigger || axis == GamepadAxis_RightTrigger) {
        gamepad->axes[axis] = clamp((float32)(event.value - info.minimum) / (float32)(info.maximum - info.minimum), 0, 1);
    } else {
        gamepad->axes[axis] = clamp((float32)(event.value - info.minimum) / (float32)(info.maximum - info.minimum) * 2.0 - 1.0, -1, 1);
    }
    if (axis == GamepadAxis_LeftTrigger) {
        if (event.value > info.flat)
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_L2);
        else
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_L2);
    } else if (axis == GamepadAxis_RightTrigger) {
        if (event.value > info.flat)
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_R2);
        else
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_R2);
    }
}

static void setButton(Gamepad *gamepad, struct input_event event) {
    GamepadButton button = evdevButton2MinoButton(event.code);
    if (button == GamepadButton_Invalid) return;
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
    if (write(gamepad->native->fileDescriptor, &event, sizeof(event)) < 0) {
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
    if (window->native->glContext) {
        glXMakeCurrent(
            window->native->xDisplay,
            window->native->xWindow,
            window->native->glContext);
        glXSwapBuffers(
            window->native->xDisplay,
            window->native->xWindow);
    }
    while (XPending(native->xDisplay)) {
        XNextEvent(native->xDisplay, &event);
        switch (event.type) {
            case Expose: {
                XWindowAttributes windowAttributes;
                XGetWindowAttributes(
                    window->native->xDisplay,
                    window->native->xWindow,
                    &windowAttributes);

                window->width = windowAttributes.width;
                window->height = windowAttributes.height;

                if (window->native->glContext) {
                    glViewport(
                        0, 0,
                        windowAttributes.width,
                        windowAttributes.height);
                }
            } break;

            case MotionNotify: {
                window->mouseX = event.xmotion.x;
                window->mouseY = event.xmotion.y;
            } break;

            case ButtonPress: {
                switch (event.xbutton.button) {
                    case 1:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Left);
                        break;
                    case 2:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Middle);
                        break;
                    case 3:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Right);
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
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Back);
                        break;
                    case 9:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Forward);
                        break;
                }
                break;
            }

            case ButtonRelease: {
                switch (event.xbutton.button) {
                    case 1:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Left);
                        break;
                    case 2:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Middle);
                        break;
                    case 3:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Right);
                        break;
                    case 8:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Back);
                        break;
                    case 9:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Forward);
                        break;
                }
            } break;

            case KeyPress:
            case KeyRelease: {
                KeySym keySymbol = XkbKeycodeToKeysym(native->xDisplay, event.xkey.keycode, 0, 0);
                Key key = XKey2MinoKey(keySymbol);
                if (key == Key_Invalid) break;

                window->keyPressed[key] = event.type == KeyPress;

                if (key == Key_LeftCtrl || key == Key_RightCtrl)
                    window->keyPressed[Key_Ctrl] = (window->keyPressed[Key_LeftCtrl] || window->keyPressed[Key_RightCtrl]);
                else if (key == Key_LeftShift || key == Key_RightShift)
                    window->keyPressed[Key_Shift] = (window->keyPressed[Key_LeftShift] || window->keyPressed[Key_RightShift]);
                else if (key == Key_LeftWin || key == Key_RightWin)
                    window->keyPressed[Key_Win] = (window->keyPressed[Key_LeftWin] || window->keyPressed[Key_RightWin]);
                else if (key == Key_LeftAlt || key == Key_RightAlt)
                    window->keyPressed[Key_Alt] = (window->keyPressed[Key_LeftAlt] || window->keyPressed[Key_RightAlt]);

                window->KeyMod =
                    (window->keyPressed[Key_Win] ? KeyMod_Win : 0) |
                    (window->keyPressed[Key_Alt] ? KeyMod_Alt : 0) |
                    (window->keyPressed[Key_Shift] ? KeyMod_Shift : 0) |
                    (window->keyPressed[Key_Ctrl] ? KeyMod_Ctrl : 0) |
                    (bitSet(event.xkey.state, LockMapIndex) ? KeyMod_CapsLock : 0) |
                    (bitSet(event.xkey.state, Mod2MapIndex) ? KeyMod_NumLock : 0) |
                    (bitSet(event.xkey.state, Mod5MapIndex) ? KeyMod_ScrollLock : 0);

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
    XFree(window->native->visualInfo);
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

bool GraphicsInit(MinoWindow *window) {
    GLXContext glContext = glXCreateContext(
        window->native->xDisplay,
        window->native->visualInfo,
        nil,
        GL_TRUE);

    if (glContext == nil) return false;

    glXMakeCurrent(
        window->native->xDisplay,
        window->native->xWindow,
        glContext);

    glEnable(GL_DEPTH_TEST);

    window->native->glContext = glContext;
    return true;
}

void GraphicsMakeCurrent(MinoWindow *window) {
    glXMakeCurrent(
        window->native->xDisplay,
        window->native->xWindow,
        window->native->glContext);
}

void GraphicsClose(MinoWindow *window) {
    if (window->native->glContext == nil) return;

    glXDestroyContext(
        window->native->xDisplay,
        window->native->glContext);

    window->native->glContext = nil;
}

#endif
