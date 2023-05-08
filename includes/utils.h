#ifndef Utils_H
#define Utils_H

#include "types.h"

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
#define println(format, ...) printf(format "\n"__VA_OPT__(, __VA_ARGS__))
#endif

// `len` gets the length of a constant width array.
//
// This should not be used with pointers since the size of a an array from a
// pointer might not be known at compile time.
#define len(array, type) (sizeof(array) / sizeof(type))

// `allocate` easily allocates and initializes to zero a chunk of memory the
// size of the specified `type`.
#define allocate(type) (type*)calloc(1, sizeof(type))
#define allocateN(type, count) (type*)calloc(count, sizeof(type))

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

#endif  // Utils_H