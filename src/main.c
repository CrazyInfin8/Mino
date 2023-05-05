#include <math.h>
#include <utils.h>
#include <consts.h>
#include <audio.h>
#include <window.h>
#include <keyboard.h>
#include <mouse.h>

Window window;
Audio audio;



// typedef struct Synth {
//     Phasor phasor;
//     float32 notes[8];
//     Oscillator oscillator;
// } Synth;

// Synth synth = {
//     .phasor = {
//         .phaseInterval = 261.63 / AUDIO_SAMPLE_RATE,
//     },
//     .notes = {
//     },
//     .oscillator = {
//         .type = SineOscillator,
//     },
// };

int main(void) {
    println("Starting game");
    if (WindowInit(&window, (WindowConfig){"Mino Demo Game Window", 800, 600}) == false) {
        println("Could not open the window");
        return 1;
    }
    if (AudioInit(&audio) == false) {
        println("Could not open Audio");
        WindowClose(&window);
        return 1;
    }

    while (WindowUpdate(&window)) {
        int64 begin = WindowTime();

        // int availableAudio = AudioAvailable(&audio);
        // if (availableAudio > 0) {
        //     float32 buffer[AUDIO_BUFFER_SIZE];
        //     // PhasorStream(&synth.phasor, buffer, AUDIO_BUFFER_SIZE);

        //     // OscillatorStream(synth.oscillator, buffer, AUDIO_BUFFER_SIZE);

        //     AudioWrite(&audio, buffer, AUDIO_BUFFER_SIZE);
        // }

        if (KeyJustPressed(&window, KEY_A)) {
            println("Key A just pressed");
        }
        if (KeyJustReleased(&window, KEY_A)) {
            println("Key A just released");
        }
        for (int i = 0; i < 5; i++) {
            if (MouseJustPressed(&window, i)) {
                println("Mouse button %d just pressed", i);
            }
            if (MouseJustReleased(&window, i)) {
                println("Mouse button %d just released", i);
            }
        }
        rune key = KeyGetChar(&window);
        if (key != 0) {
            println("Printable key %lc received", key);
        }

        int64 now = WindowTime();
        if (now - begin < 1000 / 60 && 1000 / 60 - (now - begin) > 0) {
            WindowSleep(1000 / 60 - (now - begin));
        }
    }

    AudioClose(&audio);
    WindowClose(&window);
}