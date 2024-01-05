#ifndef Synth_H
#define Synth_H

// Work in progress, in-game synthesizer.
//
// Not currently useable.

#include "audio.h"

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
        void* opt;
    } sine;
    struct SawOptions {
        void* opt;
    } saw;
    struct SquareOptions {
        void* opt;
    } square;
} OscillatorOptions;

typedef struct SineOptions SineOptions;
typedef struct SawOptions SawOptions;
typedef struct SquareOptions SquareOptions;

typedef struct Oscillator {
    OscillatorType type;
    OscillatorOptions options;
} Oscillator;

void CreateSineOscillator(Oscillator* oscillator);

void OscillatorStream(Oscillator oscillator, float32* wave, int length);

#endif