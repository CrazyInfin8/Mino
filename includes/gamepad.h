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
// Note: The ordering of `GAMEPAD_L2`, `GAMEPAD_R2`, and `GAMEPAD_HOME` is
// intentionally placed at the end because they may not have corresponding
// mappings on each platforms. L2 and R2 refer to the triggers which are
// usually analog axes. You can these values to check and see whether these are
// pressed beyond the set deadzone. `GAMEPAD_HOME` might not be available on all
// platforms as the system may use them to exit the program to the systems home
// menu (For example on consoles like XBox, PlayStation, but the Windows XInput
// API also doesn't allow access to this button).
typedef enum PACK_ENUM GamepadButton {
    GAMEPAD_UP,
    GAMEPAD_DOWN,
    GAMEPAD_LEFT,
    GAMEPAD_RIGHT,

    GAMEPAD_A,
    GAMEPAD_B,
    GAMEPAD_X,
    GAMEPAD_Y,

    GAMEPAD_START,
    GAMEPAD_SELECT,

    GAMEPAD_L1,
    GAMEPAD_R1,
    GAMEPAD_L3,
    GAMEPAD_R3,

    // `GAMEPAD_HOME`
    //
    // Note: This is not might not be accessible on every platforms yet.
    // Consoles like XBox and PlayStation may reserve these but the Windows
    // XInput API also doesn't allow access to this button.
    GAMEPAD_HOME,

    // `GAMEPAD_L2` is the left trigger. This is often an analog value however
    // you can check whether this is pressed beyond the deadzone.
    GAMEPAD_L2,
    // `GAMEPAD_R2` is the right trigger. This is often an analog value however
    // you can check whether this is pressed beyond the deadzone.
    GAMEPAD_R2,

    GAMEPAD_LB = GAMEPAD_L1,
    GAMEPAD_RB = GAMEPAD_R1,
    GAMEPAD_LT = GAMEPAD_L2,
    GAMEPAD_RT = GAMEPAD_R2,
    GAMEPAD_LSTICK = GAMEPAD_L3,
    GAMEPAD_RSTICK = GAMEPAD_R3,

    // `GAMEPAD_BUTTON_COUNT` is not a gamepad button. It denotes the number of
    // gamepad buttons available to check.
    GAMEPAD_BUTTON_COUNT,
    GAMEPAD_BUTTON_INVALID,
} GamepadButton;

// `GamepadAxis` represents an analog axis on a standard gamepad controller.
//
// A standard gamepad often has 2 joysticks (which each have 2 directions) and 2
// analog triggers.
typedef enum PACK_ENUM GamepadAxis {
    GAMEPAD_AXIS_LEFT_STICK_X,
    GAMEPAD_AXIS_LEFT_STICK_Y,
    GAMEPAD_AXIS_RIGHT_STICK_X,
    GAMEPAD_AXIS_RIGHT_STICK_Y,
    GAMEPAD_AXIS_LEFT_TRIGGER,
    GAMEPAD_AXIS_RIGHT_TRIGGER,
    // `GAMEPAD_AXIS_COUNT` is not a gamepad axis. It denotes the number of
    // gamepad axis available to check.
    GAMEPAD_AXIS_COUNT,
} GamepadAxis;

// `Gamepad` represents a single player's gamepad.
//
// Often times, a device or game can support more than 1 gamepad connected to
// have multiple players playing the same game.
typedef struct Gamepad {
    bool connected;

    float32 axes[GAMEPAD_AXIS_COUNT];
    float32 pAxes[GAMEPAD_AXIS_COUNT];

    uint32 buttons;
    uint32 pButtons;

    float32 leftMotor;
    float32 pLeftMotor;
    float32 rightMotor;
    float32 pRightMotor;

    int playerID;
    GamepadNative* native;
} Gamepad;

DecList(Gamepad, GamepadList)

    // `WindowGetGamepad` retrieves a players gamepad given their playerID (most
    // often 1-4).
    //
    // If the playerID is more than supported, this function returns `nil`.
    Gamepad* WindowGetGamepad(Window* window, int playerID);

// `WindowGetFirstConnectedGamepad` attempts to retrieve the first gamepad that
// was connected.
//
// If no gamepads have been connected, this returns `nil`. The gamepad returned
// can also disconnect, but this function will still return that gamepad.
Gamepad* WindowGetFirstConnectedGamepad(Window* window);

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