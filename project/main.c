#include <math.h>

#include "aff3.h"
#include "audio.h"
#include "consts.h"
#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "synth.h"
#include "types.h"
#include "utils.h"
#include "window.h"

// #include <GL/gl.h>

Window window;
Audio audio;

typedef struct Synth {
    Phasor phasor;
    float32 notes[8];
    Oscillator oscillator;
} Synth;

Synth synth = {
    .phasor = {
        .phaseInterval = 261.63 / AUDIO_SAMPLE_RATE,
    },
    .notes = {0},
    .oscillator = {
        .type = SineOscillator,
    },
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
    if (GraphicsInit(&window) == false) {
        println("Could not open Graphics");
        AudioClose(&audio);
        WindowClose(&window);
        return 1;
    }

    while (WindowUpdate(&window)) {
        int64 begin = WindowTime();

        // int availableAudio = AudioAvailable(&audio);
        // if (availableAudio > 0) {
        //     float32 buffer[AUDIO_BUFFER_SIZE];
        //     PhasorStream(&synth.phasor, buffer, availableAudio);

        //     OscillatorStream(synth.oscillator, buffer, availableAudio);

        //     AudioWrite(&audio, buffer, availableAudio);
        // }

        if (MouseJustPressed(&window, MouseButton_Left)) println("Left just pressed");
        if (MouseJustPressed(&window, MouseButton_Right)) println("Right just pressed");
        if (MouseJustPressed(&window, MouseButton_Middle)) println("Middle just pressed");
        if (MouseJustPressed(&window, MouseButton_Forward)) println("Forward just pressed");
        if (MouseJustPressed(&window, MouseButton_Back)) println("Back just pressed");

        if (MouseJustReleased(&window, MouseButton_Left)) println("Left just released");
        if (MouseJustReleased(&window, MouseButton_Right)) println("Right just released");
        if (MouseJustReleased(&window, MouseButton_Middle)) println("Middle just released");
        if (MouseJustReleased(&window, MouseButton_Forward)) println("Forward just released");
        if (MouseJustReleased(&window, MouseButton_Back)) println("Back just released");

        if (window.scrollX != 0 || window.scrollY != 0) println("Scroll = { X: %d, Y: %d }", window.scrollX, window.scrollY);

        for (int i = 0; i < Key_Count; i++) {
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
                GamepadAxisValue(gamepad, GamepadAxis_LeftTrigger),
                GamepadAxisValue(gamepad, GamepadAxis_RightTrigger));
        }

        GraphicsMakeCurrent(&window);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glBegin(GL_TRIANGLES);
        // {
        //     glColor4ub(0xFF, 0x00, 0x00, 0xFF);
        //     glVertex3f(0, 1, 0);

        //     glColor4ub(0x00, 0xFF, 0x00, 0xFF);
        //     glVertex3f(1, -1, 0);

        //     glColor4ub(0x00, 0x00, 0xFF, 0xFF);
        //     glVertex3f(-1, -1, 0);
        // }
        // glEnd();

        int64 now = WindowTime();
        if (now - begin < 1000 / 60 && 1000 / 60 - (now - begin) > 0) {
            WindowSleep(1000 / 60 - (now - begin));
        }
    }
    GraphicsClose(&window);
    // AudioClose(&audio);
    WindowClose(&window);
    return 0;
}