#include <math.h>

#include "aff3.h"
#include "audio.h"
#include "consts.h"
#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "types.h"
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
    // if (AudioInit(&audio) == false) {
    //     println("Could not open Audio");
    //     WindowClose(&window);
    //     return 1;
    // }
    // if (GraphicsInit(&window) == false) {
    //     println("Could not open Graphics");
    //     AudioClose(&audio);
    //     WindowClose(&window);
    // }

    // Aff3 aff3 = Aff3Identity();
    // Aff3Print(aff3);
    while (WindowUpdate(&window)) {
        int64 begin = WindowTime();
        // if (window.gamepads.len > 0) break;

        // int availableAudio = AudioAvailable(&audio);
        // if (availableAudio > 0) {
        //     float32 buffer[AUDIO_BUFFER_SIZE];
        //     // PhasorStream(&synth.phasor, buffer, AUDIO_BUFFER_SIZE);

        //     // OscillatorStream(synth.oscillator, buffer, AUDIO_BUFFER_SIZE);

        //     AudioWrite(&audio, buffer, AUDIO_BUFFER_SIZE);
        // }

        if (MouseJustPressed(&window, MOUSE_LEFT)) println("Left just pressed");
        if (MouseJustPressed(&window, MOUSE_RIGHT)) println("Right just pressed");
        if (MouseJustPressed(&window, MOUSE_MIDDLE)) println("Middle just pressed");
        if (MouseJustPressed(&window, MOUSE_FORWARD)) println("Forward just pressed");
        if (MouseJustPressed(&window, MOUSE_BACK)) println("Back just pressed");

        if (MouseJustReleased(&window, MOUSE_LEFT)) println("Left just released");
        if (MouseJustReleased(&window, MOUSE_RIGHT)) println("Right just released");
        if (MouseJustReleased(&window, MOUSE_MIDDLE)) println("Middle just released");
        if (MouseJustReleased(&window, MOUSE_FORWARD)) println("Forward just released");
        if (MouseJustReleased(&window, MOUSE_BACK)) println("Back just released");

        if (window.scrollX != 0 || window.scrollY != 0) println("Scroll = { X: %d, Y: %d }", window.scrollX, window.scrollY);

        for (int i = 0; i < KEY_COUNT; i++) {
            if (KeyJustPressed(&window, i)) {
                println("Key %d just pressed", i);
            }
            if (KeyJustReleased(&window, i)) {
                println("Key %d just released", i);
            }
        }
        rune c = KeyGetChar(&window);
        if (c) println("You typed: %c", c);

        print("Mouse: { X: %d, Y: %d }\r", window.mouseX, window.mouseY);

        Gamepad *gamepad;
        for (int i = 0, l = GamepadCount(&window); i < l; i++) {
            gamepad = WindowGetGamepad(&window, i);
            if (gamepad->connected == false) continue;
            GamepadSetVibration(gamepad,
                GamepadAxisValue(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER),
                GamepadAxisValue(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER));
        }

        // GraphicsClear(&window);
        // GraphicsBegin(&window);
        // {
        //     GraphicsAddColor(ColorHex(0xFFFF0000));
        //     GraphicsAddVertex((float32[3]){0, 1, 0});

        //     GraphicsAddColor(ColorHex(0xFF00FF00));
        //     GraphicsAddVertex((float32[3]){1, -1, 0});

        //     GraphicsAddColor(ColorHex(0xFF0000FF));
        //     GraphicsAddVertex((float32[3]){-1, -1, 0});
        // }
        // GraphicsEnd();

        int64 now = WindowTime();
        if (now - begin < 1000 / 60 && 1000 / 60 - (now - begin) > 0) {
            WindowSleep(1000 / 60 - (now - begin));
        }
    }
    // GraphicsClose(&window);
    // AudioClose(&audio);
    WindowClose(&window);
    return 0;
}