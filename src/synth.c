// Work in progress, in-game synthesizer.
//
// Not currently useable.

#include "synth.h"
#include "consts.h"
#include <math.h>

void WaveMultiply(float32 *wave, int length, float32 scale) {
    for (int i = 0; i < length; i++) {
        wave[i] *= scale;
    }
}

void PhasorSetFrequency(Phasor *phasor, float32 frequency) {
    phasor->phaseInterval = frequency / (float32)AUDIO_SAMPLE_RATE;
}

void PhasorStream(Phasor *phasor, float32 *wave, int length) {
    for (int i = 0; i < length; i++) {
        phasor->phase = fmodf(phasor->phase + phasor->phaseInterval, 1);
        wave[i] = phasor->phase * 2 - 1;
    }
}

void CreateSineOscillator(Oscillator *oscillator) {
    *oscillator = (Oscillator){
        .type = SineOscillator,
        .options = {
            .sine = (SineOptions){.opt = nil},
        },
    };
}

void OscillatorStream(Oscillator oscillator, float32 *wave, int length) {
    switch (oscillator.type) {
        case SineOscillator: {
            for (int i = 0; i < length; i++) {
                wave[i] = sinf((wave[i] + 1) * PI_2 / 2);
            }
        } break;

        case SawOscillator: {
            // for (int i = 0; i < length; i++) {
            //     wave[i] = wave[i] * 2 - 1;
            // }
            return;
        } break;

        case SquareOscillator: {
            for (int i = 0; i < length; i++) {
                wave[i] = (wave[i] > 0) ? 1 : -1;
            }
        }
    }
}