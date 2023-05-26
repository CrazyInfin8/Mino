#ifndef Gamepad_H
#define Gamepad_H

#include "types.h"
#include "list.h"

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