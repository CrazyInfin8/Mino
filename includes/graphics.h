#ifndef Graphics_H
#define Graphics_H

#include <window.h>
#include <types.h>

typedef struct Color {
    uint8 R;
    uint8 G;
    uint8 B;
    uint8 A;
} Color, RGBA;

Color ColorHex(const uint32 color);

bool GraphicsInit(Window* window);

void GraphicsClose(Window* window);

void GraphicsClear(Window* window);

void GraphicsBegin(Window* window);

void GraphicsEnd();

void GraphicsAddVertex(float32 vertex[3]);

void GraphicsAddColor(Color color);
#endif