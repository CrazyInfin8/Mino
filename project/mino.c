#if defined(PLATFORM_Linux)
#define Window MinoWindow
#endif

#include <math.h>

#ifndef Aff3_H
#define Aff3_H

#ifndef Types_H
#define Types_H

#include <stdbool.h>
#include <stddef.h>

typedef signed char int8;
typedef signed short int int16;
typedef signed long int int32;
typedef signed long long int int64;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef unsigned long long int uint64;

typedef unsigned int uint;
typedef uint8 byte;
typedef wchar_t rune;

typedef float float32;
typedef double float64;

#define nil ((void*)0)

#define PACK_ENUM __attribute__ ((__packed__))
#endif // Types_H
#ifndef Vec2_H
#define Vec2_H

#include <math.h>

#ifndef Consts_H
#define Consts_H


// `PI` is π, a mathematical constant useful for rotation and trigonometry.
//
// This is equivalent to 180 degrees of rotation or halfway around a circle.
extern const float32 PI;

// `PI_2` is π multiplied by 2.
//
// This is equivalent to 360 degrees of rotation or the full rotation of a circle.
extern const float32 PI_2;

// `TO_DEG` helps to convert radians to degrees by simply multiplying radians
// with this value.
//
// It is equal to `180 / π`.
extern const float32 TO_DEG;

// `TO_RAD` helps to convert degrees to radians by simply multiplying degrees
// with this value.
//
// It is equal to `π / 180`.
extern const float32 TO_RAD;

#endif  // Consts_H


// `Vec2` implements a vector/point in 2D space.
//
// Note: None of the `Vec2` functions modify the `Vec2`. Instead if returns a
// new `Vec2` which you can use to set the original `Vec2` if you want.
typedef struct Vec2 {
    float32 X, Y;
} Vec2;

typedef struct Aff3 Aff3;

// `Vec2TransformAff3` applies the matrix's transformations to the `Vec2`.
//
// This applies all transformations applied the the matrix onto this `Vec2`
Vec2 Vec2TransformAff3(Vec2 this, Aff3 aff3);

// `Vec2Rotate` rotates the `Vec2` around the specified pivot point by the
// `angle` in degrees.
Vec2 Vec2Rotate(Vec2 this, Vec2 pivot, float32 angle);

#endif  // Vec2_H

// `Aff3` implements a 3x3 affine transformation matrix where the bottom row is
// always implicitly `[0, 0, 1]`.
//
// Note: None of `Aff3` functions modify the `Aff3`. Instead it returns a new
// `Aff3` which you can use to set the original `Aff3 if you want.
typedef struct Aff3 {
    float32 A, B, C, D, TX, TY;
} Aff3;

// `Aff3Identity` returns an identity matrix.
//
// This is defined as: `[[1, 0, 0], [0, 1, 0], [0, 0, 1]]`.
Aff3 Aff3Identity();

// `Aff3Concat` applies the transformations from `aff3` onto `this` matrix.
//
// The order of matrices you concatenate matters. For example, concatenating a
// rotation matrix to a translation matrix may have different results than
// concatenating those same translation and a rotation matrix.
Aff3 Aff3Concat(Aff3 this, Aff3 aff3);

// `Aff3Invert` reverses the transformations applied from `this` matrix.
//
// Objects transformed by the original matrix will revert back to their original
// transformations when you apply this transformation.
Aff3 Aff3Invert(Aff3 this);

Aff3 Aff3Rotate(Aff3 this, float32 angle);

Aff3 Aff3Scale(Aff3 this, float32 x, float32 y);

Aff3 Aff3Translate(Aff3 this, float32 x, float32 y);

// `Aff3TransformVec2` applies the matrix's transformations to the `Vec2`.
//
// This applies all transformations applied to this matrix onto that `Vec2`.
Vec2 Aff3TransformVec2(Aff3 this, Vec2 vec);

// `Aff3Print` prints the matrix in a neat way. Useful for debugging.
//
// Note: if `NOPRINT` is defined, this may not print to the console.
void Aff3Print(Aff3 this);

#endif  // Aff3_H
#ifndef Utils_H
#define Utils_H

#include <stddef.h>

#ifdef NOPRINT
#define print(format, ...) \
    do {                   \
    } while (false)
#define println(format, ...) \
    do {                     \
    } while (false)
#else
int printf(const char* restrict format, ...);

// `print` prints to the standard output (terminal/command prompt).
//
// if `NOPRINT` is defined, this doesn't do anything and simply ignores passed
// arguments.
#define print(format, ...) printf(format __VA_OPT__(, __VA_ARGS__))

// `print` prints to the standard output (terminal/command prompt) printing a
// new line afterwards. `format` should be a string literal or constant.
//
// if `NOPRINT` is defined, this doesn't do anything and simply ignores passed
// arguments.
#define println(format, ...) printf("[%s:%d] ~> " format "\n", __FILE__, __LINE__ __VA_OPT__(, __VA_ARGS__)) // __println(__FILE__, __LINE__, format, __VA_ARGS__)
#define __println(file, line, format, ...) 
#endif

// `len` gets the length of a constant width array.
//
// This should not be used with pointers since the size of a an array from a
// pointer might not be known at compile time.
#define len(array, type) (sizeof(array) / sizeof(type))

void copy(const void* restrict src, void* restrict dst, size_t size);

// `copyPad` copies as much from `src` into `dst` up to the size specified. If
// `src` is smaller than size, the rest of `dst` is padded with null bytes 
// ('\0')
void copyPad(const void* restrict src, void* restrict dst, size_t size);

// `allocate` easily allocates and initializes to zero a chunk of memory the
// size of the specified `type`.
#define allocate(type) (type*)calloc(1, sizeof(type))
#define allocateN(type, count) (type*)calloc(count, sizeof(type))
#define reallocateN(pointer, type, count) (type*)realloc(pointer, sizeof(type) * count)

// `bitSet` checks that the bit offset by `index` is set in `bits`.
bool bitSet(uint64 bits, uint8 index);

// `bitSet` checks that the bit offset by `index` is not set in `bits`.
bool bitUnset(uint64 bits, uint8 index);

// `setBit` sets the bit offset by `index` to 1.
uint64 setBit(uint64 bits, uint8 index);

// `setBit` sets the bit offset by `index` to 0.
uint64 unsetBit(uint64 bits, uint8 index);

// `clamp` constrains value to be between the `min` and `max`.
float32 clamp(float32 value, float32 min, float32 max);

bool hasPrefix(const char* text, const char* prefix);

#endif  // Utils_H

static const Aff3 aff3 = (Aff3){.A = 1, .B = 0, .TX = 0, .C = 0, .D = 1, .TY = 0};

extern inline Aff3 Aff3Identity() {
    return aff3;
}

Aff3 Aff3Concat(Aff3 this, Aff3 aff3) {
    return (Aff3){
        .A = this.A * aff3.A + this.B * aff3.C,
        .B = this.A * aff3.B + this.B * aff3.D,
        .C = this.C * aff3.A + this.D * aff3.C,
        .D = this.C * aff3.B + this.D * aff3.D,
        .TX = this.TX * aff3.A + this.TY * aff3.C + aff3.TX,
        .TY = this.TX * aff3.B + this.TY * aff3.D + aff3.TY,
    };
}

Aff3 Aff3Invert(Aff3 this) {
    float32 norm = this.A * this.D - this.B * this.C;
    if (norm == 0) {
        return (Aff3){.A = 0, .B = 0, .C = 0, .D = 0, .TX = -this.TX, .TY = -this.TY};
    }
    return (Aff3){
        .A = this.D * norm,
        .B = this.B * -norm,
        .C = this.C * -norm,
        .D = this.D * norm,
        .TX = -this.A * this.TX - this.C * this.TY,
        .TY = -this.B * this.TX - this.D * this.TY,
    };
}

Aff3 Aff3Rotate(Aff3 this, float32 angle) {
    angle *= TO_RAD;
    float32 sin = sinf(angle), cos = cosf(angle);
    return (Aff3){
        .A = this.A * cos - this.B * sin,
        .B = this.A * sin + this.B * cos,
        .C = this.C * cos - this.D * sin,
        .D = this.C * sin + this.D * cos,
        .TX = -this.A * this.TX - this.C * this.TY,
        .TY = -this.B * this.TX - this.D * this.TY,
    };
}

Aff3 Aff3Scale(Aff3 this, float32 x, float32 y) {
    return (Aff3){
        .A = this.A * x,
        .B = this.B * y,
        .C = this.C * x,
        .D = this.D * y,
        .TX = this.TX * x,
        .TY = this.TY * y,
    };
}

Aff3 Aff3Translate(Aff3 this, float32 x, float32 y) {
    return (Aff3){
        .A = this.A,
        .B = this.B,
        .C = this.C,
        .D = this.D,
        .TX = this.TX + x,
        .TY = this.TX + y,
    };
}

Vec2 Aff3TransformVec2(Aff3 this, Vec2 vec) {
    return (Vec2){
        .X = this.A * vec.X + this.C * vec.Y + this.TX,
        .Y = this.B * vec.X + this.D * vec.Y + this.TY,
    };
}

#ifdef NOPRINT
extern inline void Aff3Print(Aff3) {}
#else
void Aff3Print(Aff3 this) {
    println(
        "Matrix {\n\tA: %.6f, B: %.6f, TX: %.6f,\n\tC: %.6f, D: %.6f, TY: %.6f,\n}",
        this.A,
        this.B,
        this.TX,
        this.C,
        this.D,
        this.TY);
}
#endif
#include <stdlib.h>

#ifndef Audio_H
#define Audio_H


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

// `AudioNative` is the platform's native audio interface.
//
// It is not meant to be interacted with directly.
typedef struct AudioNative AudioNative;

// `Audio` is an interface to stream sound to your devices speakers .
typedef struct Audio {
    AudioNative* native;
} Audio;

// `AudioInit` initializes the Audio interface for your current platform.
// It returns true when successful or false if there was an error.
//
// Remember to close finished `Audio` interfaces with `AudioClose`
bool AudioInit(Audio* audio);

// `AudioClose` cleans up this audio interface.
//
// It should be called when you are finished using this audio interface.
void AudioClose(Audio* audio);

// `AudioAvailable` returns the number of samples that are available to be
// written.
int AudioAvailable(Audio* audio);

// `AudioWrite` writes the soundwave from `wave` to the audio interface.
// `length` is the number of samples to be written.
void AudioWrite(Audio* audio, float32* wave, int length);


#endif  // Audio_H

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


// `PI` is π, a mathematical constant useful for rotation and trigonometry.
//
// This is equivalent to 180 degrees of rotation or halfway around a circle.
const float32 PI = 3.1415926535897932384626433832795028841971693993751058209749;

// `PI_2` is π multiplied by 2.
//
// This is equivalent to 360 degrees of rotation or the full rotation of a circle.
const float32 PI_2 = 6.28318530717958647692528676655900576839433879875021164194;

// `TO_DEG` helps to convert radians to degrees by simply multiplying radians
// with this value.
//
// It is equal to `180 / π`.
const float32 TO_DEG = 57.29577951308232087679815481410517033240547246656432154;

// `TO_RAD` helps to convert degrees to radians by simply multiplying degrees
// with this value.
//
// It is equal to `π / 180`.
const float32 TO_RAD = 0.017453292519943295769236907684886127134428718885417254;
#ifndef Gamepad_H
#define Gamepad_H

#ifndef List_H
#define List_H


// `List` is a simple, dynamically allocated, array of contiguous memory.
//
// Before you use a new list, you should call `ListInit` to initialize the size
// of each element. Once you do, you can add, index, and remove elements of that
// size.
//
// As memory is dynamically allocated, it is a good idea to call `ListFree` when
// you are done with this list to free up it's memory. Note that after you call
// `ListFree`, you can still use the list like normal as it frees the data and
// zeroes the capacity but the list is still valid.
//
// See also the `DecList` and `DefList` macros for ways to create lists specific
// to a certain type.
typedef struct List {
    byte* const data;
    const int itemSize;
    const int len;
    const int cap;
} List;

// `ListInit` initializes a blank list with the given `itemSize`. You can
// specify the initial length and capacity of the list to preallocate memory for
// this list.
//
// This returns true if the allocation was successful, else it returns false.
bool ListInit(List* list, int itemSize, int len, int cap);

// `ListGet` retrieves an element from this list at the specified index. This
// returns nil if the index is out of range.
//
// Note: The pointer returned from `ListGet` is owned by this list. It may
// change, especially if you shift, unshift, pop, or grow this list.
void* ListGet(List* list, int index);

// `ListFree` discards all elements from this list and frees it's memory. Even
// though the memory is freed, the list is still valid to use without
// initializing again; it's capacity is simply set to zero (which may mean
// other operations may need to grow this list again).
//
// You should call this when you are finished using this list.
void ListFree(List* list);

// `ListGrow` tries to grow this list to fit the amount of elements specified by
// `newCap`. This does not modify the length of the list, rather it allocates
// more memory to fit more elements, copies the old data, then changes the lists
// capacity. This allows you to allocate extra memory to minimize calls to the
// allocator.
//
// This returns true if the allocation was successful, else it returns false. If
// `newCap` is smaller than the current capacity, no changes are made and this
// returns true.
bool ListGrow(List* list, int newCap);

// `ListPush` tries to append a new entry and copies the data located by `item`
// into this list. If the list's capacity is too small to accommodate a new
// entry, the list is automatically grown by a scale factor of `1.5 * list.cap`.
//
// This returns true if appending the entry was successful. If the list was
// unable to allocate a new entry then this returns false.
bool ListPush(List* list, void* item);

// `ListPop` removes an entry from the end of the list. if `dest` is not nil,
// the entry is copied to that location.
//
// This returns true if it actually removed an element. It returns false if
// there are no elements to remove.
bool ListPop(List* list, void* dest);

// `ListUnshift` tries to prepend a new entry and copies the data located by
// `item` into this list. If the list's capacity is too small to accommodate a
// new entry, the list is automatically grown by a scale factor of
// `1.5 * list.cap`. This is slightly more expensive than pushing as the older
// data needs to be copied over to accommodate the new entry.
//
// This returns true if prepending the entry was successful. If the list was
// unable to allocate a new entry then this returns false.
bool ListUnshift(List* list, void* item);

// `ListPop` removes an entry from the beginning of the list. if `dest` is not
// nil, the entry is copied to that location. This is slightly more expensive
// than removing from the beginning as the remaining data needs to be copied
// over.
//
// This returns true if it actually removed an element. It returns false if
// there are no elements to remove.
bool ListShift(List* list, void* dest);

// `DecList` declares a list with entries of type: `Type` and type name: `Name`.
// This defines the type but forward declares the functions without
// implementing or defining them yet (see `DefList`). This allows you to declare
// lists as a struct instead of a pointer as well as get access to list length
// and capacity but might necessitate the use of include guards. or
// `#pragma once`.
//
// This is intended to be placed in header files so as not to duplicate the
// definition of each function.
//
// The functions declared by this macro wrap the ordinary `List` type but ensure
// that the size of entries match the size of `Type` and provide some ergonomics
// such as passing entries by the entry type instead of by `void*`.
//
// Make sure you call the init function before you use the list to ensure that
// the size of each entry is properly set.
#define DecList(Type, Name)                        \
    typedef struct Name {                          \
        Type* const data;                          \
        const int itemSize;                        \
        const int len;                             \
        const int cap;                             \
    } Name;                                        \
    bool Name##Init(Name* list, int len, int cap); \
    Type* Name##Get(struct Name* list, int index); \
    void Name##Free(Name* list);                   \
    bool Name##Grow(Name* list, int newCap);       \
    bool Name##Push(Name* list, Type item);        \
    bool Name##Pop(Name* list, Type* dest);        \
    bool Name##Unshift(Name* list, Type item);     \
    bool Name##Shift(Name* list, Type* dest);

// `DefList` defines the functions of a list declared with `DecList`. The
// functions defined by this macro wrap the ordinary list functions but ensure
// that the size of entries math the size of `Type` and provide some ergonomics
// such as passing entries by the entry type instead of by `void*`.
//
// This is intended to be placed in a C function to avoid duplicating the
// function definitions but it should come after you have declared the list with
// `DecList`.
//
// Make sure you call the init function before you use the list to ensure that
// the size of each entry is properly set.
#define DefList(Type, Name)                                       \
    extern inline bool Name##Init(Name* list, int len, int cap) { \
        return ListInit((List*)list, sizeof(Type), len, cap);     \
    }                                                             \
    extern inline Type* Name##Get(Name* list, int index) {        \
        return (Type*)ListGet((List*)list, index);                \
    }                                                             \
    extern inline void Name##Free(Name* list) {                   \
        ListFree((List*)list);                                    \
    }                                                             \
    extern inline bool Name##Grow(Name* list, int newCap) {       \
        return ListGrow((List*)list, newCap);                     \
    }                                                             \
    extern inline bool Name##Push(Name* list, Type item) {        \
        return ListPush((List*)list, &item);                      \
    }                                                             \
    extern inline bool Name##Pop(Name* list, Type* dest) {        \
        return ListPop((List*)list, dest);                        \
    }                                                             \
    extern inline bool Name##Unshift(Name* list, Type item) {     \
        return ListUnshift((List*)list, &item);                   \
    }                                                             \
    extern inline bool Name##Shift(Name* list, Type* dest) {      \
        return ListShift((List*)list, dest);                      \
    }

#endif  // List_H

typedef struct Window Window;

typedef struct GamepadNative GamepadNative;

// `GamepadButton` represents the buttons on a standard gamepad controller.
//
// This follows the XBox controller layout unlike a PlayStation controller that
// utilizes symbols for it's buttons.
//
// If you were using a PlayStation controller (and Mino was able to register and
// understand it's input):
//
//  - the X symbol usually corresponds to the A button.
//  - the circle symbol usually corresponds to the B button.
//  - the square symbol usually corresponds to the X button.
//  - the triangle symbol usually corresponds to the Y button.
//
// Note: The ordering of `GamepadButton_L2`, `GamepadButton_R2`, and `GamepadButton_Home` is
// intentionally placed at the end because they may not have corresponding
// mappings on each platforms. L2 and R2 refer to the triggers which are
// usually analog axes. You can these values to check and see whether these are
// pressed beyond the set deadzone. `GamepadButton_Home` might not be available on all
// platforms as the system may use them to exit the program to the systems home
// menu (For example on consoles like XBox, PlayStation, but the Windows XInput
// API also doesn't allow access to this button).
typedef enum PACK_ENUM GamepadButton {
    GamepadButton_Up,
    GamepadButton_Down,
    GamepadButton_Left,
    GamepadButton_Right,

    GamepadButton_A,
    GamepadButton_B,
    GamepadButton_X,
    GamepadButton_Y,

    GamepadButton_Start,
    GamepadButton_Select,

    GamepadButton_L1,
    GamepadButton_R1,
    GamepadButton_L3,
    GamepadButton_R3,

    // `GamepadButton_Home`
    //
    // Note: This is not might not be accessible on every platforms yet.
    // Consoles like XBox and PlayStation may reserve these but the Windows
    // XInput API also doesn't allow access to this button.
    GamepadButton_Home,

    // `GamepadButton_L2` is the left trigger. This is often an analog value however
    // you can check whether this is pressed beyond the deadzone.
    GamepadButton_L2,
    // `GamepadButton_R2` is the right trigger. This is often an analog value however
    // you can check whether this is pressed beyond the deadzone.
    GamepadButton_R2,

    GamepadButton_LB = GamepadButton_L1,
    GamepadButton_RB = GamepadButton_R1,
    GamepadButton_LT = GamepadButton_L2,
    GamepadButton_RT = GamepadButton_R2,

    GamepadButton_LeftStick = GamepadButton_L3,
    GamepadButton_RightStick = GamepadButton_R3,

    // `GamepadButton_Count` is not a gamepad button. It denotes the number of
    // gamepad buttons available to check.
    GamepadButton_Count,
    GamepadButton_Invalid = ~0,
} GamepadButton;

// `GamepadAxis` represents an analog axis on a standard gamepad controller.
//
// A standard gamepad often has 2 joysticks (which each have 2 directions) and 2
// analog triggers.
typedef enum PACK_ENUM GamepadAxis {
    GamepadAxis_LeftStickX,
    GamepadAxis_LeftStickY,
    GamepadAxis_RightStickX,
    GamepadAxis_RightStickY,
    GamepadAxis_LeftTrigger,
    GamepadAxis_RightTrigger,
    // `GamepadAxis_Count` is not a gamepad axis. It denotes the number of
    // gamepad axis available to check.
    GamepadAxis_Count,
} GamepadAxis;

// `Gamepad` represents a single player's gamepad.
//
// Often times, a device or game can support more than 1 gamepad connected to
// have multiple players playing the same game.
typedef struct Gamepad {
    bool connected;

    float32 axes[GamepadAxis_Count];
    float32 pAxes[GamepadAxis_Count];

    uint32 buttons;
    uint32 pButtons;

    float32 leftMotor;
    float32 pLeftMotor;
    float32 rightMotor;
    float32 pRightMotor;

    int playerID;
    GamepadNative* native;
} Gamepad;

DecList(Gamepad, GamepadList);

// `WindowGetGamepad` retrieves a players gamepad given their playerID (most
// often 1-4).
//
// If the playerID is more than supported, this function returns `nil`.
//
// Note: The returned pointer is still owned by Mino and it's location can be
// changed. Instead, you may want to hold onto the playerID and retrieve the
// players gamepad on each update.
Gamepad* WindowGetGamepad(Window* window, int playerID);

// `WindowGetFirstConnectedGamepad` attempts to retrieve the first gamepad that
// was connected.
//
// If no gamepads have been connected, this returns `nil`. The gamepad returned
// can also disconnect, but this function will still return that gamepad.
//
// Note: The returned pointer is still owned by Mino and it's location can be
// changed.
Gamepad* WindowGetFirstConnectedGamepad(Window* window);

// `GamepadCount` returns the number of gamepads the window is keeping track of.
//
// Not all gamepads counted may be connected.
int GamepadCount(Window* window);

// `GamepadConnected` checks if the current gamepad is connected.
bool GamepadConnected(Gamepad* gamepad);

// `GamepadButtonPressed` returns true if the specified button on the current
// gamepad is pressed.
//
// If the gamepad is not connected, this will always return false.
bool GamepadButtonPressed(Gamepad* gamepad, GamepadButton button);

// `GamepadButtonJustPressed` returns true if the specified button on the
// current gamepad has just been pressed since the last call to `WindowUpdate`.
//
// If the gamepad is not connected, this will always return false.
bool GamepadButtonJustPressed(Gamepad* gamepad, GamepadButton button);

// `GamepadButtonJustReleased` returns true if the specified button on the
// current gamepad has just been released since the last call to `WindowUpdate`.
//
// If the gamepad is not connected, this will always return false.
bool GamepadButtonJustReleased(Gamepad* gamepad, GamepadButton button);

// `GamepadAxisValue` returns the analog value of the current gamepad's analog
// axis.
//
// The value is normalized so joysticks range from -1 to 1 in both directions
// (horizontal and vertical) and triggers range from 0 to 1.
//
// If the gamepad is not connected, this will always return 0.
float32 GamepadAxisValue(Gamepad* gamepad, GamepadAxis axis);

// `GamepadSetVibration` sets the speed for the vibration motors.
//
// The values for left and right range from 0 (no vibration) to 1 (full
// vibration).
//
// The left and right vibration motor may not be the same. For example, on XBox
// controllers and PS4 DualShocks, the left motor is for low frequency
// vibrations while the right motor is for high frequency vibrations.
void GamepadSetVibration(Gamepad* gamepad, float32 leftMotor, float32 rightMotor);

#endif  // Gamepad_H
#ifndef Window_H
#define Window_H

#ifndef Keyboard_H
#define Keyboard_H


typedef struct Window Window;

// `Key` represents a keyboard key code. Each key code corresponds to a key on a
// keyboard.
typedef enum Key Key;

// `KeyMod` represents modifier keys that could change or modify the
// behavior of other certain gestures.
typedef enum KeyMod KeyMod;

// `KeyPressed` returns true if the current key is pressed.
bool KeyPressed(Window* window, Key key);

// `KeyJustPressed` returns true if the specified key code has just been pressed
// since the last call to `WindowUpdate`
bool KeyJustPressed(Window* window, Key key);

// `KeyJustReleased` returns true if the specified key code has just been
// release since the last call to `WindowUpdate`.
bool KeyJustReleased(Window* window, Key key);

// `KeyModSet` checks that the modifier is currently enabled.
bool KeyModSet(Window* window, KeyMod modifier);

// `KeyGetChar` returns the character that the combinations of pressed keys
// would produce.
rune KeyGetChar(Window* window);

enum PACK_ENUM KeyMod {
    KeyMod_Ctrl = (1 << 0),
    KeyMod_Shift = (1 << 1),
    KeyMod_Alt = (1 << 2),
    KeyMod_Win = (1 << 3),
    KeyMod_CapsLock = (1 << 4),
    KeyMod_ScrollLock = (1 << 5),
    KeyMod_NumLock = (1 << 6),
};

enum PACK_ENUM Key {
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,
    Key_LeftAlt,
    Key_LeftOption = Key_LeftAlt,
    Key_RightAlt,
    Key_RightOption = Key_RightAlt,
    Key_DownArrow,
    Key_LeftArrow,
    Key_RightArrow,
    Key_UpArrow,
    Key_Tilde,
    Key_Backslash,
    Key_Backspace,
    Key_LeftBracket,
    Key_RightBracket,
    Key_CapsLock,
    Key_Comma,
    Key_Menu,
    Key_LeftCtrl,
    Key_RightCtrl,
    Key_Delete,
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,
    Key_End,
    Key_Enter,
    Key_Equal,
    Key_Escape,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_Home,
    Key_Insert,
    Key_LeftWin,
    Key_LeftSuper = Key_LeftWin,
    Key_LeftCommand = Key_LeftWin,
    Key_RightWin,
    Key_RightSuper = Key_RightWin,
    Key_RightCommand = Key_RightWin,
    Key_Minus,
    Key_NumLock,
    Key_NumPad0,
    Key_NumPad1,
    Key_NumPad2,
    Key_NumPad3,
    Key_NumPad4,
    Key_NumPad5,
    Key_NumPad6,
    Key_NumPad7,
    Key_NumPad8,
    Key_NumPad9,
    Key_NumPadAdd,
    Key_NumPadDecimal,
    Key_NumPadDivide,
    Key_NumPadEnter,
    Key_NumPadEqual,
    Key_NumPadMultiply,
    Key_NumPadSubtract,
    Key_PageDown,
    Key_PageUp,
    Key_Pause,
    Key_Period,
    Key_PrintScreen,
    Key_Quote,
    Key_ScrollLock,
    Key_Semicolon,
    Key_LeftShift,
    Key_RightShift,
    Key_Slash,
    Key_Space,
    Key_Tab,
    Key_Alt,
    Key_Option = Key_Alt,
    Key_Ctrl,
    Key_Shift,
    Key_Win,
    Key_Super = Key_Win,
    Key_Command = Key_Win,

    // `Key_COUNT` is not a key code. It denotes the number of key codes
    // available to check.
    Key_Count,

    // Used internally to denote an unsupported key code.
    Key_Invalid = ~0,
};

#endif  // Keyboard_H

// `WindowNative` is the platforms native implementation of a window.
//
// It is not meant to be interacted with directly.
typedef struct WindowNative WindowNative;

// `WindowConfig` specifies settings used to create and initialize the window.
typedef struct WindowConfig {
    const char* title;
    const int width, height;
} WindowConfig;

// `Window` is the primary way to draw content to the screen and process user
// input.
typedef struct Window {
    bool keyPressed[Key_Count];
    bool pKeyPressed[Key_Count];
    byte KeyMod;
    rune keyChar;

    int width, height;

    int mouseX, mouseY;
    int pMouseX, pMouseY;

    int scrollX, scrollY;
    int pScrollX, pScrollY;

    byte mousePressed;
    byte pMousePressed;

    GamepadList gamepads;

    WindowNative* native;
} Window;

// `WindowInit` Initializes and creates a new window with the settings in
// `config`
//
// Remember to close finished `Window`s with `WindowClose`
bool WindowInit(Window* window, WindowConfig config);

// `WindowUpdate` processes user input as well as updates the content of the
// window with previous graphic draw calls. This should be called every frame.
bool WindowUpdate(Window* window);

// `WindowClose` cleans up this window.
//
// It should be called when you are finished using this `Window`.
void WindowClose(Window* window);

// `WindowTime` milliseconds since the platforms start time.
//
// You can take readings of time from the start and end of a frame to figure out
// how long that frame has taken to finish.
int64 WindowTime();

// `WindowSleep` sleeps for the specified number of milliseconds.
//
// Currently, `WindowUpdate` does not wait until the next frame to return.
// Instead it may run without delay which can be unnecessarily taxing on your
// computer's resources. Finding the correct amount of time to sleep and calling
// this function can help prevent the computer from doing extra unnecessary
// work by sleeping.
void WindowSleep(int64 milliseconds);

#endif  // Window_H

DefList(Gamepad, GamepadList);

bool GamepadConnected(Gamepad* gamepad) {
    return gamepad->connected;
}

bool GamepadButtonPressed(Gamepad* gamepad, GamepadButton button) {
    if (GamepadConnected(gamepad)) {
        return bitSet(gamepad->buttons, button);
    }
    return false;
}

bool GamepadButtonJustPressed(Gamepad* gamepad, GamepadButton button) {
    if (GamepadConnected(gamepad)) {
        bool pressed = bitSet(gamepad->buttons, button);
        return pressed && (pressed != bitSet(gamepad->pButtons, button));
    }
    return false;
}

bool GamepadButtonJustReleased(Gamepad* gamepad, GamepadButton button) {
    if (GamepadConnected(gamepad)) {
        bool released = bitUnset(gamepad->buttons, button);
        return released && (released != bitUnset(gamepad->pButtons, button));
    }
    return false;
}

float32 GamepadAxisValue(Gamepad* gamepad, GamepadAxis axis) {
    if (GamepadConnected(gamepad)) {
        return gamepad->axes[axis];
    }
    return false;
}

void GamepadSetVibration(Gamepad* gamepad, float32 leftMotor, float32 rightMotor) {
    gamepad->leftMotor = clamp(leftMotor, 0, 1);
    gamepad->rightMotor = clamp(rightMotor, 0, 1);
}
#include <stdlib.h>
#include <string.h>


bool ListInit(List* list, int itemSize, int len, int cap) {
    cap = cap < len ? len : cap;
    void* ptr = nil;
    if (cap > 0) {
        ptr = (byte*)malloc(itemSize * cap);
        if (ptr == nil) return false;
        memset(ptr, 0, itemSize * cap);
    }
    memcpy(list,
        &(List){
            .data = ptr,
            .itemSize = itemSize,
            .cap = cap,
            .len = len,
        },
        sizeof(List));
    return true;
}

void* ListGet(List* list, int index) {
    if (index >= list->cap || index < 0) return nil;
    return &list->data[index * list->itemSize];
}

void ListFree(List* list) {
    if (list->data == nil) return;
    free(list->data);

    memcpy(list,
        &(List){
            .data = nil,
            .itemSize = list->itemSize,
            .cap = 0,
            .len = 0,
        },
        sizeof(List));
}

bool ListGrow(List* list, int newCap) {
    if (newCap < list->cap) return true;
    byte* newPtr = (byte*)malloc(newCap * list->itemSize);
    if (newPtr == nil) return false;
    memset(newPtr, 0, newCap * list->itemSize);
    memcpy(newPtr, list->data, list->cap * list->itemSize);
    free(list->data);
    memcpy(list,
        &(List){
            .data = newPtr,
            .itemSize = list->itemSize,
            .cap = newCap,
            .len = list->len,
        },
        sizeof(List));
    return true;
}

bool ListPush(List* list, void* item) {
    const int index = list->len * list->itemSize;
    if (list->len + 1 > list->cap) {
        int newCap = (list->len * 3) / 2 + 1;
        if (ListGrow(list, newCap) == false) return false;
    }
    memcpy(&list->data[index], item, list->itemSize);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len + 1,
        },
        sizeof(List));
    return true;
}

bool ListPop(List* list, void* dest) {
    if (list->len == 0) return false;
    const int index = list->len * list->itemSize;
    if (dest != nil) memcpy(dest, &list->data[index], list->itemSize);
    // zero out removed element
    memset(&list->data[index], 0, list->itemSize);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len - 1,
        },
        sizeof(List));
    return true;
}

bool ListUnshift(List* list, void* item) {
    if (list->len + 1 > list->cap) {
        int newCap;
        if (list->cap > 0)
            newCap = (list->len * 3) / 2;
        else
            newCap = 1;
        if (ListGrow(list, newCap) == false) return false;
    }
    memmove(&list->data[list->itemSize], list->data, list->itemSize * list->len);
    memcpy(list->data, item, list->itemSize);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len + 1,
        },
        sizeof(List));
    return true;
}

bool ListShift(List* list, void* dest) {
    if (list->len == 0) return false;
    if (dest != nil) memcpy(dest, list->data, list->itemSize);
    memmove(list->data, &list->data[list->itemSize], list->itemSize * list->len);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len + 1,
        },
        sizeof(List));
    // clean end of list
    memset(&list->data[list->len * list->itemSize], 0, list->itemSize);
    return true;
}

#include <math.h>

#ifndef Synth_H
#define Synth_H


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

void CreateSineOscillator(Oscillator* oscillator)  {
    *oscillator = (Oscillator){
        .type = SineOscillator,
        .options = {
            .sine = {

            }
        }
    };
}

void OscillatorStream(Oscillator oscillator, float32* wave, int length) {
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
            for(int i = 0; i < length; i++) {
                wave[i] = (wave[i] > 0) ? 1 : -1;
            }
        }
    }
}
#include <stdbool.h>
#include <string.h>
#include <strings.h>

void copy(const void *restrict src, void *restrict dst, size_t size) {
    memcpy(dst, src, size);
}

void copyPad(const void *restrict src, void *restrict dst, size_t size) {
    if (src == nil) {
        for (uint32 i = 0; i < size; i++) {
            ((byte *)dst)[i] = '\0';
        }
    }

    uint32 srclen = strlen(src);
    if (srclen > size) srclen = size;
    memcpy(dst, src, srclen);
    for (uint32 i = srclen; i < size; i++) {
        ((byte *)dst)[i] = '\0';
    }
}

bool bitSet(uint64 bits, uint8 index) {
    uint64 mask = 1 << (uint64)index;
    return (bits & mask) == mask;
}

bool bitUnset(uint64 bits, uint8 index) {
    uint64 mask = 1 << (uint64)index;
    return (bits & mask) != mask;
}

uint64 setBit(uint64 bits, uint8 index) {
    return bits | (1 << (uint64)index);
}

uint64 unsetBit(uint64 bits, uint8 index) {
    return bits & ~(1 << (uint64)index);
}

float32 clamp(float32 value, float32 min, float32 max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

bool hasPrefix(const char *text, const char *prefix) {
    if (text == nil || prefix == nil) return false;
    uint32 prefixLen = strlen(prefix), textLen = strlen(text);
    if (textLen < prefixLen) return false;
    return strncmp(text, prefix, prefixLen) == 0;
}
#include <math.h>


extern inline Vec2 Vec2TransformAff3(Vec2 this, Aff3 aff3) {
    return Aff3TransformVec2(aff3, this);
}

Vec2 Vec2Rotate(Vec2 this, Vec2 pivot, float32 angle) {
    angle *= TO_RAD;
    float32 sin = sinf(angle), cos = sinf(angle);
    float32 dx = this.X - pivot.X;
    float32 dy = this.Y - pivot.Y;

    return (Vec2){
        .X = cos * dx - sin * dy + pivot.X,
        .Y = sin * dx + cos * dy + pivot.Y,
    };
}
#ifndef Graphics_H
#define Graphics_H


// `Color` represents an RGBA color value.
//
// By varying the intensity of red (`R`), green (`G`), and blue (`B`), you
// can create a wide range of color. Alpha (`A`) specifies how transparent or
// opaque the color is.
typedef struct Color {
    uint8 R;
    uint8 G;
    uint8 B;
    uint8 A;
} Color, RGBA;

// `ColorHex` creates a Color object using a hexadecimal value in ARGB format.
Color ColorHex(const uint32 color);

// `GraphicsInit` creates an OpenGL context for the current window. It returns
// true when successful or false if there was an error.
//
// Remember to close the graphics api by calling `GraphicsClose`. (This should
// be done before disposing the window)
bool GraphicsInit(Window* window);

// `GraphicsClose` frees the OpenGL context for this window.
//
// It should be called when you are finished drawing graphics on this window.
void GraphicsClose(Window* window);

// `GraphicsMakeCurrent` makes the OpenGL api use this window and its context.
//
// This should be called before doing OpenGL operations to ensure that OpenGL
// functions target this window and context
void GraphicsMakeCurrent(Window* window);

#endif  // Graphics_H
#ifndef Mouse_H
#define Mouse_H


typedef struct Window Window;

// `MouseButton` represents the buttons on a standard mouse.
//
// Most likely, there are 5 buttons at most on a mouse. Some gaming mice may
// have more buttons but they usually get mapped to keyboard keys or have other
// purposes like changing CPI/DPI instead
typedef enum MouseButton {
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
    MouseButton_Back,
    MouseButton_Forward,
} MouseButton;

// `MousePressed` returns true if the specified mouse button is pressed.
bool MousePressed(Window* window, MouseButton button);

// `MouseJustPressed` returns true if the specified mouse button has just been
// pressed since the last call to `WindowUpdate`.
bool MouseJustPressed(Window* window, MouseButton button);

// `MouseJustReleased` returns true if the specified mouse button has just been
// released since the last call to `WindowUpdate`.
bool MouseJustReleased(Window* window, MouseButton button);

#endif  // Mouse_H

extern inline Color ColorHex(const uint32 color) {
    return (Color){
        (uint8)(color >> 16),
        (uint8)(color >> 8),
        (uint8)(color >> 0),
        (uint8)(color >> 24),
    };
}

bool MousePressed(Window *window, MouseButton button) {
    return bitSet(window->mousePressed, button);
}

bool MouseJustPressed(Window *window, MouseButton button) {
    bool pressed = bitSet(window->mousePressed, button);
    return pressed && pressed != bitSet(window->pMousePressed, button);
}

bool MouseJustReleased(Window *window, MouseButton button) {
    bool released = bitUnset(window->mousePressed, button);
    return released && released != bitUnset(window->pMousePressed, button);
}

bool KeyPressed(Window *window, Key key) {
    return window->keyPressed[key];
}

bool KeyJustPressed(Window *window, Key key) {
    return window->keyPressed[key] && window->keyPressed[key] != window->pKeyPressed[key];
}

bool KeyJustReleased(Window *window, Key key) {
    return window->keyPressed[key] == false && window->keyPressed[key] != window->pKeyPressed[key];
}

bool KeyModSet(Window *window, KeyMod modifier) {
    return ((window->KeyMod | modifier) == modifier);
}

rune KeyGetChar(Window *window) {
    return window->keyChar;
}

Gamepad *WindowGetGamepad(Window *window, int playerID) {
    return GamepadListGet(&window->gamepads, playerID);
}

extern inline Gamepad *WindowGetFirstConnectedGamepad(Window *window) {
    return GamepadListGet(&window->gamepads, 0);
}

int GamepadCount(Window *window) {
    return window->gamepads.len;
}

#undef Window
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// X11 also defines a `Window` type that conflicts with Mino. This hack
// basically renames Mino's `Window` typedef to MinoWindow only for the linux
// platform so that windowNative can compile without issue and we can maintain
// intuitive names without prefixes.
//
// I suppose you can't include both X11 and Mino's `<window.h>` in your source
// code without this hack. But Mino's goal is to make it so that you don't need
// to do that. Mino aims to provide everything you needing to make your game
// without any of the platform specific code or headers.
#if defined(PLATFORM_Linux)
#define Window MinoWindow
#endif


static void resetInputState(Window *window) {
    for (int i = 0; i < Key_Count; i++) {
        window->pKeyPressed[i] = window->keyPressed[i];
    }
    window->pMouseX = window->mouseX;
    window->pMouseY = window->mouseY;
    window->pMousePressed = window->mousePressed;
    window->keyChar = 0;
    window->pScrollX = window->scrollX;
    window->pScrollY = window->scrollY;
    window->scrollX = 0;
    window->scrollY = 0;
    Gamepad *gamepad;
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false) continue;
        gamepad->pButtons = gamepad->buttons;
        for (int j = 0; j < GamepadAxis_Count; j++) {
            gamepad->pAxes[j] = gamepad->axes[j];
        }
    }
}

#if defined(PLATFORM_Windows)

#include <windows.h>
#include <winuser.h>
#include <GL/gl.h>
#include <GL/wgl.h>
#include <wingdi.h>
#include <xinput.h>

struct WindowNative {
    HWND windowHandle;
    HGLRC glContext;
    int64 lastTick;
};

const Key WinKey2MinoKey[256] = {['A'] = Key_A, ['B'] = Key_B, ['C'] = Key_C, ['D'] = Key_D, ['E'] = Key_E, ['F'] = Key_F, ['G'] = Key_G, ['H'] = Key_H, ['I'] = Key_I, ['J'] = Key_J, ['K'] = Key_K, ['L'] = Key_L, ['M'] = Key_M, ['N'] = Key_N, ['O'] = Key_O, ['P'] = Key_P, ['Q'] = Key_Q, ['R'] = Key_R, ['S'] = Key_S, ['T'] = Key_T, ['U'] = Key_U, ['V'] = Key_V, ['W'] = Key_W, ['X'] = Key_X, ['Y'] = Key_Y, ['Z'] = Key_Z, [VK_LMENU] = Key_LeftAlt, [VK_RMENU] = Key_RightAlt, [VK_DOWN] = Key_DownArrow, [VK_LEFT] = Key_LeftArrow, [VK_RIGHT] = Key_RightArrow, [VK_UP] = Key_UpArrow, [VK_OEM_3] = Key_Tilde, [VK_OEM_5] = Key_Backslash, [VK_BACK] = Key_Backspace, [VK_OEM_4] = Key_LeftBracket, [VK_OEM_6] = Key_RightBracket, [VK_CAPITAL] = Key_CapsLock, [VK_OEM_COMMA] = Key_Comma, [VK_APPS] = Key_Menu, [VK_LCONTROL] = Key_LeftCtrl, [VK_RCONTROL] = Key_RightCtrl, [VK_DELETE] = Key_Delete, ['0'] = Key_0, ['1'] = Key_1, ['2'] = Key_2, ['3'] = Key_3, ['4'] = Key_4, ['5'] = Key_5, ['6'] = Key_6, ['7'] = Key_7, ['8'] = Key_8, ['9'] = Key_9, [VK_END] = Key_End, [VK_RETURN] = Key_Enter, [VK_OEM_PLUS] = Key_Equal, [VK_ESCAPE] = Key_Escape, [VK_F1] = Key_F1, [VK_F2] = Key_F2, [VK_F3] = Key_F3, [VK_F4] = Key_F4, [VK_F5] = Key_F5, [VK_F6] = Key_F6, [VK_F7] = Key_F7, [VK_F8] = Key_F8, [VK_F9] = Key_F9, [VK_F10] = Key_F10, [VK_F11] = Key_F11, [VK_F12] = Key_F12, [VK_HOME] = Key_Home, [VK_INSERT] = Key_Insert, [VK_LWIN] = Key_LeftWin, [VK_RWIN] = Key_RightWin, [VK_OEM_MINUS] = Key_Minus, [VK_NUMLOCK] = Key_NumLock, [VK_NUMPAD0] = Key_NumPad0, [VK_NUMPAD1] = Key_NumPad1, [VK_NUMPAD2] = Key_NumPad2, [VK_NUMPAD3] = Key_NumPad3, [VK_NUMPAD4] = Key_NumPad4, [VK_NUMPAD5] = Key_NumPad5, [VK_NUMPAD6] = Key_NumPad6, [VK_NUMPAD7] = Key_NumPad7, [VK_NUMPAD8] = Key_NumPad8, [VK_NUMPAD9] = Key_NumPad9, [VK_ADD] = Key_NumPadAdd, [VK_DECIMAL] = Key_NumPadDecimal, [VK_DIVIDE] = Key_NumPadDivide, [VK_SEPARATOR] = Key_NumPadEnter, /*Key_NP_EQUAL*/[VK_MULTIPLY] = Key_NumPadMultiply, [VK_SUBTRACT] = Key_NumPadSubtract, [VK_NEXT] = Key_PageDown, [VK_PRIOR] = Key_PageUp, [VK_PAUSE] = Key_Pause, [VK_OEM_PERIOD] = Key_Period, [VK_SNAPSHOT] = Key_PrintScreen, [VK_OEM_7] = Key_Quote, [VK_SCROLL] = Key_ScrollLock, [VK_OEM_1] = Key_Semicolon, [VK_LSHIFT] = Key_LeftShift, [VK_RSHIFT] = Key_RightShift, [VK_OEM_2] = Key_Slash, [VK_SPACE] = Key_Space, [VK_TAB] = Key_Tab, [VK_MENU] = Key_Alt, [VK_CONTROL] = Key_Ctrl, [VK_SHIFT] = Key_Shift /*, Key_WIN*/};
const int MinoKey2WinKey[Key_Count] = {[Key_A] = 'A', [Key_B] = 'B', [Key_C] = 'C', [Key_D] = 'D', [Key_E] = 'E', [Key_F] = 'F', [Key_G] = 'G', [Key_H] = 'H', [Key_I] = 'I', [Key_J] = 'J', [Key_K] = 'K', [Key_L] = 'L', [Key_M] = 'M', [Key_N] = 'N', [Key_O] = 'O', [Key_P] = 'P', [Key_Q] = 'Q', [Key_R] = 'R', [Key_S] = 'S', [Key_T] = 'T', [Key_U] = 'U', [Key_V] = 'V', [Key_W] = 'W', [Key_X] = 'X', [Key_Y] = 'Y', [Key_Z] = 'Z', [Key_LeftAlt] = VK_LMENU, [Key_RightAlt] = VK_RMENU, [Key_DownArrow] = VK_DOWN, [Key_LeftArrow] = VK_LEFT, [Key_RightArrow] = VK_RIGHT, [Key_UpArrow] = VK_UP, [Key_Tilde] = VK_OEM_3, [Key_Backslash] = VK_OEM_5, [Key_Backspace] = VK_BACK, [Key_LeftBracket] = VK_OEM_4, [Key_RightBracket] = VK_OEM_6, [Key_CapsLock] = VK_CAPITAL, [Key_Comma] = VK_OEM_COMMA, [Key_Menu] = VK_APPS, [Key_LeftCtrl] = VK_LCONTROL, [Key_RightCtrl] = VK_RCONTROL, [Key_Delete] = VK_DELETE, [Key_0] = '0', [Key_1] = '1', [Key_2] = '2', [Key_3] = '3', [Key_4] = '4', [Key_5] = '5', [Key_6] = '6', [Key_7] = '7', [Key_8] = '8', [Key_9] = '9', [Key_End] = VK_END, [Key_Enter] = VK_RETURN, [Key_Equal] = VK_OEM_PLUS, [Key_Escape] = VK_ESCAPE, [Key_F1] = VK_F1, [Key_F2] = VK_F2, [Key_F3] = VK_F3, [Key_F4] = VK_F4, [Key_F5] = VK_F5, [Key_F6] = VK_F6, [Key_F7] = VK_F7, [Key_F8] = VK_F8, [Key_F9] = VK_F9, [Key_F10] = VK_F10, [Key_F11] = VK_F11, [Key_F12] = VK_F12, [Key_Home] = VK_HOME, [Key_Insert] = VK_INSERT, [Key_LeftWin] = VK_LWIN, [Key_RightWin] = VK_RWIN, [Key_Minus] = VK_OEM_MINUS, [Key_NumLock] = VK_NUMLOCK, [Key_NumPad0] = VK_NUMPAD0, [Key_NumPad1] = VK_NUMPAD1, [Key_NumPad2] = VK_NUMPAD2, [Key_NumPad3] = VK_NUMPAD3, [Key_NumPad4] = VK_NUMPAD4, [Key_NumPad5] = VK_NUMPAD5, [Key_NumPad6] = VK_NUMPAD6, [Key_NumPad7] = VK_NUMPAD7, [Key_NumPad8] = VK_NUMPAD8, [Key_NumPad9] = VK_NUMPAD9, [Key_NumPadAdd] = VK_ADD, [Key_NumPadDecimal] = VK_DECIMAL, [Key_NumPadDivide] = VK_DIVIDE, [Key_NumPadEnter] = VK_SEPARATOR, /*Key_NP_EQUAL*/[Key_NumPadMultiply] = VK_MULTIPLY, [Key_NumPadSubtract] = VK_SUBTRACT, [Key_PageDown] = VK_NEXT, [Key_PageUp] = VK_PRIOR, [Key_Pause] = VK_PAUSE, [Key_Period] = VK_OEM_PERIOD, [Key_PrintScreen] = VK_SNAPSHOT, [Key_Quote] = VK_OEM_7, [Key_ScrollLock] = VK_SCROLL, [Key_Semicolon] = VK_OEM_1, [Key_LeftShift] = VK_LSHIFT, [Key_RightShift] = VK_RSHIFT, [Key_Slash] = VK_OEM_2, [Key_Space] = VK_SPACE, [Key_Tab] = VK_TAB, [Key_Alt] = VK_MENU, [Key_Ctrl] = VK_CONTROL, [Key_Shift] = VK_SHIFT /*, Key_WIN*/};

static LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT msg, WPARAM wParam,
    LPARAM lParam) {
    Window *window = (Window *)GetWindowLongPtr(windowHandle, GWLP_USERDATA);

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = GetDC(windowHandle);
            BeginPaint(windowHandle, &ps);
            SwapBuffers(deviceContext);
            EndPaint(windowHandle, &ps);
            ReleaseDC(windowHandle, deviceContext);
        } break;

        case WM_SIZE: {
            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);

            if (window->native->glContext) {
                GraphicsMakeCurrent(window);

                glViewport(
                    0, 0,
                    window->width,
                    window->height);
            }
        } break;

        case WM_CLOSE: {
            DestroyWindow(windowHandle);
        } break;

        case WM_LBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Left);
        } break;

        case WM_LBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MouseButton_Left);
        } break;

        case WM_RBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Right);
        } break;

        case WM_RBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MouseButton_Right);
        } break;

        case WM_MBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Middle);
        } break;

        case WM_MBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, MouseButton_Middle);
        } break;

        case WM_XBUTTONDOWN: {
            window->mousePressed = (uint8)setBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MouseButton_Back : MouseButton_Forward);
        } break;

        case WM_XBUTTONUP: {
            window->mousePressed = (uint8)unsetBit(window->mousePressed, (HIWORD(wParam) == XBUTTON1) ? MouseButton_Back : MouseButton_Forward);
        } break;

        case WM_MOUSEMOVE: {
            window->mouseX = LOWORD(lParam);
            window->mouseY = HIWORD(lParam);
        } break;

        case WM_MOUSEWHEEL: {
            window->scrollY += GET_WHEEL_DELTA_WPARAM(wParam);
        } break;

        case WM_MOUSEHWHEEL: {
            window->scrollX += GET_WHEEL_DELTA_WPARAM(wParam);
        } break;

        case WM_KEYDOWN:
        case WM_KEYUP: {
            Key key = WinKey2MinoKey[wParam];
            window->keyPressed[key] = (msg == WM_KEYDOWN);

            // Check modifiers
            window->KeyMod =
                (GetKeyState(VK_CONTROL) & 0x8000 ? KeyMod_Ctrl : 0) |
                (GetKeyState(VK_SHIFT) & 0x8000 ? KeyMod_Shift : 0) |
                (GetKeyState(VK_MENU) & 0x8000 ? KeyMod_Alt : 0) |
                ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000 ? KeyMod_Win : 0) |
                (GetKeyState(VK_CAPITAL) & 0x0001 ? KeyMod_CapsLock : 0) |
                (GetKeyState(VK_SCROLL) & 0x0001 ? KeyMod_ScrollLock : 0) |
                (GetKeyState(VK_NUMLOCK) & 0x0001 ? KeyMod_NumLock : 0);
        } break;

        case WM_CHAR: {
            window->keyChar = wParam;
        } break;

        case WM_ACTIVATEAPP: {
            XInputEnable(wParam);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        default: {
            return DefWindowProc(windowHandle, msg, wParam, lParam);
        }
    }
    return 0;
}

bool WindowInit(Window *window, WindowConfig config) {
    HINSTANCE instance = GetModuleHandle(nil);
    WNDCLASSEX windowClass = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_VREDRAW | CS_HREDRAW,
        .lpfnWndProc = WindowProcedure,
        .hInstance = instance,
        .lpszClassName = "Mino Window Class"};
    RegisterClassEx(&windowClass);

    WindowNative *native = window->native = allocate(WindowNative);

    native->windowHandle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        windowClass.lpszClassName,
        config.title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        config.width, config.height,
        nil, nil, instance, nil);

    if (native->windowHandle == nil) {
        free(native);
        return false;
    }

    GamepadListInit(&window->gamepads, 4, 4);
    for (int i = 0; i < window->gamepads.len; i++) {
        Gamepad *gamepad = GamepadListGet(&window->gamepads, i);
        gamepad->playerID = i;
    }

    SetWindowLongPtr(native->windowHandle, GWLP_USERDATA, (LONG_PTR)window);
    ShowWindow(native->windowHandle, SW_NORMAL);
    UpdateWindow(native->windowHandle);

    return true;
}

const int MinoGamepadButton2XinputButton[] = {[GamepadButton_Up] = XINPUT_GAMEPAD_DPAD_UP, [GamepadButton_Down] = XINPUT_GAMEPAD_DPAD_DOWN, [GamepadButton_Left] = XINPUT_GAMEPAD_DPAD_LEFT, [GamepadButton_Right] = XINPUT_GAMEPAD_DPAD_RIGHT, [GamepadButton_A] = XINPUT_GAMEPAD_A, [GamepadButton_B] = XINPUT_GAMEPAD_B, [GamepadButton_X] = XINPUT_GAMEPAD_X, [GamepadButton_Y] = XINPUT_GAMEPAD_Y, [GamepadButton_L1] = XINPUT_GAMEPAD_LEFT_SHOULDER, [GamepadButton_L3] = XINPUT_GAMEPAD_LEFT_THUMB, [GamepadButton_R1] = XINPUT_GAMEPAD_RIGHT_SHOULDER, [GamepadButton_R3] = XINPUT_GAMEPAD_RIGHT_THUMB, [GamepadButton_Start] = XINPUT_GAMEPAD_START, [GamepadButton_Select] = XINPUT_GAMEPAD_BACK};

static void updateGamepads(Window *window) {
    XINPUT_STATE xInputState;
    Gamepad *gamepad;

    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        gamepad->connected = XInputGetState(i, &xInputState) == ERROR_SUCCESS;
        if (gamepad->connected == false) {
            continue;
        }

        for (int j = 0; j < (int)len(MinoGamepadButton2XinputButton, int); j++) {
            int XinputButton = MinoGamepadButton2XinputButton[j];
            if ((xInputState.Gamepad.wButtons & XinputButton) == XinputButton) {
                gamepad->buttons = setBit(gamepad->buttons, j);
            } else {
                gamepad->buttons = unsetBit(gamepad->buttons, j);
            }
        }

        gamepad->axes[GamepadAxis_LeftStickX] = clamp((float32)xInputState.Gamepad.sThumbLX / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_LeftStickY] = clamp((float32)xInputState.Gamepad.sThumbLY / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_RightStickX] = clamp((float32)xInputState.Gamepad.sThumbRX / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_RightStickY] = clamp((float32)xInputState.Gamepad.sThumbRY / 0x7FFF, -1, 1);
        gamepad->axes[GamepadAxis_LeftTrigger] = clamp((float32)xInputState.Gamepad.bLeftTrigger / 0xFF, 0, 1);
        gamepad->axes[GamepadAxis_RightTrigger] = clamp((float32)xInputState.Gamepad.bRightTrigger / 0xFF, 0, 1);

        if (xInputState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            gamepad->buttons = (uint32)unsetBit(gamepad->buttons, GamepadButton_L2);
        };
        if (xInputState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            gamepad->buttons = (uint32)unsetBit(gamepad->buttons, GamepadButton_R2);
        };

        XINPUT_VIBRATION vibrations = {
            .wLeftMotorSpeed = (uint16)(gamepad->leftMotor * 0xFFFF),
            .wRightMotorSpeed = (uint16)(gamepad->rightMotor * 0xFFFF),
        };
        XInputSetState(i, &vibrations);
    }
}

bool WindowUpdate(Window *window) {
    resetInputState(window);
    updateGamepads(window);

    MSG message;
    while (PeekMessage(&message, nil, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    InvalidateRect(window->native->windowHandle, nil, false);
    return true;
}

void WindowClose(Window *window) {
    GamepadListFree(&window->gamepads);

    if (window->native == nil) return;

    if (window->native->windowHandle) {
        CloseWindow(window->native->windowHandle);
    }

    free(window->native);
    window->native = nil;
}

int64 WindowTime() {
    LARGE_INTEGER frequency, count;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&count);
    return (int64)(count.QuadPart * 1000.0 / frequency.QuadPart);
}

void WindowSleep(int64 milliseconds) {
    Sleep(milliseconds);
}

bool GraphicsInit(Window *window) {
    if (window->native->glContext) {
        return false;
    }
    HDC deviceContext = GetDC(window->native->windowHandle);
    PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,

        .cRedBits = 8,
        .cRedShift = 16,
        .cAccumRedBits = 8,

        .cGreenBits = 8,
        .cGreenShift = 8,
        .cAccumGreenBits = 8,

        .cBlueBits = 8,
        .cBlueShift = 0,
        .cAccumBlueBits = 8,

        .cAlphaBits = 8,
        .cAlphaShift = 24,
        .cAccumAlphaBits = 8,

        .cDepthBits = 8,
        .cStencilBits = 8,
    };

    int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
    if (pixelFormat == 0) {
        ReleaseDC(window->native->windowHandle, deviceContext);
        return false;
    }

    if (SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor) == false) {
        ReleaseDC(window->native->windowHandle, deviceContext);
        return false;
    }

    DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor);

    window->native->glContext = wglCreateContext(deviceContext);
    if (window->native->glContext == nil) {
        ReleaseDC(window->native->windowHandle, deviceContext);
        return false;
    }
    ReleaseDC(window->native->windowHandle, deviceContext);

    glEnable(GL_DEPTH_TEST);
    return true;
}

void GraphicsClose(Window *window) {
    if (window->native->glContext == nil) return;
    wglDeleteContext(window->native->glContext);
    window->native->glContext = nil;
}

void GraphicsMakeCurrent(Window *window) {
    HDC deviceContext = GetDC(window->native->windowHandle);
    wglMakeCurrent(deviceContext, window->native->glContext);
    ReleaseDC(window->native->windowHandle, deviceContext);
}

#elif defined(PLATFORM_Linux)

// Gotta clean this up so X11 can work. Now `MinoWindow` is the Window for Mino
// and `Window` is for X11's window ID.
#undef Window
#include <X11/Xutil.h>

static Key XKey2MinoKey(int key) {
    switch (key) {
        case XK_a: return Key_A;
        case XK_b: return Key_B;
        case XK_c: return Key_C;
        case XK_d: return Key_D;
        case XK_e: return Key_E;
        case XK_f: return Key_F;
        case XK_g: return Key_G;
        case XK_h: return Key_H;
        case XK_i: return Key_I;
        case XK_j: return Key_J;
        case XK_k: return Key_K;
        case XK_l: return Key_L;
        case XK_m: return Key_M;
        case XK_n: return Key_N;
        case XK_o: return Key_O;
        case XK_p: return Key_P;
        case XK_q: return Key_Q;
        case XK_r: return Key_R;
        case XK_s: return Key_S;
        case XK_t: return Key_T;
        case XK_u: return Key_U;
        case XK_v: return Key_V;
        case XK_w: return Key_W;
        case XK_x: return Key_X;
        case XK_y: return Key_Y;
        case XK_z: return Key_Z;
        case XK_Alt_L: return Key_LeftAlt;
        case XK_Alt_R: return Key_RightAlt;
        case XK_Down: return Key_DownArrow;
        case XK_Left: return Key_LeftArrow;
        case XK_Right: return Key_RightArrow;
        case XK_Up: return Key_UpArrow;
        case XK_grave: return Key_Tilde;
        case XK_backslash: return Key_Backslash;
        case XK_BackSpace: return Key_Backspace;
        case XK_bracketleft: return Key_LeftBracket;
        case XK_bracketright: return Key_RightBracket;
        case XK_Caps_Lock: return Key_CapsLock;
        case XK_comma: return Key_Comma;
        case XK_Menu: return Key_Menu;
        case XK_Control_L: return Key_LeftCtrl;
        case XK_Control_R: return Key_RightCtrl;
        case XK_Delete: return Key_Delete;
        case XK_0: return Key_0;
        case XK_1: return Key_1;
        case XK_2: return Key_2;
        case XK_3: return Key_3;
        case XK_4: return Key_4;
        case XK_5: return Key_5;
        case XK_6: return Key_6;
        case XK_7: return Key_7;
        case XK_8: return Key_8;
        case XK_9: return Key_9;
        case XK_End: return Key_End;
        case XK_Return: return Key_Enter;
        case XK_equal: return Key_Equal;
        case XK_Escape: return Key_Escape;
        case XK_F1: return Key_F1;
        case XK_F2: return Key_F2;
        case XK_F3: return Key_F3;
        case XK_F4: return Key_F4;
        case XK_F5: return Key_F5;
        case XK_F6: return Key_F6;
        case XK_F7: return Key_F7;
        case XK_F8: return Key_F8;
        case XK_F9: return Key_F9;
        case XK_F10: return Key_F10;
        case XK_F11: return Key_F11;
        case XK_F12: return Key_F12;
        case XK_Home: return Key_Home;
        case XK_Insert: return Key_Insert;
        case XK_Super_L: return Key_LeftWin;
        case XK_Super_R: return Key_RightWin;
        case XK_minus: return Key_Minus;
        case XK_Num_Lock: return Key_NumLock;
        case XK_KP_Insert: return Key_NumPad0;
        case XK_KP_End: return Key_NumPad1;
        case XK_KP_Down: return Key_NumPad2;
        case XK_KP_Page_Down: return Key_NumPad3;
        case XK_KP_Left: return Key_NumPad4;
        case XK_KP_Begin: return Key_NumPad5;
        case XK_KP_Right: return Key_NumPad6;
        case XK_KP_Home: return Key_NumPad7;
        case XK_KP_Up: return Key_NumPad8;
        case XK_KP_Page_Up: return Key_NumPad9;
        case XK_KP_Add: return Key_NumPadAdd;
        case XK_KP_Delete: return Key_NumPadDecimal;
        case XK_KP_Divide: return Key_NumPadDivide;
        case XK_KP_Enter: return Key_NumPadEnter;
        case XK_KP_Equal: return Key_NumPadEqual;
        case XK_KP_Multiply: return Key_NumPadMultiply;
        case XK_KP_Subtract: return Key_NumPadSubtract;
        case XK_Page_Down: return Key_PageDown;
        case XK_Page_Up: return Key_PageUp;
        case XK_Pause: return Key_Pause;
        case XK_period: return Key_Period;
        case XK_Print: return Key_PrintScreen;
        case XK_apostrophe: return Key_Quote;
        case XK_Scroll_Lock: return Key_ScrollLock;
        case XK_semicolon: return Key_Semicolon;
        case XK_Shift_L: return Key_LeftShift;
        case XK_Shift_R: return Key_RightShift;
        case XK_slash: return Key_Slash;
        case XK_space: return Key_Space;
        case XK_Tab: return Key_Tab;
        default: return Key_Invalid;
    }
}

// `XKey2MinoKey` needs to be before `#include <linux/input.h>` because it contains defines that collide with Mino.
#include <fcntl.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <libudev.h>
#include <linux/input.h>
#include <linux/types.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>

struct WindowNative {
    Display *xDisplay;
    Window xWindow;
    Atom deleteWindow;
    XVisualInfo *visualInfo;
    struct udev *udev;
    struct udev_monitor *monitor;

    GLXContext glContext;
};

struct GamepadNative {
    int fileDescriptor;
    char *devicePath;
    struct input_absinfo analogInfos[GamepadAxis_Count];
    struct ff_effect rumble;
};

const int minoAxis2EvdevAxis[GamepadAxis_Count] = {
    [GamepadAxis_LeftStickX] = ABS_X,
    [GamepadAxis_LeftStickY] = ABS_Y,
    [GamepadAxis_LeftTrigger] = ABS_Z,
    [GamepadAxis_RightStickX] = ABS_RX,
    [GamepadAxis_RightStickY] = ABS_RY,
    [GamepadAxis_RightTrigger] = ABS_RZ,
};

const int evdevAxis2MinoAxis[GamepadAxis_Count] = {
    [ABS_X] = GamepadAxis_LeftStickX,
    [ABS_Y] = GamepadAxis_LeftStickY,
    [ABS_Z] = GamepadAxis_LeftTrigger,
    [ABS_RX] = GamepadAxis_RightStickX,
    [ABS_RY] = GamepadAxis_RightStickY,
    [ABS_RZ] = GamepadAxis_RightTrigger,
};

GamepadButton evdevButton2MinoButton(int button) {
    switch (button) {
        case BTN_A: return GamepadButton_A;
        case BTN_B: return GamepadButton_B;
        case BTN_X: return GamepadButton_X;
        case BTN_Y: return GamepadButton_Y;
        case BTN_START: return GamepadButton_Start;
        case BTN_SELECT: return GamepadButton_Select;
        case BTN_TL: return GamepadButton_L1;
        case BTN_TR: return GamepadButton_R1;
        case BTN_TL2: return GamepadButton_L2;
        case BTN_TR2: return GamepadButton_R2;
        case BTN_THUMBL: return GamepadButton_L3;
        case BTN_THUMBR: return GamepadButton_R3;
        case BTN_MODE: return GamepadButton_Home;
        default: return GamepadButton_Invalid;
    }
};

const struct input_absinfo fallbackInfos = {
    .minimum = -1,
    .maximum = 1,
};

static void connectController(MinoWindow *window, const char *devicePath) {
    const int devicePathLen = strlen(devicePath) + 1;
    Gamepad *gamepad = nil;
    int fd = open(devicePath, O_RDWR | O_NONBLOCK);
    if (fd < 0) return;

    // Attempt to reconnect controller with matching file name.
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false && strcmp(gamepad->native->devicePath, devicePath) == 0) {
            goto foundGamepad;
        }
    }

    // If no controllers have this file name, attempt to reconnect
    // with any disconnected controller.
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false) {
            free(gamepad->native->devicePath);
            goto replaceGamepad;
        };
    }

    // Finally, if no controllers are disconnected, just connect a
    // new controller.
    {
        GamepadListPush(&window->gamepads, (Gamepad){});
        gamepad = GamepadListGet(&window->gamepads, window->gamepads.len - 1);
        gamepad->native = malloc(sizeof(GamepadNative));
        memset(gamepad->native, 0, sizeof(GamepadNative));
    }

replaceGamepad:
    gamepad->native->devicePath = malloc(devicePathLen + 1);
    gamepad->native->devicePath[devicePathLen] = '\0';
    gamepad->native->rumble = (struct ff_effect){
        .id = -1,
        .type = FF_RUMBLE,
    };

    memcpy(gamepad->native->devicePath, devicePath, devicePathLen);

foundGamepad:;
    for (GamepadAxis i = 0; i < GamepadAxis_Count; i++) {
        if (ioctl(fd, EVIOCGABS(minoAxis2EvdevAxis[i]), &gamepad->native->analogInfos[i])) {
            gamepad->native->analogInfos[i] = fallbackInfos;
        };
    }
    gamepad->native->fileDescriptor = fd;
    gamepad->connected = true;
}

static void disconnectController(MinoWindow *window, const char *devicePath) {
    Gamepad *gamepad = nil;
    for (int i = 0; i < window->gamepads.len; i++) {
        Gamepad *temp = GamepadListGet(&window->gamepads, i);
        if (temp->connected == true && strcmp(temp->native->devicePath, devicePath) == 0) {
            gamepad = temp;
            break;
        }
    }
    if (gamepad == nil) return;

    close(gamepad->native->fileDescriptor);
    gamepad->native->fileDescriptor = 0;
    gamepad->connected = false;
    // just zero these out to be safe so, when a controller reconnects, none of
    // the buttons or sticks are still held
    gamepad->buttons = 0;
    for (int i = 0; i < GamepadAxis_Count; i++) {
        gamepad->axes[i] = 0;
    }
}

bool WindowInit(MinoWindow *window, WindowConfig config) {
    Display *xDisplay = XOpenDisplay(nil);
    if (xDisplay == nil) return false;

    XVisualInfo *visualInfo = glXChooseVisual(
        xDisplay,
        0,
        (GLint[]){
            GLX_RGBA,
            GLX_DEPTH_SIZE,
            24,
            GLX_DOUBLEBUFFER,
            None,
        });

    if (visualInfo == nil) return false;

    Window root = DefaultRootWindow(xDisplay);

    Colormap colormap = XCreateColormap(xDisplay, root, visualInfo->visual, AllocNone);

    int screenID = DefaultScreen(xDisplay);

    XSetWindowAttributes windowAttributes = {
        .colormap = colormap,
        .event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
        .background_pixel = WhitePixel(xDisplay, screenID),
        .border_pixel = BlackPixel(xDisplay, screenID),
        .override_redirect = True,
    };

    Window xWindow = XCreateWindow(
        xDisplay,
        root,
        0, 0, config.width, config.height,
        0,
        visualInfo->depth,
        InputOutput,
        visualInfo->visual,
        CWColormap | CWEventMask,
        &windowAttributes);

    if (xWindow == 0) return false;

    Atom deleteWindowAtom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(xDisplay, xWindow, &deleteWindowAtom, 1);

    XMapWindow(xDisplay, xWindow);
    XSync(xDisplay, xWindow);

    XStoreName(xDisplay, xWindow, config.title);

    struct udev *udev = udev_new();
    if (udev == nil) {
        println("Warning: unable to open udev");
        XDestroyWindow(xDisplay, xWindow);
        return false;
    }

    window->native = allocate(WindowNative);
    *window->native = (WindowNative){
        .xDisplay = xDisplay,
        .xWindow = xWindow,
        .deleteWindow = deleteWindowAtom,
        .visualInfo = visualInfo,
        .udev = udev,
    };
    GamepadListInit(&window->gamepads, 0, 4);

    struct udev_enumerate *devices = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(devices, "input");
    udev_enumerate_add_match_property(devices, "ID_INPUT_JOYSTICK", "1");
    udev_enumerate_scan_devices(devices);
    struct udev_list_entry *entry, *listEntry = udev_enumerate_get_list_entry(devices);
    struct udev_device *device = NULL;
    udev_list_entry_foreach(entry, listEntry) {
        const char *sysPath = udev_list_entry_get_name(entry);
        device = udev_device_new_from_syspath(udev, sysPath);

        const char *devicePath = udev_device_get_devnode(device);
        if (hasPrefix(devicePath, "/dev/input/event") == false) goto end;

        connectController(window, devicePath);
    end:
        udev_device_unref(device);
    }
    udev_enumerate_unref(devices);

    struct udev_monitor *monitor = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "input", nil);
    udev_monitor_enable_receiving(monitor);
    window->native->monitor = monitor;

    return true;
}

static void refreshControllers(MinoWindow *window) {
    fd_set fileDescriptors;
    FD_ZERO(&fileDescriptors);
    FD_SET(udev_monitor_get_fd(window->native->monitor), &fileDescriptors);
    while (true) {
        if (FD_ISSET(udev_monitor_get_fd(window->native->monitor), &fileDescriptors)) {
            struct udev_device *device;
            const char *action;
            device = udev_monitor_receive_device(window->native->monitor);
            if (device == nil) {
                return;
            }

            const char *isJoystick = udev_device_get_property_value(device, "ID_INPUT_JOYSTICK");
            if (isJoystick == nil || strcmp(isJoystick, "1") != 0) goto end;

            const char *devicePath = udev_device_get_devnode(device);
            if (hasPrefix(devicePath, "/dev/input/event") == false) {
                goto end;
            }

            action = udev_device_get_action(device);
            if (strcmp(action, "add") == 0) {
                connectController(window, devicePath);
            } else if (strcmp(action, "remove") == 0) {
                disconnectController(window, devicePath);
            }
        end:
            udev_device_unref(device);
        }
    }
}

static void setAxis(Gamepad *gamepad, struct input_event event) {
    if (event.code == ABS_HAT0X) {
        if (event.value >= 1) {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Left);
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Right);
        } else if (event.value <= -1) {
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Left);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Right);
        } else {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Left);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Right);
        }
        return;
    } else if (event.code == ABS_HAT0Y) {
        if (event.value >= 1) {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Down);
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Up);
        } else if (event.value <= -1) {
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_Down);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Up);
        } else {
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Down);
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_Up);
        }
        return;
    }
    if (event.code >= GamepadAxis_Count) return;  // unsupported axis.
    GamepadAxis axis = evdevAxis2MinoAxis[event.code];
    struct input_absinfo info = gamepad->native->analogInfos[axis];
    if (axis == GamepadAxis_LeftStickY || axis == GamepadAxis_RightStickY) event.value *= -1;
    if (axis == GamepadAxis_LeftTrigger || axis == GamepadAxis_RightTrigger) {
        gamepad->axes[axis] = clamp((float32)(event.value - info.minimum) / (float32)(info.maximum - info.minimum), 0, 1);
    } else {
        gamepad->axes[axis] = clamp((float32)(event.value - info.minimum) / (float32)(info.maximum - info.minimum) * 2.0 - 1.0, -1, 1);
    }
    if (axis == GamepadAxis_LeftTrigger) {
        if (event.value > info.flat)
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_L2);
        else
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_L2);
    } else if (axis == GamepadAxis_RightTrigger) {
        if (event.value > info.flat)
            gamepad->buttons = setBit(gamepad->buttons, GamepadButton_R2);
        else
            gamepad->buttons = unsetBit(gamepad->buttons, GamepadButton_R2);
    }
}

static void setButton(Gamepad *gamepad, struct input_event event) {
    GamepadButton button = evdevButton2MinoButton(event.code);
    if (button == GamepadButton_Invalid) return;
    if (event.value > 0) {
        gamepad->buttons = setBit(gamepad->buttons, button);
    } else {
        gamepad->buttons = unsetBit(gamepad->buttons, button);
    }
}

static void setRumble(Gamepad *gamepad) {
    gamepad->native->rumble.type = FF_RUMBLE;
    gamepad->native->rumble.replay.length = 0xFFFF;
    gamepad->native->rumble.u.rumble = (struct ff_rumble_effect){
        .strong_magnitude = (int32)(gamepad->leftMotor * 0xFFFF),
        .weak_magnitude = (int32)(gamepad->rightMotor * 0xFFFF),
    };
    if (ioctl(gamepad->native->fileDescriptor, EVIOCSFF, &gamepad->native->rumble) < 0) {
        gamepad->native->rumble.id = -1;
        if (ioctl(gamepad->native->fileDescriptor, EVIOCSFF, &gamepad->native->rumble) < 0) {
            return;
        }
    };
    struct input_event event = {
        .type = EV_FF,
        .code = gamepad->native->rumble.id,
        .value = 1,
    };
    if (write(gamepad->native->fileDescriptor, &event, sizeof(event)) < 0) {
        // This if statement is to remove a warning. We don't really need to do
        // anything if write fails anyway. We'll just ignore it.
    };
}

static void updateControllers(MinoWindow *window) {
    Gamepad *gamepad;
    struct input_event events[8];
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected == false) continue;
        while (true) {
            int32 bytesRead = read(gamepad->native->fileDescriptor, &events, sizeof(events));
            if (bytesRead == -1) break;
            for (int32 i = 0; (i + 1) * (int32)sizeof(struct input_event) < bytesRead; i++) {
                switch (events[i].type) {
                    case EV_ABS: {
                        setAxis(gamepad, events[i]);
                    } break;
                    case EV_KEY: {
                        setButton(gamepad, events[i]);
                    } break;
                }
            }
        }
        if (gamepad->leftMotor != gamepad->pLeftMotor || gamepad->rightMotor != gamepad->pRightMotor) {
            setRumble(gamepad);
            gamepad->pLeftMotor = gamepad->leftMotor;
            gamepad->pRightMotor = gamepad->rightMotor;
        }
    }
}

bool WindowUpdate(MinoWindow *window) {
    XEvent event;
    WindowNative *native = window->native;

    resetInputState(window);
    refreshControllers(window);
    updateControllers(window);
    if (window->native->glContext) {
        glXMakeCurrent(
            window->native->xDisplay,
            window->native->xWindow,
            window->native->glContext);
        glXSwapBuffers(
            window->native->xDisplay,
            window->native->xWindow);
    }
    while (XPending(native->xDisplay)) {
        XNextEvent(native->xDisplay, &event);
        switch (event.type) {
            case Expose: {
                XWindowAttributes windowAttributes;
                XGetWindowAttributes(
                    window->native->xDisplay,
                    window->native->xWindow,
                    &windowAttributes);

                window->width = windowAttributes.width;
                window->height = windowAttributes.height;

                if (window->native->glContext) {
                    glViewport(
                        0, 0,
                        windowAttributes.width,
                        windowAttributes.height);
                }
            } break;

            case MotionNotify: {
                window->mouseX = event.xmotion.x;
                window->mouseY = event.xmotion.y;
            } break;

            case ButtonPress: {
                switch (event.xbutton.button) {
                    case 1:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Left);
                        break;
                    case 2:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Middle);
                        break;
                    case 3:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Right);
                        break;
                    case 4:
                        window->scrollY++;
                        break;
                    case 5:
                        window->scrollY--;
                        break;
                    case 6:
                        window->scrollX--;
                        break;
                    case 7:
                        window->scrollX++;
                        break;
                    case 8:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Back);
                        break;
                    case 9:
                        window->mousePressed = setBit(window->mousePressed, MouseButton_Forward);
                        break;
                }
                break;
            }

            case ButtonRelease: {
                switch (event.xbutton.button) {
                    case 1:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Left);
                        break;
                    case 2:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Middle);
                        break;
                    case 3:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Right);
                        break;
                    case 8:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Back);
                        break;
                    case 9:
                        window->mousePressed = unsetBit(window->mousePressed, MouseButton_Forward);
                        break;
                }
            } break;

            case KeyPress:
            case KeyRelease: {
                KeySym keySymbol = XkbKeycodeToKeysym(native->xDisplay, event.xkey.keycode, 0, 0);
                Key key = XKey2MinoKey(keySymbol);
                if (key == Key_Invalid) break;

                window->keyPressed[key] = event.type == KeyPress;

                if (key == Key_LeftCtrl || key == Key_RightCtrl)
                    window->keyPressed[Key_Ctrl] = (window->keyPressed[Key_LeftCtrl] || window->keyPressed[Key_RightCtrl]);
                else if (key == Key_LeftShift || key == Key_RightShift)
                    window->keyPressed[Key_Shift] = (window->keyPressed[Key_LeftShift] || window->keyPressed[Key_RightShift]);
                else if (key == Key_LeftWin || key == Key_RightWin)
                    window->keyPressed[Key_Win] = (window->keyPressed[Key_LeftWin] || window->keyPressed[Key_RightWin]);
                else if (key == Key_LeftAlt || key == Key_RightAlt)
                    window->keyPressed[Key_Alt] = (window->keyPressed[Key_LeftAlt] || window->keyPressed[Key_RightAlt]);

                window->KeyMod =
                    (window->keyPressed[Key_Win] ? KeyMod_Win : 0) |
                    (window->keyPressed[Key_Alt] ? KeyMod_Alt : 0) |
                    (window->keyPressed[Key_Shift] ? KeyMod_Shift : 0) |
                    (window->keyPressed[Key_Ctrl] ? KeyMod_Ctrl : 0) |
                    (bitSet(event.xkey.state, LockMapIndex) ? KeyMod_CapsLock : 0) |
                    (bitSet(event.xkey.state, Mod2MapIndex) ? KeyMod_NumLock : 0) |
                    (bitSet(event.xkey.state, Mod5MapIndex) ? KeyMod_ScrollLock : 0);

                if (event.type == KeyPress) {
                    char buffer[sizeof(rune)];
                    if (XLookupString(&event.xkey, buffer, sizeof(buffer), &keySymbol, NULL) > 0) {
                        mbstowcs(&window->keyChar, buffer, 1);
                    };
                }
            } break;

            case ClientMessage: {
                if (event.xclient.data.l[0] == (long)native->deleteWindow) {
                    return false;
                }
            } break;
        }
    }
    return true;
}

void WindowClose(MinoWindow *window) {
    XFree(window->native->visualInfo);
    XDestroyWindow(window->native->xDisplay, window->native->xWindow);
    XCloseDisplay(window->native->xDisplay);
    Gamepad *gamepad;
    for (int i = 0; i < window->gamepads.len; i++) {
        gamepad = GamepadListGet(&window->gamepads, i);
        if (gamepad->connected) {
            close(gamepad->native->fileDescriptor);
        }
        if (gamepad->native != nil) {
            if (gamepad->native->devicePath != nil) {
                free(gamepad->native->devicePath);
                gamepad->native->devicePath = nil;
            }
            free(gamepad->native);
            gamepad->native = nil;
        }
    }
    GamepadListFree(&window->gamepads);
    udev_monitor_unref(window->native->monitor);
    udev_unref(window->native->udev);
    free(window->native);
}

#include <time.h>

int64 WindowTime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + (time.tv_nsec / 1000000);
}

void WindowSleep(int64 milliseconds) {
    struct timespec time;
    time.tv_sec = milliseconds / 1000;
    time.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&time, nil);
}

bool GraphicsInit(MinoWindow *window) {
    GLXContext glContext = glXCreateContext(
        window->native->xDisplay,
        window->native->visualInfo,
        nil,
        GL_TRUE);

    if (glContext == nil) return false;

    glXMakeCurrent(
        window->native->xDisplay,
        window->native->xWindow,
        glContext);

    glEnable(GL_DEPTH_TEST);

    window->native->glContext = glContext;
    return true;
}

void GraphicsMakeCurrent(MinoWindow *window) {
    glXMakeCurrent(
        window->native->xDisplay,
        window->native->xWindow,
        window->native->glContext);
}

void GraphicsClose(MinoWindow *window) {
    if (window->native->glContext == nil) return;

    glXDestroyContext(
        window->native->xDisplay,
        window->native->glContext);

    window->native->glContext = nil;
}

#endif


