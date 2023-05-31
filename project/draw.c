#include "types.h"
#include "vec2.h"
#include "nanovg/nanovg.h"
#include "./svg.h"
#include "graphics.h"
extern NVGcontext* vg;

void drawPolygon(float32 radius, int sides) {
    nvgSave(vg);

    Vec2 vec = (Vec2){radius, 0};

    float32 angle = (float32)360 / (float32)sides;

    nvgBeginPath(vg);
    nvgMoveTo(vg, vec.X, vec.Y);
    for (int i = 0; i < sides; i++) {
        vec = Vec2Rotate(
            vec,
            (Vec2){0, 0},
            -angle);
        nvgLineTo(vg, vec.X, vec.Y);
    }
    nvgClosePath(vg);
    nvgStroke(vg);

    nvgRestore(vg);
}

// void drawShip(float32 radius) {
//     nvgSave(vg);

//     nvgBeginPath(vg);
//     nvgMoveTo(vg, radius, 0);

//     nvgLineTo(vg, float x, float y)
// }

extern const Shape assets_ship__svg;
void drawShape(Shape const* shape) {
    for (int i = 0; i < shape->len; i++) {
        Path* path = &shape->path[i];
        for (int i = 0; i < path->len; i++) {
            Op* op = &path->op[i];
            switch (op->type) {
                case Op_Move:
                    nvgMoveTo(vg, op->points[0], op->points[1]);
                    break;

                case Op_Line:
                    nvgLineTo(vg, op->points[0], op->points[1]);
                    break;

                case Op_Quad:
                    nvgQuadTo(vg, op->points[0], op->points[1], op->points[2], op->points[3]);
                    break;

                case Op_Cube:
                    nvgBezierTo(vg, op->points[0], op->points[1], op->points[2], op->points[3], op->points[4], op->points[5]);
                    break;

                case Op_Close:
                    nvgClosePath(vg);
                    break;

                default:
            }
        }
    }
}

void drawShapeTransformed(Shape const* shape, Vec2 position, float32 scale, float32 angle, Color stroke) {
    nvgSave(vg);

    nvgSave(vg);
    // nvgScale(vg, scale, scale);
    nvgBeginPath(vg);
    nvgCircle(vg, position.X, position.Y, 5);
    nvgFillColor(vg, nvgRGB(0xff, 0, 0));
    nvgFill(vg);
    nvgRestore(vg);

    nvgTranslate(vg, position.X, position.Y);
    // nvgTranslate(vg, 250 * scale, 250 * scale);
    nvgRotate(vg, angle);
    nvgTranslate(vg, -250 * scale, -250 * scale);
    nvgScale(vg, scale, scale);

    nvgBeginPath(vg);
    for (int i = 0; i < shape->len; i++) {
        Path* path = &shape->path[i];
        for (int i = 0; i < path->len; i++) {
            Op* op = &path->op[i];
            switch (op->type) {
                case Op_Move:
                    nvgMoveTo(vg, op->points[0], op->points[1]);
                    break;

                case Op_Line:
                    nvgLineTo(vg, op->points[0], op->points[1]);
                    break;

                case Op_Quad:
                    nvgQuadTo(vg, op->points[0], op->points[1], op->points[2], op->points[3]);
                    break;

                case Op_Cube:
                    nvgBezierTo(vg, op->points[0], op->points[1], op->points[2], op->points[3], op->points[4], op->points[5]);
                    break;

                case Op_Close:
                    nvgClosePath(vg);
                    break;

                default:;
            }
        }
    }
    nvgStrokeColor(vg, nvgRGBA(stroke.R, stroke.G, stroke.B, stroke.A));
    nvgStrokeWidth(vg, 1 / scale);
    nvgStroke(vg);

    nvgRestore(vg);
}