#ifndef Audio_H
#define Audio_H

#include <math.h>
#include <types.h>

#ifndef AUDIO_SAMPLE_RATE
#define AUDIO_SAMPLE_RATE 44100
#endif  // AUDIO_SAMPLE_RATE

#ifndef AUDIO_BUFFER_SIZE
#define AUDIO_BUFFER_SIZE 8192
#endif  // AUDIO_BUFFER_SIZE

#ifndef AUDIO_BUFFER_COUNT
#define AUDIO_BUFFER_COUNT 2
#endif  // AUDIO_BUFFER_COUNT

// #ifndef AUDIO_CHANNEL_COUNT
#define AUDIO_CHANNEL_COUNT 1
// #endif

typedef struct AudioNative AudioNative;

// Audio is an interface to your platforms speakers
typedef struct Audio {
    AudioNative* native;
} Audio;

// AudioInit initializes the Audio interface for your current platform.
// It returns true when successful or false if there was an error.
bool AudioInit(Audio* audio);

// AudioAvailable returns the number of samples that are available to be written.
int AudioAvailable(Audio* audio);

// AudioWrite writes the soundwave from "wave" to the audio interface.
// "length" is the number of samples to be written.
void AudioWrite(Audio* audio, float32* wave, int length);

// AudioClose cleans up this audio interface. It should be called when you are finished using this audio interface.
void AudioClose(Audio* audio);

#endif  // Audio_H