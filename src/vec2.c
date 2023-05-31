#include <math.h>

#include "aff3.h"
#include "types.h"
#include "vec2.h"

extern inline Vec2 Vec2TransformAff3(Vec2 this, Aff3 aff3) {
    return Aff3TransformVec2(aff3, this);
}

Vec2 Vec2Rotate(Vec2 this, Vec2 pivot, float32 angle) {
    angle *= TO_RAD;
    float32 sin = sinf(angle), cos = cosf(angle);
    float32 dx = this.X - pivot.X;
    float32 dy = this.Y - pivot.Y;

    return (Vec2){
        .X = cos * dx - sin * dy + pivot.X,
        .Y = sin * dx + cos * dy + pivot.Y,
    };
}

extern inline Vec2 Vec2Add(Vec2 vector1, Vec2 vector2) {
    return (Vec2){
        .X = vector1.X + vector2.X,
        .Y = vector1.Y + vector2.Y,
    };
}

extern inline Vec2 Vec2Translate(Vec2 vector, float32 x, float32 y) {
    return (Vec2){
        .X = vector.X + x,
        .Y = vector.Y + y,
    };
}

extern inline Vec2 Vec2Subtract(Vec2 vector1, Vec2 vector2) {
    return (Vec2){
        .X = vector1.X - vector2.X,
        .Y = vector1.Y - vector2.Y,
    };
}

extern inline Vec2 Vec2Divide(Vec2 vector1, Vec2 vector2) {
    return (Vec2){
        .X = vector1.X / vector2.X,
        .Y = vector1.Y / vector2.Y,
    };
}

extern inline Vec2 Vec2Multiply(Vec2 vector1, Vec2 vector2) {
    return (Vec2){
        .X = vector1.X * vector2.X,
        .Y = vector1.Y * vector2.Y,
    };
}

extern inline Vec2 Vec2Scale(Vec2 vector, float32 x, float32 y) {
    return (Vec2){
        .X = vector.X * x,
        .Y = vector.Y * y,
    };
}

float32 Vec2Dist2(Vec2 vector1, Vec2 vector2) {
    Vec2 distance = Vec2Subtract(vector2, vector1);
    return distance.X * distance.X + distance.Y * distance.Y;
}

float32 Vec2Dist(Vec2 vector1, Vec2 vector2) {
    float32 distance2 = Vec2Dist2(vector1, vector2);
    return sqrtf(distance2);
}

float32 Vec2AngleBetween(Vec2 vector1, Vec2 vector2) {
    return atan2f(vector2.Y - vector1.Y, vector2.X - vector1.X) * TO_DEG;
}