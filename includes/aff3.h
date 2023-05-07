#ifndef Aff3_H
#define Aff3_H

#include <types.h>
#include <vec2.h>

// `Aff3` implements a 3x3 affine transformation matrix where the bottom row is
// always implicitly `[0, 0, 1]`.
//
// Note: None of `Aff3` functions modify the `Aff3`. Instead it returns a new
// `Aff3` which you can use to set the original `Aff3 if you want.
typedef struct Aff3 {
    float32 A, B, C, D, TX, TY;
} Aff3;

// `Aff3Identity` returns an identity matrix.
//
// This is defined as: `[[1, 0, 0], [0, 1, 0], [0, 0, 1]]`.
Aff3 Aff3Identity();

// `Aff3Concat` applies the transformations from `aff3` onto `this` matrix.
//
// The order of matrices you concatenate matters. For example, concatenating a
// rotation matrix to a translation matrix may have different results than
// concatenating those same translation and a rotation matrix.
Aff3 Aff3Concat(Aff3 this, Aff3 aff3);

// `Aff3Invert` reverses the transformations applied from `this` matrix.
//
// Objects transformed by the original matrix will revert back to their original
// transformations when you apply this transformation.
Aff3 Aff3Invert(Aff3 this);

Aff3 Aff3Rotate(Aff3 this, float32 angle);

Aff3 Aff3Scale(Aff3 this, float32 x, float32 y);

Aff3 Aff3Translate(Aff3 this, float32 x, float32 y);

// `Aff3TransformVec2` applies the matrix transformation to a `Vec2`.
//
// This applies all transformations applied to this matrix onto that `Vec2`.
Vec2 Aff3TransformVec2(Aff3 this, Vec2 vec);

// `Aff3Print` prints the matrix in a neat way. Useful for debugging.
//
// Note: if `NOPRINT` is defined, this may not print to the console.
void Aff3Print(Aff3 this);

#endif