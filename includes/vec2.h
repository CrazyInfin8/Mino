#ifndef Vec2_H
#define Vec2_H

#include <math.h>

#include "consts.h"
#include "types.h"

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

Vec2 Vec2Add(Vec2 vector1, Vec2 vector2);

Vec2 Vec2Translate(Vec2 vector, float32 x, float32 y);

Vec2 Vec2Subtract(Vec2 vector1, Vec2 vector2);

Vec2 Vec2Divide(Vec2 vector1, Vec2 vector2);

Vec2 Vec2Multiply(Vec2 vector1, Vec2 vector2);

Vec2 Vec2Scale(Vec2 vector, float32 x, float32 y);

float32 Vec2Dist2(Vec2 vector1, Vec2 vector2);

float32 Vec2Dist(Vec2 vector1, Vec2 vector2);

#endif  // Vec2_H