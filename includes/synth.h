#ifndef Synth_H
#define Synth_H

#include <audio.h>

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
    } sine;
    struct SawOptions {
    } saw;
    struct SquareOptions {
    } square;
} OscillatorOptions;

typedef struct Oscillator {
    OscillatorType type;
    OscillatorOptions options;
} Oscillator;

void CreateSineOscillator(Oscillator* oscillator);

void OscillatorStream(Oscillator oscillator, float32* wave, int length);

#endif