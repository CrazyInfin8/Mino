#ifndef Utils_H
#define Utils_H

#ifdef NOPRINT
#define print(format, ...) \
    do {                   \
    } while (false)
#define println(format, ...) \
    do {                     \
    } while (false)
#else
int printf(const char* restrict format, ...);
#define print(format, ...) printf(format __VA_OPT__(, __VA_ARGS__))
#define println(format, ...) printf(format "\n"__VA_OPT__(, __VA_ARGS__))
#endif

// len gets the length of a constant width array.  
#define len(array, type) (sizeof(array) / sizeof(type))

#define allocate(type) (type*)calloc(1, sizeof(type))

#define bitsSet(bits, condition) (((bits) & (1 << (condition))) == (1 << (condition)))
#define bitsUnset(bits, condition) (((bits) & (1 << (condition))) != (1 << (condition)))

#define setBit(data, bits) ((data) |= 1 << (bits))
#define unsetBit(data, bits) ((data) &= ~(1 << (bits)))

#endif  // Utils_H