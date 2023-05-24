# Mino - (Pronounced: ミノ) - Minimalist Game Framework

Mino is a minimalistic game framework similar to GLFW and SDL2 but without the bloat. Mino is meant to make it easier to create cross platform games with tiny binary sizes.

Mino is still under development but is available in case others may want to use it as reference.

# But why Mino instead of X?

Libraries like **GLFW**, **SDL2**, **Raylib**, etc. are bloated for what Mino is meant for. Mino is being created for a game jam where binary file size matters immensely (Hard limits of 4MBs for the entire game but my target is 32KBs). Libraries like Raylib also depend on other libraries like GLFW, Miniaudio, etc. Mino uses libraries that exist on the target system (for example, Windows implementations should only uses the win32 API).

Libraries like [**Fenster**](https://github.com/zserge/fenster) are way too bare bones and not easy for people to dig into it's code (and had some subtle problems I had to change anyway). While Mino is trying to be minimalist, it is still trying to be approachable. Mino is a lot more verbose as it encourages people to dig into it's source code. Mino also provides a little more than the bare minimum to make it easier to use such as a graphic api interface (TODO), math types such as vectors and matrices, gamepad support, audio synthesis (TODO), etc.

Libraries like [**GLBind**](https://github.com/mackron/glbind) still require the user to initialize stuff like the window per each platform. Mino is meant to set everything up without having the user touch a single platform specific header.
