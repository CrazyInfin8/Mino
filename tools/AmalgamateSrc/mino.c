#if defined(PLATFORM_Linux)
#define Window MinoWindow
#endif

#include "../src/aff3.c"
#include "../src/audio.c"
#include "../src/consts.c"
#include "../src/gamepad.c"
#include "../src/list.c"
#include "../src/synth.c"
#include "../src/utils.c"
#include "../src/vec2.c"
#include "../src/window.c"

#undef Window
#include "../src/windowNative.c"