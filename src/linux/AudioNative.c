#include <stdlib.h>
#include <alsa/asoundlib.h>

#include "audio.h"
#include "consts.h"
#include "types.h"
#include "utils.h"

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

void AudioClose(Audio* audio) {
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