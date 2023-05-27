#include <math.h>

// #include "../amalgamate.h"
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

#include <GL/gl.h>
#include <stdbool.h>

Window window;
Audio audio;

const int INTERVAL = 1000 / 60;

typedef struct Synth {
    Phasor phasor;
    float32 notes[8];
    Oscillator oscillator;
} Synth;

Synth synth = {
    .phasor = {
        .phaseInterval = 261.63 / AUDIO_SAMPLE_RATE,
    },
    .notes = {},
    .oscillator = {
        .type = SineOscillator,
    },
};

void loop(float32 dt);

int main(void) {
    if (WindowInit(&window,
            (WindowConfig){
                .width = 1280,
                .height = 800,
                .title = "Quantum Leap",
            }) == false) goto errWindow;
    if (AudioInit(&audio) == false) goto errAudio;
    if (GraphicsInit(&window) == false) goto errGraphics;

    // int64 prevTime = WindowTime();
    // while (WindowUpdate(&window)) {
    //     int64 nextTime = prevTime + INTERVAL;
    //     int64 currTime = WindowTime();

    //     loop((float32)(currTime - prevTime) / 1000);

    //     int64 waitTime = nextTime - WindowTime();
    //     println("PrevTime: %lld, CurrTime: %lld, NextTime: %lld, WaitTime: %lld", prevTime, currTime, nextTime, waitTime);
    //     if (waitTime > 0) {
    //         println("Sleeping");
    //         WindowSleep(waitTime);
    //     }
    //     prevTime = currTime;
    // }

    const int INTERVAL = 1000 / 60;
    int64 prevTime = WindowTime();
    int64 nextTime = 0;
    bool running;
    do {
        // while (nextTime > WindowTime()) {}
        int64 waitTime = nextTime - WindowTime();
        if (waitTime > 0) WindowSleep(waitTime);

        int64 currTime = WindowTime();
        int64 elapsedTime = currTime - prevTime;
        prevTime = currTime;
        nextTime = prevTime + INTERVAL;
        while (nextTime < currTime) {
            prevTime += INTERVAL;
        }

        loop((float32)elapsedTime / 1000);
        running = WindowUpdate(&window);

        prevTime = currTime;
    } while (running);

    GraphicsClose(&window);
    AudioClose(&audio);
    WindowClose(&window);

    return 0;

    // Deinitialize on error cases
errGraphics:
    AudioClose(&audio);
errAudio:
    WindowClose(&window);
errWindow:
    return 1;
}

float32 sum;
int n = 0;
void loop(float32 dt) {
    sum += dt;
    n++;
    println("Delta time is: %.09f, fps: %.4f, avg: %.09f, avgFps: %0.4f", dt, 1 / dt, sum / n, 1 / (sum / n));
}