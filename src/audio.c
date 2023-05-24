#include <stdlib.h>

#include "audio.h"
#include "consts.h"
#include "types.h"
#include "utils.h"

#if defined(PLATFORM_Windows)

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
            for (int j = 0; j < length; j++) {
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

#elif defined(PLATFORM_Linux)

#include <alsa/asoundlib.h>

struct AudioNative {
    snd_pcm_t* wave;
    int16 buffer[AUDIO_BUFFER_SIZE * AUDIO_CHANNEL_COUNT];
};

bool AudioInit(Audio* audio) {
    snd_pcm_t* wave;
    if (snd_pcm_open(&wave, "default", SND_PCM_STREAM_PLAYBACK, 0)) return false;
    if (snd_pcm_set_params(
            wave,
            SND_PCM_FORMAT_S16,
            SND_PCM_ACCESS_RW_INTERLEAVED,
            AUDIO_CHANNEL_COUNT,
            AUDIO_SAMPLE_RATE,
            1,
            100000) < 0) {
        snd_pcm_close(wave);
        return false;
    };

    audio->native = allocate(AudioNative);
    *audio->native = (AudioNative){
        .wave = wave,
    };

    return true;
}

void AudioClose(Audio *audio) {
    if (audio->native == nil) return;

    println("Closing audio");
    snd_pcm_close(audio->native->wave);
    free(audio->native);
    audio->native = nil;
}

int AudioAvailable(Audio* audio) {
    int available = snd_pcm_avail(audio->native->wave);
    if (available < 0) {
        snd_pcm_recover(audio->native->wave, available, 0);
        return 0;
    }
    return available;
}

void AudioWrite(Audio* audio, float32* wave, int length) {
    if (length > AUDIO_BUFFER_SIZE * AUDIO_CHANNEL_COUNT) {
        length = AUDIO_BUFFER_SIZE * AUDIO_CHANNEL_COUNT;
    }
    for (int i = 0; i < length; i++) {
        audio->native->buffer[i] = (int16)(wave[i] * 0x7FFF);
    }

    int err = snd_pcm_writei(
        audio->native->wave,
        audio->native->buffer,
        length);
    
    if (err < 0) {
        snd_pcm_recover(audio->native->wave, err, 0);
    }
}

#endif
