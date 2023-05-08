#include <math.h>
#include <processthreadsapi.h>

#include "aff3.h"
#include "audio.h"
#include "consts.h"
#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "utils.h"
#include "window.h"

Window window;
Audio audio;

// typedef struct Synth {
//     Phasor phasor;
//     float32 notes[8];
//     Oscillator oscillator;
// } Synth;

// Synth synth = {
//     .phasor = {
//         .phaseInterval = 261.63 / AUDIO_SAMPLE_RATE,
//     },
//     .notes = {
//     },
//     .oscillator = {
//         .type = SineOscillator,
//     },
// };

int main(void) {
    println("Starting game");
    if (WindowInit(&window, (WindowConfig){"Mino Demo Game Window", 800, 600}) == false) {
        println("Could not open the window");
        return 1;
    }
    if (AudioInit(&audio) == false) {
        println("Could not open Audio");
        WindowClose(&window);
        return 1;
    }
    if (GraphicsInit(&window) == false) {
        println("Could not open Graphics");
        AudioClose(&audio);
        WindowClose(&window);
    }

    // Aff3 aff3 = Aff3Identity();
    // Aff3Print(aff3);
    while (WindowUpdate(&window)) {
        int64 begin = WindowTime();

        // int availableAudio = AudioAvailable(&audio);
        // if (availableAudio > 0) {
        //     float32 buffer[AUDIO_BUFFER_SIZE];
        //     // PhasorStream(&synth.phasor, buffer, AUDIO_BUFFER_SIZE);

        //     // OscillatorStream(synth.oscillator, buffer, AUDIO_BUFFER_SIZE);

        //     AudioWrite(&audio, buffer, AUDIO_BUFFER_SIZE);
        // }

        Gamepad *gamepad = WindowGetFirstConnectedGamepad(&window);
        if (gamepad && gamepad->connected) {
            bool APressed = GamepadButtonPressed(gamepad, GAMEPAD_A);
            bool BPressed = GamepadButtonPressed(gamepad, GAMEPAD_B);
            bool XPressed = GamepadButtonPressed(gamepad, GAMEPAD_X);
            bool YPressed = GamepadButtonPressed(gamepad, GAMEPAD_Y);
            println("Gamepad connected: (Buttons: %s, %s, %s, %s), Left stick: (X: % .6f, Y: % .6f), Right stick: (X: % .6f, Y: % .6f)",
                APressed ? "A" : " ",
                BPressed ? "B" : " ",
                XPressed ? "X" : " ",
                YPressed ? "Y" : " ",
                GamepadAxisValue(gamepad, GAMEPAD_AXIS_LEFT_STICK_X),
                GamepadAxisValue(gamepad, GAMEPAD_AXIS_LEFT_STICK_Y),
                GamepadAxisValue(gamepad, GAMEPAD_AXIS_RIGHT_STICK_X),
                GamepadAxisValue(gamepad, GAMEPAD_AXIS_RIGHT_STICK_Y));
        } else {
            println("No gamepads connected");
        }

        GraphicsClear(&window);
        GraphicsBegin(&window);
        {
            GraphicsAddColor(ColorHex(0xFFFF0000));
            GraphicsAddVertex((float32[3]){0, 1, 0});

            GraphicsAddColor(ColorHex(0xFF00FF00));
            GraphicsAddVertex((float32[3]){1, -1, 0});

            GraphicsAddColor(ColorHex(0xFF0000FF));
            GraphicsAddVertex((float32[3]){-1, -1, 0});
        }
        GraphicsEnd();

        int64 now = WindowTime();
        if (now - begin < 1000 / 60 && 1000 / 60 - (now - begin) > 0) {
            WindowSleep(1000 / 60 - (now - begin));
        }
    }

    GraphicsClose(&window);
    AudioClose(&audio);
    WindowClose(&window);
    return 0;
}