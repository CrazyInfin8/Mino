#ifndef Svg_H
#define Svg_H

#include "types.h"
#include "vec2.h"
#include "graphics.h"

typedef struct Op {
	enum {
		Op_Move,
		Op_Line,
		Op_Cube,
		Op_Quad,
		Op_Arc,
		Op_Close,

	} type;
	float points[6];
} Op;

typedef struct Path {
	int len;
	Op *op;
} Path;

typedef struct Shape {
	int len;
	Path *path;
} Shape;

void drawShape(Shape const* shape);
void drawShapeTransformed(Shape const* shape, Vec2 position, float32 scale, float32 angle, Color stroke);


#endif