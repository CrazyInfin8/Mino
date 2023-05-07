#ifndef Vec2_H
#define Vec2_H

#include <types.h>
#include <consts.h>
#include <math.h>

typedef struct Vec2 {
    float32 X, Y;
} Vec2;

typedef struct Aff3 Aff3;

Vec2 Vec2TransformAff3(Vec2 this, Aff3 aff3);

Vec2 Vec2Rotate(Vec2 this, Vec2 pivot, float32 angle);

#endif