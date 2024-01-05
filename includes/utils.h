#ifndef Utils_H
#define Utils_H

#include <stddef.h>

#include "types.h"

#define UNUSED(...) (void)(__VA_ARGS__)

#ifdef NOPRINT
#define println(format, ...) \
    do {                     \
        UNUSED(format);      \
        UNUSED(__VA_ARGS__); \
    } while (false)

#define print(format, ...)   \
    do {                     \
        UNUSED(format);      \
        UNUSED(__VA_ARGS__); \
    } while (false)

#else

// `print` prints to the standard output (terminal/command prompt). This is functionally similar to printf
//
// if `NOPRINT` is defined, this doesn't do anything and simply ignores passed
// arguments.
int print(const char* restrict format, ...);

// `println` prints to the standard output (terminal/command prompt) adding a
// new line afterwards. `format` should be a string literal or constant.
//
// if `NOPRINT` is defined, this doesn't do anything and simply ignores passed
// arguments.
int println(const char* restrict format, ...);
#endif

// `len` gets the length of a constant width array.
//
// This should not be used with pointers since the size of an array from a
// pointer may not be known at compile time.
#define len(array, type) (sizeof(array) / sizeof(type))

void copy(const void* restrict src, void* restrict dst, size_t size);

// `copyPad` copies as much from `src` into `dst` up to the size specified. If
// `src` is smaller than size, the rest of `dst` is padded with null bytes
// ('\0').
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