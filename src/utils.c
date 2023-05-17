#include "types.h"

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