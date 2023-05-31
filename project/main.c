#include <math.h>
#include <stdlib.h>

// #include "../amalgamate.h"
#include "aff3.h"
#include "audio.h"
#include "consts.h"
#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "list.h"
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

typedef struct Asteroid {
    Vec2 position, velocity;
    float32 angle;
} Asteroid;

DecList(Asteroid, AsteroidList);
DefList(Asteroid, AsteroidList);
int64 asteroidSpawnTimer;

AsteroidList asteroids;

NVGcontext* vg;
void setup() {
    vg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
    AsteroidListInit(&asteroids, 0, 5);
}

// void drawSettingsButton(int x, int y) {}
#include "./svg.h"

extern const Shape assets_ship__svg;
extern const Shape assets_settings__svg;

struct {
    Vec2 position, velocity, acceleration;
    float32 angle;
} player = {};

float32 sum;
int n = 0;

#define A 1103515245
#define C 12345
#define M 2147483648

unsigned int seed = 0;

int rand() {
    seed = (A * seed + C) % M;
    return seed;
}

void srand(unsigned int new_seed) {
    seed = new_seed;
}

void spawnAsteroid() {
    float32 angle = ((float32)(rand() % 360));
    Vec2 spawnPoint = Vec2Rotate(
        (Vec2){window.width, 0},
        (Vec2){
            (float32)window.width / 2,
            (float32)window.height / 2},
        angle);

    Vec2 velocity = Vec2Rotate(
        (Vec2){-100, 0},
        (Vec2){0, 0},
        angle);

    AsteroidListPush(&asteroids, (Asteroid){
                                     .angle = 0,
                                     .position = spawnPoint,
                                     .velocity = velocity,
                                 });
}

void update(float32 dt) {
    Gamepad* gamepad = WindowGetGamepad(&window, 0);
    Vec2 accel = {0, 0};
    if (gamepad != nil) {
        accel = (Vec2){
            GamepadAxisValue(gamepad, GamepadAxis_LeftStickX),
            -GamepadAxisValue(gamepad, GamepadAxis_LeftStickY),
        };
    }
    if (accel.X == 0 && accel.Y == 0) {
        if (KeyPressed(&window, Key_A)) accel.X--;
        if (KeyPressed(&window, Key_D)) accel.X++;
        if (KeyPressed(&window, Key_W)) accel.Y--;
        if (KeyPressed(&window, Key_S)) accel.Y++;
    }
    player.acceleration = Vec2Scale(accel, 500, 500);

    player.velocity = Vec2Add(
        player.velocity,
        Vec2Scale(
            player.acceleration,
            dt, dt));

    player.position = Vec2Add(
        player.position,
        Vec2Scale(
            player.velocity,
            dt, dt));

    if (player.position.X > window.width) {
        player.velocity.X *= -0.45;
        player.position.X = window.width;
    }
    if (player.position.X < 0) {
        player.velocity.X *= -0.45;
        player.position.X = 0;
    }
    if (player.position.Y > window.height) {
        player.velocity.Y *= -0.45;
        player.position.Y = window.height;
    }
    if (player.position.Y < 0) {
        player.velocity.Y *= -0.45;
        player.position.Y = 0;
    }
    // if (player.position.Y > window.height || player.position.Y < 0) player.velocity.Y *= -0.75;

    if (player.acceleration.X != 0 || player.acceleration.Y != 0)
        player.angle = atan2f(player.acceleration.Y, player.acceleration.X);

    if (WindowTime() > asteroidSpawnTimer) {
        println("spawned");
        spawnAsteroid();
        asteroidSpawnTimer = WindowTime() + 250;
        if (asteroids.len > 15) {
            AsteroidListShift(&asteroids, nil);
        }
    }

    for (int i = 0; i < asteroids.len; i++) {
        Asteroid* asteroid = AsteroidListGet(&asteroids, i);
        asteroid->position = Vec2Add(
            asteroid->position,
            Vec2Scale(asteroid->velocity,
                dt, dt));

        asteroid->angle += PI * dt / 5;

        drawShapeTransformed(
            &assets_asteroid__svg,
            asteroid->position,
            1,
            asteroid->angle,
            ColorHex(0xFFFF0000));

        nvgBeginPath(vg);
        nvgMoveTo(vg, asteroid->position.X, asteroid->position.Y);
        nvgLineTo(vg, asteroid->position.X + asteroid->velocity.X, asteroid->position.Y + asteroid->position.Y);
        nvgStrokeColor(vg, (NVGcolor){{{0, 1, 0, 1}}});
        nvgStroke(vg);
    }
}

void draw() {
    drawShapeTransformed(
        &assets_ship__svg,
        player.position,
        0.25, player.angle + PI / 2,
        ColorHex(0xFFABCDEF));
}

bool loop(float32 dt) {
    sum += dt;
    n++;
    GraphicsMakeCurrent(&window);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, window.width, window.height);

    nvgBeginFrame(vg, window.width, window.height, 1);
    {
        update(dt);
        draw();
    }
    nvgEndFrame(vg);
    return true;
}

#include "nanovg/nanovg.c"