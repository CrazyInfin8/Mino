#include <math.h>

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
#include <execinfo.h>
void print_trace(void) {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }
}

struct GamepadNative {
    int fileDescriptor;
    char serial[32];
};

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

        // Gamepad *gamepad = WindowGetFirstConnectedGamepad(&window);
        // if (gamepad && gamepad->connected) {
        //     bool APressed = GamepadButtonPressed(gamepad, GAMEPAD_A);
        //     bool BPressed = GamepadButtonPressed(gamepad, GAMEPAD_B);
        //     bool XPressed = GamepadButtonPressed(gamepad, GAMEPAD_X);
        //     bool YPressed = GamepadButtonPressed(gamepad, GAMEPAD_Y);
        //     println("Gamepad connected: (Buttons: %s, %s, %s, %s), Left stick: (X: % .6f, Y: % .6f), Right stick: (X: % .6f, Y: % .6f)",
        //         APressed ? "A" : " ",
        //         BPressed ? "B" : " ",
        //         XPressed ? "X" : " ",
        //         YPressed ? "Y" : " ",
        //         GamepadAxisValue(gamepad, GAMEPAD_AXIS_LEFT_STICK_X),
        //         GamepadAxisValue(gamepad, GAMEPAD_AXIS_LEFT_STICK_Y),
        //         GamepadAxisValue(gamepad, GAMEPAD_AXIS_RIGHT_STICK_X),
        //         GamepadAxisValue(gamepad, GAMEPAD_AXIS_RIGHT_STICK_Y));
        // } else {
        //     println("No gamepads connected");
        // }

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
        if (now - begin < 2000 && 2000 - (now - begin) > 0) {
            WindowSleep(2000 - (now - begin));
        }
    }
    // GraphicsClose(&window);
    // AudioClose(&audio);
    WindowClose(&window);
    return 0;
}