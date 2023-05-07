#include <stdlib.h>

#include "audio.h"
#include "consts.h"
#include "types.h"
#include "utils.h"

#if PLATFORM == Windows

#include <windows.h>
#include <mmsystem.h>

struct AudioNative {
    HWAVEOUT waveOut;
    WAVEHDR headers[AUDIO_BUFFER_COUNT];
    int16 buffers[AUDIO_BUFFER_COUNT][AUDIO_BUFFER_SIZE * AUDIO_CHANNEL_COUNT];
};

bool AudioInit(Audio *audio) {
    MMRESULT results;
    const WAVEFORMATEX wfx = {
        .wFormatTag = WAVE_FORMAT_PCM,
        .nChannels = AUDIO_CHANNEL_COUNT,
        .nSamplesPerSec = AUDIO_SAMPLE_RATE,
        .nAvgBytesPerSec = AUDIO_SAMPLE_RATE * AUDIO_CHANNEL_COUNT * sizeof(int16),
        .nBlockAlign = sizeof(int16) * AUDIO_CHANNEL_COUNT,
        .wBitsPerSample = sizeof(int16) * 8,
        .cbSize = 0,
    };

    AudioNative *native = (AudioNative *)malloc(sizeof(AudioNative));
    ZeroMemory(native, sizeof(AudioNative));
    audio->native = native;

    results = waveOutOpen(
        &native->waveOut,
        WAVE_MAPPER,
        &wfx,
        0,
        0,
        CALLBACK_NULL);

    if (results != MMSYSERR_NOERROR) {
        return false;
    }

    for (int i = 0; i < AUDIO_BUFFER_COUNT; i++) {
        println("Initializing Buffer: %d", i);
        native->headers[i] = (WAVEHDR){
            .lpData = (LPSTR)native->buffers[i],
            .dwBufferLength = AUDIO_BUFFER_SIZE * 2,
        };
        results = waveOutPrepareHeader(
            native->waveOut,
            &native->headers[i],
            sizeof(WAVEHDR));
        if (results != MMSYSERR_NOERROR) {
            println("Error in waveOutPrepareHeader");
        }

        results = waveOutWrite(native->waveOut,
            &native->headers[i],
            sizeof(WAVEHDR));

        if (results != MMSYSERR_NOERROR) {
            println("Error in waveOutWrite");
        }
    }

    return true;
}

int AudioAvailable(Audio *audio) {
    for (int i = 0; i < AUDIO_BUFFER_COUNT; i++) {
        if (audio->native->headers[i].dwFlags & WHDR_DONE) {
            return AUDIO_BUFFER_SIZE;
        }
    }
    return 0;
}

void AudioWrite(Audio *audio, float *wave, int length) {
    if (length == 0) {
        return;
    }

    if (length > AUDIO_BUFFER_SIZE) {
        length = AUDIO_BUFFER_SIZE;
    }

    for (int i = 0; i < AUDIO_BUFFER_COUNT; i++) {
        if (audio->native->headers[i].dwFlags & WHDR_DONE) {
            for (unsigned j = 0; j < length; j++) {
                audio->native->buffers[i][j] = (int16)(wave[j] * 0x7FFF);
            }

            waveOutWrite(
                audio->native->waveOut,
                &audio->native->headers[i],
                sizeof(WAVEHDR));
            return;
        }
    }
}

void AudioClose(Audio *audio) {
    waveOutClose(audio->native->waveOut);
    free(audio->native);
    audio->native = nil;
}

#elif PLATFORM == Linux

#endif
