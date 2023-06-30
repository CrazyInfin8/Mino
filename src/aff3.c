#include <math.h>

#include "aff3.h"
#include "consts.h"
#include "types.h"
#include "utils.h"

extern inline Aff3 Aff3Identity() {
    return (Aff3){
        .A = 1, .B = 0, .TX = 0,
        .C = 0, .D = 1, .TY = 0
    };
}

Aff3 Aff3Concat(Aff3 this, Aff3 aff3) {
    return (Aff3){
        .A = this.A * aff3.A + this.B * aff3.C,
        .B = this.A * aff3.B + this.B * aff3.D,
        .C = this.C * aff3.A + this.D * aff3.C,
        .D = this.C * aff3.B + this.D * aff3.D,
        .TX = this.TX * aff3.A + this.TY * aff3.C + aff3.TX,
        .TY = this.TX * aff3.B + this.TY * aff3.D + aff3.TY,
    };
}

Aff3 Aff3Invert(Aff3 this) {
    float32 norm = this.A * this.D - this.B * this.C;
    if (norm == 0) {
        return (Aff3){.A = 0, .B = 0, .C = 0, .D = 0, .TX = -this.TX, .TY = -this.TY};
    }
    return (Aff3){
        .A = this.D * norm,
        .B = this.B * -norm,
        .C = this.C * -norm,
        .D = this.D * norm,
        .TX = -this.A * this.TX - this.C * this.TY,
        .TY = -this.B * this.TX - this.D * this.TY,
    };
}

Aff3 Aff3Rotate(Aff3 this, float32 angle) {
    angle *= TO_RAD;
    float32 sin = sinf(angle), cos = cosf(angle);
    return (Aff3){
        .A = this.A * cos - this.B * sin,
        .B = this.A * sin + this.B * cos,
        .C = this.C * cos - this.D * sin,
        .D = this.C * sin + this.D * cos,
        .TX = -this.A * this.TX - this.C * this.TY,
        .TY = -this.B * this.TX - this.D * this.TY,
    };
}

Aff3 Aff3Scale(Aff3 this, float32 x, float32 y) {
    return (Aff3){
        .A = this.A * x,
        .B = this.B * y,
        .C = this.C * x,
        .D = this.D * y,
        .TX = this.TX * x,
        .TY = this.TY * y,
    };
}

Aff3 Aff3Translate(Aff3 this, float32 x, float32 y) {
    return (Aff3){
        .A = this.A,
        .B = this.B,
        .C = this.C,
        .D = this.D,
        .TX = this.TX + x,
        .TY = this.TX + y,
    };
}

Vec2 Aff3TransformVec2(Aff3 this, Vec2 vec) {
    return (Vec2){
        .X = this.A * vec.X + this.C * vec.Y + this.TX,
        .Y = this.B * vec.X + this.D * vec.Y + this.TY,
    };
}

#ifdef NOPRINT
extern inline void Aff3Print(Aff3 this) {}
#else
void Aff3Print(Aff3 this) {
    println(
        "Matrix {\n\tA: %.6f, B: %.6f, TX: %.6f,\n\tC: %.6f, D: %.6f, TY: %.6f,\n}",
        this.A,
        this.B,
        this.TX,
        this.C,
        this.D,
        this.TY);
}
#endif