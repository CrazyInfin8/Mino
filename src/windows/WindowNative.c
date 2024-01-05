#include <windows.h>
#include <winuser.h>
#include <GL/gl.h>
// #include <GL/wgl.h>
#include <wingdi.h>
#include <xinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

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

void GraphicsMakeCurrent(Window *window) {
    HDC deviceContext = GetDC(window->native->windowHandle);
    wglMakeCurrent(deviceContext, window->native->glContext);
    ReleaseDC(window->native->windowHandle, deviceContext);
}