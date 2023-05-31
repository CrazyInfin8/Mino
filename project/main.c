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
#include "vec2.h"
#include "window.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
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

void setup();
bool loop(float32 dt);

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

    setup();

    GraphicsMakeCurrent(&window);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_SCISSOR_TEST);

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

        running = loop((float32)elapsedTime / 1000) && WindowUpdate(&window);

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

#define NANOVG_GL2_IMPLEMENTATION
#define NVG_NO_STB
#include "nanovg/nanovg.h"
#include "nanovg/nanovg_gl.h"

NVGcontext* vg;
void setup() {
    vg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
}

void drawPolygon(float32 radius, int sides);

void drawCog(float rad1, float rad2, int sides) {
    nvgSave(vg);

    nvgBeginPath(vg);
    {
        Vec2 vec1 = {0, rad1}, vec2 = {0, rad2};

        float32 angle = (float32)360 / (float32)sides;

        nvgMoveTo(vg, vec1.X, vec1.Y);
        for (int i = 0; i < sides; i++) {
            nvgLineTo(vg, vec2.X, vec2.Y);
            vec2 = Vec2Rotate(vec2, (Vec2){0, 0}, angle);
            nvgLineTo(vg, vec2.X, vec2.Y);
            vec1 = Vec2Rotate(vec1, (Vec2){0, 0}, angle);
            nvgLineTo(vg, vec1.X, vec1.Y);
            vec1 = Vec2Rotate(vec1, (Vec2){0, 0}, angle);
            nvgLineTo(vg, vec1.X, vec1.Y);
            vec2 = Vec2Rotate(vec2, (Vec2){0, 0}, angle);
        }
        nvgClosePath(vg);
    }
    nvgStrokeColor(vg, (NVGcolor){{{1, 0.75, 0.25, 1}}});
    nvgStroke(vg);

    nvgRestore(vg);
}

// void drawBG(Vec2 offset) {
//     nvgSave(vg);
//     nvgReset(vg);

//     if (false) {
//         // offset.X;
//     }
//     // nvgBeginPath(vg);
//     // {
//     //     // nvgRect(vg, 0, 0, window.width, window.height);
//     // }
//     // nvgFillColor(vg, nvgRGB(255, 255, 255));

//     // for (int i = 0; i < offse)
//     nvgFill(vg);

//     nvgRestore(vg);
// }

float32 i;

void drawPlayButton(int x, int y) {
    nvgSave(vg);

    nvgTranslate(vg, x, y);

    nvgBeginPath(vg);
    {
        nvgCircle(vg, 0, 0, 100);
    }
    nvgFillColor(vg, (NVGcolor){{{0.25, 0.75, 1, 1}}});
    nvgFill(vg);

    // nvgBeginPath(vg);
    // {
    //     nvgArc(vg, x, y, 75. + (25. / 2.), 0, PI_2, NVG_CW);
    // }
    // nvgStrokeColor(vg, (NVGcolor){{{1, 1, 1, 1}}});
    // nvgStrokeWidth(vg, 25);
    // nvgStroke(vg);

    nvgBeginPath(vg);
    {
        drawPolygon(50, 3);
    }
    nvgFillColor(vg, (NVGcolor){{{1, 1, 1, 1}}});
    nvgFill(vg);

    nvgRestore(vg);
}

// void drawSettingsButton(int x, int y) {}
#include "./svg.h"

extern const Shape assets_ship__svg;
extern const Shape assets_settings__svg;

struct {
    Vec2 position, velocity, acceleration;

} player = {};

float32 sum;
int n = 0;

void update(float32 dt) {
    Gamepad* gamepad = WindowGetGamepad(&window, 0);
    Vec2 accel = {0, 0};
    bool shoot, boost, teleport;
    if (gamepad != nil) {
        accel.X = GamepadAxisValue(gamepad, GamepadAxis_LeftStickX) * 300;
        accel.Y = GamepadAxisValue(gamepad, GamepadAxis_LeftStickY) * -300;
    }
    if (accel.X == 0 && accel.Y == 0) {
        if (KeyPressed(&window, Key_A)) accel.X-=300;
        if (KeyPressed(&window, Key_D)) accel.X+=300;
        if (KeyPressed(&window, Key_W)) accel.Y-=300;
        if (KeyPressed(&window, Key_S)) accel.Y+=300;
    }
    player.acceleration = accel;
    player.velocity.X += player.acceleration.X * dt;
    player.velocity.Y += player.acceleration.Y * dt;
    player.position.X += player.velocity.X * dt;
    player.position.Y += player.velocity.Y * dt;
}

void draw() {
    println("{ X: %f; Y: %f }", player.position.X, player.position.Y);
    drawShapeTransformed(
        &assets_ship__svg,
        player.position,
        0.25, 0,
        ColorHex(0xFFABCDEF));
}

bool loop(float32 dt) {
    sum += dt;
    n++;
    // println("Delta time is: %.09f, fps: %.4f, avg: %.09f, avgFps: %0.4f", dt, 1 / dt, sum / n, 1 / (sum / n));
    GraphicsMakeCurrent(&window);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, window.width, window.height);

    nvgBeginFrame(vg, window.width, window.height, 1);
    {
        update(dt);

        nvgSave(vg);

        draw();
        // // drawBG((Vec2){0, 0});
        // drawPlayButton(window.width / 2, window.height / 2);

        // nvgTranslate(vg, (float32)window.width / 2, (float32)window.height / 2);
        // // drawCog();

        nvgTranslate(vg, window.mouseX, window.mouseY);
        // nvgScale(vg, 0.25, 0.25);
        // drawCog(25, 35, 16);
        nvgBeginPath(vg);
        drawShape(&assets_settings__svg);
        nvgStrokeColor(vg, (NVGcolor){{{1, 0.75, 0.25, 1}}});
        nvgStrokeWidth(vg, 1);
        nvgStroke(vg);

        nvgRestore(vg);
    }
    nvgEndFrame(vg);
    return true;
}

#include "nanovg/nanovg.c"