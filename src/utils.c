#include "types.h"
#include "utils.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#ifndef NOPRINT
#include <stdio.h>

int print(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    int count = vprintf(format, args);
    va_end(args);
    return count;
}

int println(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    int count = vprintf(format, args);
    va_end(args);
    count += putchar('\n');
    return count;
}
#endif

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

