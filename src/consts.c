#include "types.h"

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