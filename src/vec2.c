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