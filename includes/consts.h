#ifndef Consts_H
#define Consts_H

#include "types.h"

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
