#ifndef Graphics_H
#define Graphics_H

#include "types.h"
#include "window.h"

// `Color` represents an RGBA color value.
//
// By varying the intensity of red (`R`), green (`G`), and blue (`B`), you
// can create a wide range of color. Alpha (`A`) specifies how transparent or
// opaque the color is.
typedef struct Color {
    uint8 R;
    uint8 G;
    uint8 B;
    uint8 A;
} Color, RGBA;

// `ColorHex` creates a Color object using a hexadecimal value in ARGB format.
Color ColorHex(const uint32 color);

// `GraphicsInit` creates an OpenGL context for the current window. It returns
// true when successful or false if there was an error.
//
// Remember to close the graphics api by calling `GraphicsClose`. (This should
// be done before disposing the window)
bool GraphicsInit(Window* window);

// `GraphicsClose` frees the OpenGL context for this window.
//
// It should be called when you are finished drawing graphics on this window.
void GraphicsClose(Window* window);

// `GraphicsMakeCurrent` makes the OpenGL api use this window and its context.
//
// This should be called before doing OpenGL operations to ensure that OpenGL
// functions target this window and context
void GraphicsMakeCurrent(Window* window);

#endif  // Graphics_H