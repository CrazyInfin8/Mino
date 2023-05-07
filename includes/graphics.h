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

// `GraphicsInit` initializes the platforms graphics API on the current window.
// It returns true when successful or false if there was an error.
//
// Remember to close the graphics api by calling `GraphicsClose`.
bool GraphicsInit(Window* window);

// `GraphicsClose` cleans up the graphics api.
//
// It should be called when you are finished drawing graphics on this window.
void GraphicsClose(Window* window);

// `GraphicsClear` clears the screen of graphics that have been drawn.
void GraphicsClear(Window* window);

// `GraphicsBegin` prepares the graphics API to draw a series of vertices.
//
// It should be followed by a call to `GraphicsEnd` when you finished drawing
// the frame.
void GraphicsBegin(Window* window);

// `GraphicsEnd` should be called after `GraphicsBegin` to signify that you are
// done drawing this frame.
void GraphicsEnd();

// `GraphicsAddVertex` adds a vertex to the platforms graphics API.
//
// You should call `GraphicsBegin` before you call `GraphicsAddVertex` to
// prepare the graphics API for drawing.
void GraphicsAddVertex(float32 vertex[3]);

// `GraphicsAddColor` adds a color vertex to the platforms graphics API.
void GraphicsAddColor(Color color);

#endif  // Graphics_H