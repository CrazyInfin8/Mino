#ifndef Aff3_H
#define Aff3_H

#ifndef Types_H
#define Types_H

#include <stdbool.h>
#include <stddef.h>

typedef signed char int8;
typedef signed short int int16;
typedef signed long int int32;
typedef signed long long int int64;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef unsigned long long int uint64;

typedef unsigned int uint;
typedef uint8 byte;
typedef wchar_t rune;

typedef float float32;
typedef double float64;

#define nil ((void*)0)

#define PACK_ENUM __attribute__ ((__packed__))
#endif // Types_H
#ifndef Vec2_H
#define Vec2_H

#include <math.h>

#ifndef Consts_H
#define Consts_H


// `PI` is π, a mathematical constant useful for rotation and trigonometry.
//
// This is equivalent to 180 degrees of rotation or halfway around a circle.
extern const float32 PI;

// `PI_2` is π multiplied by 2.
//
// This is equivalent to 360 degrees of rotation or the full rotation of a circle.
extern const float32 PI_2;

// `TO_DEG` helps to convert radians to degrees by simply multiplying radians
// with this value.
//
// It is equal to `180 / π`.
extern const float32 TO_DEG;

// `TO_RAD` helps to convert degrees to radians by simply multiplying degrees
// with this value.
//
// It is equal to `π / 180`.
extern const float32 TO_RAD;

#endif  // Consts_H


// `Vec2` implements a vector/point in 2D space.
//
// Note: None of the `Vec2` functions modify the `Vec2`. Instead if returns a
// new `Vec2` which you can use to set the original `Vec2` if you want.
typedef struct Vec2 {
    float32 X, Y;
} Vec2;

typedef struct Aff3 Aff3;

// `Vec2TransformAff3` applies the matrix's transformations to the `Vec2`.
//
// This applies all transformations applied the the matrix onto this `Vec2`
Vec2 Vec2TransformAff3(Vec2 this, Aff3 aff3);

// `Vec2Rotate` rotates the `Vec2` around the specified pivot point by the
// `angle` in degrees.
Vec2 Vec2Rotate(Vec2 this, Vec2 pivot, float32 angle);

#endif  // Vec2_H

// `Aff3` implements a 3x3 affine transformation matrix where the bottom row is
// always implicitly `[0, 0, 1]`.
//
// Note: None of `Aff3` functions modify the `Aff3`. Instead it returns a new
// `Aff3` which you can use to set the original `Aff3 if you want.
typedef struct Aff3 {
    float32 A, B, C, D, TX, TY;
} Aff3;

// `Aff3Identity` returns an identity matrix.
//
// This is defined as: `[[1, 0, 0], [0, 1, 0], [0, 0, 1]]`.
Aff3 Aff3Identity();

// `Aff3Concat` applies the transformations from `aff3` onto `this` matrix.
//
// The order of matrices you concatenate matters. For example, concatenating a
// rotation matrix to a translation matrix may have different results than
// concatenating those same translation and a rotation matrix.
Aff3 Aff3Concat(Aff3 this, Aff3 aff3);

// `Aff3Invert` reverses the transformations applied from `this` matrix.
//
// Objects transformed by the original matrix will revert back to their original
// transformations when you apply this transformation.
Aff3 Aff3Invert(Aff3 this);

Aff3 Aff3Rotate(Aff3 this, float32 angle);

Aff3 Aff3Scale(Aff3 this, float32 x, float32 y);

Aff3 Aff3Translate(Aff3 this, float32 x, float32 y);

// `Aff3TransformVec2` applies the matrix's transformations to the `Vec2`.
//
// This applies all transformations applied to this matrix onto that `Vec2`.
Vec2 Aff3TransformVec2(Aff3 this, Vec2 vec);

// `Aff3Print` prints the matrix in a neat way. Useful for debugging.
//
// Note: if `NOPRINT` is defined, this may not print to the console.
void Aff3Print(Aff3 this);

#endif  // Aff3_H
#ifndef Audio_H
#define Audio_H


#ifndef AUDIO_SAMPLE_RATE
#define AUDIO_SAMPLE_RATE 44100
#endif  // AUDIO_SAMPLE_RATE

#ifndef AUDIO_BUFFER_SIZE
#define AUDIO_BUFFER_SIZE 8192
#endif  // AUDIO_BUFFER_SIZE

#ifndef AUDIO_BUFFER_COUNT
#define AUDIO_BUFFER_COUNT 2
#endif  // AUDIO_BUFFER_COUNT

// #ifndef AUDIO_CHANNEL_COUNT
#define AUDIO_CHANNEL_COUNT 1
// #endif

// `AudioNative` is the platform's native audio interface.
//
// It is not meant to be interacted with directly.
typedef struct AudioNative AudioNative;

// `Audio` is an interface to stream sound to your devices speakers .
typedef struct Audio {
    AudioNative* native;
} Audio;

// `AudioInit` initializes the Audio interface for your current platform.
// It returns true when successful or false if there was an error.
//
// Remember to close finished `Audio` interfaces with `AudioClose`
bool AudioInit(Audio* audio);

// `AudioClose` cleans up this audio interface.
//
// It should be called when you are finished using this audio interface.
void AudioClose(Audio* audio);

// `AudioAvailable` returns the number of samples that are available to be
// written.
int AudioAvailable(Audio* audio);

// `AudioWrite` writes the soundwave from `wave` to the audio interface.
// `length` is the number of samples to be written.
void AudioWrite(Audio* audio, float32* wave, int length);


#endif  // Audio_H
#ifndef Gamepad_H
#define Gamepad_H

#ifndef List_H
#define List_H


// `List` is a simple, dynamically allocated, array of contiguous memory.
//
// Before you use a new list, you should call `ListInit` to initialize the size
// of each element. Once you do, you can add, index, and remove elements of that
// size.
//
// As memory is dynamically allocated, it is a good idea to call `ListFree` when
// you are done with this list to free up it's memory. Note that after you call
// `ListFree`, you can still use the list like normal as it frees the data and
// zeroes the capacity but the list is still valid.
//
// See also the `DecList` and `DefList` macros for ways to create lists specific
// to a certain type.
typedef struct List {
    byte* const data;
    const int itemSize;
    const int len;
    const int cap;
} List;

// `ListInit` initializes a blank list with the given `itemSize`. You can
// specify the initial length and capacity of the list to preallocate memory for
// this list.
//
// This returns true if the allocation was successful, else it returns false.
bool ListInit(List* list, int itemSize, int len, int cap);

// `ListGet` retrieves an element from this list at the specified index. This
// returns nil if the index is out of range.
//
// Note: The pointer returned from `ListGet` is owned by this list. It may
// change, especially if you shift, unshift, pop, or grow this list.
void* ListGet(List* list, int index);

// `ListFree` discards all elements from this list and frees it's memory. Even
// though the memory is freed, the list is still valid to use without
// initializing again; it's capacity is simply set to zero (which may mean
// other operations may need to grow this list again).
//
// You should call this when you are finished using this list.
void ListFree(List* list);

// `ListGrow` tries to grow this list to fit the amount of elements specified by
// `newCap`. This does not modify the length of the list, rather it allocates
// more memory to fit more elements, copies the old data, then changes the lists
// capacity. This allows you to allocate extra memory to minimize calls to the
// allocator.
//
// This returns true if the allocation was successful, else it returns false. If
// `newCap` is smaller than the current capacity, no changes are made and this
// returns true.
bool ListGrow(List* list, int newCap);

// `ListPush` tries to append a new entry and copies the data located by `item`
// into this list. If the list's capacity is too small to accommodate a new
// entry, the list is automatically grown by a scale factor of `1.5 * list.cap`.
//
// This returns true if appending the entry was successful. If the list was
// unable to allocate a new entry then this returns false.
bool ListPush(List* list, void* item);

// `ListPop` removes an entry from the end of the list. if `dest` is not nil,
// the entry is copied to that location.
//
// This returns true if it actually removed an element. It returns false if
// there are no elements to remove.
bool ListPop(List* list, void* dest);

// `ListUnshift` tries to prepend a new entry and copies the data located by
// `item` into this list. If the list's capacity is too small to accommodate a
// new entry, the list is automatically grown by a scale factor of
// `1.5 * list.cap`. This is slightly more expensive than pushing as the older
// data needs to be copied over to accommodate the new entry.
//
// This returns true if prepending the entry was successful. If the list was
// unable to allocate a new entry then this returns false.
bool ListUnshift(List* list, void* item);

// `ListPop` removes an entry from the beginning of the list. if `dest` is not
// nil, the entry is copied to that location. This is slightly more expensive
// than removing from the beginning as the remaining data needs to be copied
// over.
//
// This returns true if it actually removed an element. It returns false if
// there are no elements to remove.
bool ListShift(List* list, void* dest);

// `DecList` declares a list with entries of type: `Type` and type name: `Name`.
// This defines the type but forward declares the functions without
// implementing or defining them yet (see `DefList`). This allows you to declare
// lists as a struct instead of a pointer as well as get access to list length
// and capacity but might necessitate the use of include guards. or
// `#pragma once`.
//
// This is intended to be placed in header files so as not to duplicate the
// definition of each function.
//
// The functions declared by this macro wrap the ordinary `List` type but ensure
// that the size of entries match the size of `Type` and provide some ergonomics
// such as passing entries by the entry type instead of by `void*`.
//
// Make sure you call the init function before you use the list to ensure that
// the size of each entry is properly set.
#define DecList(Type, Name)                        \
    typedef struct Name {                          \
        Type* const data;                          \
        const int itemSize;                        \
        const int len;                             \
        const int cap;                             \
    } Name;                                        \
    bool Name##Init(Name* list, int len, int cap); \
    Type* Name##Get(struct Name* list, int index); \
    void Name##Free(Name* list);                   \
    bool Name##Grow(Name* list, int newCap);       \
    bool Name##Push(Name* list, Type item);        \
    bool Name##Pop(Name* list, Type* dest);        \
    bool Name##Unshift(Name* list, Type item);     \
    bool Name##Shift(Name* list, Type* dest);

// `DefList` defines the functions of a list declared with `DecList`. The
// functions defined by this macro wrap the ordinary list functions but ensure
// that the size of entries math the size of `Type` and provide some ergonomics
// such as passing entries by the entry type instead of by `void*`.
//
// This is intended to be placed in a C function to avoid duplicating the
// function definitions but it should come after you have declared the list with
// `DecList`.
//
// Make sure you call the init function before you use the list to ensure that
// the size of each entry is properly set.
#define DefList(Type, Name)                                       \
    extern inline bool Name##Init(Name* list, int len, int cap) { \
        return ListInit((List*)list, sizeof(Type), len, cap);     \
    }                                                             \
    extern inline Type* Name##Get(Name* list, int index) {        \
        return (Type*)ListGet((List*)list, index);                \
    }                                                             \
    extern inline void Name##Free(Name* list) {                   \
        ListFree((List*)list);                                    \
    }                                                             \
    extern inline bool Name##Grow(Name* list, int newCap) {       \
        return ListGrow((List*)list, newCap);                     \
    }                                                             \
    extern inline bool Name##Push(Name* list, Type item) {        \
        return ListPush((List*)list, &item);                      \
    }                                                             \
    extern inline bool Name##Pop(Name* list, Type* dest) {        \
        return ListPop((List*)list, dest);                        \
    }                                                             \
    extern inline bool Name##Unshift(Name* list, Type item) {     \
        return ListUnshift((List*)list, &item);                   \
    }                                                             \
    extern inline bool Name##Shift(Name* list, Type* dest) {      \
        return ListShift((List*)list, dest);                      \
    }

#endif  // List_H

typedef struct Window Window;

typedef struct GamepadNative GamepadNative;

// `GamepadButton` represents the buttons on a standard gamepad controller.
//
// This follows the XBox controller layout unlike a PlayStation controller that
// utilizes symbols for it's buttons.
//
// If you were using a PlayStation controller (and Mino was able to register and
// understand it's input):
//
//  - the X symbol usually corresponds to the A button.
//  - the circle symbol usually corresponds to the B button.
//  - the square symbol usually corresponds to the X button.
//  - the triangle symbol usually corresponds to the Y button.
//
// Note: The ordering of `GamepadButton_L2`, `GamepadButton_R2`, and `GamepadButton_Home` is
// intentionally placed at the end because they may not have corresponding
// mappings on each platforms. L2 and R2 refer to the triggers which are
// usually analog axes. You can these values to check and see whether these are
// pressed beyond the set deadzone. `GamepadButton_Home` might not be available on all
// platforms as the system may use them to exit the program to the systems home
// menu (For example on consoles like XBox, PlayStation, but the Windows XInput
// API also doesn't allow access to this button).
typedef enum PACK_ENUM GamepadButton {
    GamepadButton_Up,
    GamepadButton_Down,
    GamepadButton_Left,
    GamepadButton_Right,

    GamepadButton_A,
    GamepadButton_B,
    GamepadButton_X,
    GamepadButton_Y,

    GamepadButton_Start,
    GamepadButton_Select,

    GamepadButton_L1,
    GamepadButton_R1,
    GamepadButton_L3,
    GamepadButton_R3,

    // `GamepadButton_Home`
    //
    // Note: This is not might not be accessible on every platforms yet.
    // Consoles like XBox and PlayStation may reserve these but the Windows
    // XInput API also doesn't allow access to this button.
    GamepadButton_Home,

    // `GamepadButton_L2` is the left trigger. This is often an analog value however
    // you can check whether this is pressed beyond the deadzone.
    GamepadButton_L2,
    // `GamepadButton_R2` is the right trigger. This is often an analog value however
    // you can check whether this is pressed beyond the deadzone.
    GamepadButton_R2,

    GamepadButton_LB = GamepadButton_L1,
    GamepadButton_RB = GamepadButton_R1,
    GamepadButton_LT = GamepadButton_L2,
    GamepadButton_RT = GamepadButton_R2,

    GamepadButton_LeftStick = GamepadButton_L3,
    GamepadButton_RightStick = GamepadButton_R3,

    // `GamepadButton_Count` is not a gamepad button. It denotes the number of
    // gamepad buttons available to check.
    GamepadButton_Count,
    GamepadButton_Invalid = ~0,
} GamepadButton;

// `GamepadAxis` represents an analog axis on a standard gamepad controller.
//
// A standard gamepad often has 2 joysticks (which each have 2 directions) and 2
// analog triggers.
typedef enum PACK_ENUM GamepadAxis {
    GamepadAxis_LeftStickX,
    GamepadAxis_LeftStickY,
    GamepadAxis_RightStickX,
    GamepadAxis_RightStickY,
    GamepadAxis_LeftTrigger,
    GamepadAxis_RightTrigger,
    // `GamepadAxis_Count` is not a gamepad axis. It denotes the number of
    // gamepad axis available to check.
    GamepadAxis_Count,
} GamepadAxis;

// `Gamepad` represents a single player's gamepad.
//
// Often times, a device or game can support more than 1 gamepad connected to
// have multiple players playing the same game.
typedef struct Gamepad {
    bool connected;

    float32 axes[GamepadAxis_Count];
    float32 pAxes[GamepadAxis_Count];

    uint32 buttons;
    uint32 pButtons;

    float32 leftMotor;
    float32 pLeftMotor;
    float32 rightMotor;
    float32 pRightMotor;

    int playerID;
    GamepadNative* native;
} Gamepad;

DecList(Gamepad, GamepadList);

// `WindowGetGamepad` retrieves a players gamepad given their playerID (most
// often 1-4).
//
// If the playerID is more than supported, this function returns `nil`.
//
// Note: The returned pointer is still owned by Mino and it's location can be
// changed. Instead, you may want to hold onto the playerID and retrieve the
// players gamepad on each update.
Gamepad* WindowGetGamepad(Window* window, int playerID);

// `WindowGetFirstConnectedGamepad` attempts to retrieve the first gamepad that
// was connected.
//
// If no gamepads have been connected, this returns `nil`. The gamepad returned
// can also disconnect, but this function will still return that gamepad.
//
// Note: The returned pointer is still owned by Mino and it's location can be
// changed.
Gamepad* WindowGetFirstConnectedGamepad(Window* window);

// `GamepadCount` returns the number of gamepads the window is keeping track of.
//
// Not all gamepads counted may be connected.
int GamepadCount(Window* window);

// `GamepadConnected` checks if the current gamepad is connected.
bool GamepadConnected(Gamepad* gamepad);

// `GamepadButtonPressed` returns true if the specified button on the current
// gamepad is pressed.
//
// If the gamepad is not connected, this will always return false.
bool GamepadButtonPressed(Gamepad* gamepad, GamepadButton button);

// `GamepadButtonJustPressed` returns true if the specified button on the
// current gamepad has just been pressed since the last call to `WindowUpdate`.
//
// If the gamepad is not connected, this will always return false.
bool GamepadButtonJustPressed(Gamepad* gamepad, GamepadButton button);

// `GamepadButtonJustReleased` returns true if the specified button on the
// current gamepad has just been released since the last call to `WindowUpdate`.
//
// If the gamepad is not connected, this will always return false.
bool GamepadButtonJustReleased(Gamepad* gamepad, GamepadButton button);

// `GamepadAxisValue` returns the analog value of the current gamepad's analog
// axis.
//
// The value is normalized so joysticks range from -1 to 1 in both directions
// (horizontal and vertical) and triggers range from 0 to 1.
//
// If the gamepad is not connected, this will always return 0.
float32 GamepadAxisValue(Gamepad* gamepad, GamepadAxis axis);

// `GamepadSetVibration` sets the speed for the vibration motors.
//
// The values for left and right range from 0 (no vibration) to 1 (full
// vibration).
//
// The left and right vibration motor may not be the same. For example, on XBox
// controllers and PS4 DualShocks, the left motor is for low frequency
// vibrations while the right motor is for high frequency vibrations.
void GamepadSetVibration(Gamepad* gamepad, float32 leftMotor, float32 rightMotor);

#endif  // Gamepad_H
#ifndef Graphics_H
#define Graphics_H

#ifndef Window_H
#define Window_H

#ifndef Keyboard_H
#define Keyboard_H


typedef struct Window Window;

// `Key` represents a keyboard key code. Each key code corresponds to a key on a
// keyboard.
typedef enum Key Key;

// `KeyMod` represents modifier keys that could change or modify the
// behavior of other certain gestures.
typedef enum KeyMod KeyMod;

// `KeyPressed` returns true if the current key is pressed.
bool KeyPressed(Window* window, Key key);

// `KeyJustPressed` returns true if the specified key code has just been pressed
// since the last call to `WindowUpdate`
bool KeyJustPressed(Window* window, Key key);

// `KeyJustReleased` returns true if the specified key code has just been
// release since the last call to `WindowUpdate`.
bool KeyJustReleased(Window* window, Key key);

// `KeyModSet` checks that the modifier is currently enabled.
bool KeyModSet(Window* window, KeyMod modifier);

// `KeyGetChar` returns the character that the combinations of pressed keys
// would produce.
rune KeyGetChar(Window* window);

enum PACK_ENUM KeyMod {
    KeyMod_Ctrl = (1 << 0),
    KeyMod_Shift = (1 << 1),
    KeyMod_Alt = (1 << 2),
    KeyMod_Win = (1 << 3),
    KeyMod_CapsLock = (1 << 4),
    KeyMod_ScrollLock = (1 << 5),
    KeyMod_NumLock = (1 << 6),
};

enum PACK_ENUM Key {
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,
    Key_LeftAlt,
    Key_LeftOption = Key_LeftAlt,
    Key_RightAlt,
    Key_RightOption = Key_RightAlt,
    Key_DownArrow,
    Key_LeftArrow,
    Key_RightArrow,
    Key_UpArrow,
    Key_Tilde,
    Key_Backslash,
    Key_Backspace,
    Key_LeftBracket,
    Key_RightBracket,
    Key_CapsLock,
    Key_Comma,
    Key_Menu,
    Key_LeftCtrl,
    Key_RightCtrl,
    Key_Delete,
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,
    Key_End,
    Key_Enter,
    Key_Equal,
    Key_Escape,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_Home,
    Key_Insert,
    Key_LeftWin,
    Key_LeftSuper = Key_LeftWin,
    Key_LeftCommand = Key_LeftWin,
    Key_RightWin,
    Key_RightSuper = Key_RightWin,
    Key_RightCommand = Key_RightWin,
    Key_Minus,
    Key_NumLock,
    Key_NumPad0,
    Key_NumPad1,
    Key_NumPad2,
    Key_NumPad3,
    Key_NumPad4,
    Key_NumPad5,
    Key_NumPad6,
    Key_NumPad7,
    Key_NumPad8,
    Key_NumPad9,
    Key_NumPadAdd,
    Key_NumPadDecimal,
    Key_NumPadDivide,
    Key_NumPadEnter,
    Key_NumPadEqual,
    Key_NumPadMultiply,
    Key_NumPadSubtract,
    Key_PageDown,
    Key_PageUp,
    Key_Pause,
    Key_Period,
    Key_PrintScreen,
    Key_Quote,
    Key_ScrollLock,
    Key_Semicolon,
    Key_LeftShift,
    Key_RightShift,
    Key_Slash,
    Key_Space,
    Key_Tab,
    Key_Alt,
    Key_Option = Key_Alt,
    Key_Ctrl,
    Key_Shift,
    Key_Win,
    Key_Super = Key_Win,
    Key_Command = Key_Win,

    // `Key_COUNT` is not a key code. It denotes the number of key codes
    // available to check.
    Key_Count,

    // Used internally to denote an unsupported key code.
    Key_Invalid = ~0,
};

#endif  // Keyboard_H

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
    bool keyPressed[Key_Count];
    bool pKeyPressed[Key_Count];
    byte KeyMod;
    rune keyChar;

    int width, height;

    int mouseX, mouseY;
    int pMouseX, pMouseY;

    int scrollX, scrollY;
    int pScrollX, pScrollY;

    byte mousePressed;
    byte pMousePressed;

    GamepadList gamepads;

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

// `Color` represents an RGBA color value.
//
// By varying the intensity of red (`R`), green (`G`), and blue (`B`), you
// can create a wide range of color. Alpha (`A`) specifies how transparent or
// opaque the color is.
typedef struct Color {
    uint8 R;
    uint8 G;
    uint8 B;
    uint8 A;
} Color, RGBA;

// `ColorHex` creates a Color object using a hexadecimal value in ARGB format.
Color ColorHex(const uint32 color);

// `GraphicsInit` creates an OpenGL context for the current window. It returns
// true when successful or false if there was an error.
//
// Remember to close the graphics api by calling `GraphicsClose`. (This should
// be done before disposing the window)
bool GraphicsInit(Window* window);

// `GraphicsClose` frees the OpenGL context for this window.
//
// It should be called when you are finished drawing graphics on this window.
void GraphicsClose(Window* window);

// `GraphicsMakeCurrent` makes the OpenGL api use this window and its context.
//
// This should be called before doing OpenGL operations to ensure that OpenGL
// functions target this window and context
void GraphicsMakeCurrent(Window* window);

#endif  // Graphics_H
#ifndef Mouse_H
#define Mouse_H


typedef struct Window Window;

// `MouseButton` represents the buttons on a standard mouse.
//
// Most likely, there are 5 buttons at most on a mouse. Some gaming mice may
// have more buttons but they usually get mapped to keyboard keys or have other
// purposes like changing CPI/DPI instead
typedef enum MouseButton {
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
    MouseButton_Back,
    MouseButton_Forward,
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
#ifndef Synth_H
#define Synth_H


void WaveMultiply(float32* wave, int length, float32 scale);

typedef struct Phasor {
    float32 phase;
    float32 phaseInterval;
} Phasor;

void PhasorSetFrequency(Phasor* phasor, float32 frequency);

void PhasorStream(Phasor* phasor, float32* wave, int length);

typedef enum OscillatorType {
    SineOscillator,
    SawOscillator,
    SquareOscillator,
} OscillatorType;

typedef union OscillatorOptions {
    struct SineOptions {
    } sine;
    struct SawOptions {
    } saw;
    struct SquareOptions {
    } square;
} OscillatorOptions;

typedef struct Oscillator {
    OscillatorType type;
    OscillatorOptions options;
} Oscillator;

void CreateSineOscillator(Oscillator* oscillator);

void OscillatorStream(Oscillator oscillator, float32* wave, int length);

#endif
#ifndef Utils_H
#define Utils_H

#include <stddef.h>

#ifdef NOPRINT
#define print(format, ...) \
    do {                   \
    } while (false)
#define println(format, ...) \
    do {                     \
    } while (false)
#else
int printf(const char* restrict format, ...);

// `print` prints to the standard output (terminal/command prompt).
//
// if `NOPRINT` is defined, this doesn't do anything and simply ignores passed
// arguments.
#define print(format, ...) printf(format __VA_OPT__(, __VA_ARGS__))

// `print` prints to the standard output (terminal/command prompt) printing a
// new line afterwards. `format` should be a string literal or constant.
//
// if `NOPRINT` is defined, this doesn't do anything and simply ignores passed
// arguments.
#define println(format, ...) printf("[%s:%d] ~> " format "\n", __FILE__, __LINE__ __VA_OPT__(, __VA_ARGS__)) // __println(__FILE__, __LINE__, format, __VA_ARGS__)
#define __println(file, line, format, ...) 
#endif

// `len` gets the length of a constant width array.
//
// This should not be used with pointers since the size of a an array from a
// pointer might not be known at compile time.
#define len(array, type) (sizeof(array) / sizeof(type))

void copy(const void* restrict src, void* restrict dst, size_t size);

// `copyPad` copies as much from `src` into `dst` up to the size specified. If
// `src` is smaller than size, the rest of `dst` is padded with null bytes 
// ('\0')
void copyPad(const void* restrict src, void* restrict dst, size_t size);

// `allocate` easily allocates and initializes to zero a chunk of memory the
// size of the specified `type`.
#define allocate(type) (type*)calloc(1, sizeof(type))
#define allocateN(type, count) (type*)calloc(count, sizeof(type))
#define reallocateN(pointer, type, count) (type*)realloc(pointer, sizeof(type) * count)

// `bitSet` checks that the bit offset by `index` is set in `bits`.
bool bitSet(uint64 bits, uint8 index);

// `bitSet` checks that the bit offset by `index` is not set in `bits`.
bool bitUnset(uint64 bits, uint8 index);

// `setBit` sets the bit offset by `index` to 1.
uint64 setBit(uint64 bits, uint8 index);

// `setBit` sets the bit offset by `index` to 0.
uint64 unsetBit(uint64 bits, uint8 index);

// `clamp` constrains value to be between the `min` and `max`.
float32 clamp(float32 value, float32 min, float32 max);

bool hasPrefix(const char* text, const char* prefix);

#endif  // Utils_H

