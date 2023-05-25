# Mino - (Pronounced: ミノ) - Minimalist Game Framework

Mino is a minimalistic game framework similar to GLFW and SDL2 but without the bloat. Mino is meant to make it easier to create cross platform games with tiny binary sizes.

Mino is still under development but is available in case others may want to use it as reference.

# Dependancies

While Mino is fully self contained and the user won't need to install anything to run your game. Aside from the obvious (A C compiler and `make`) Some platforms may need extra tools to properly compile. Also Mino can use optional tools like UPX to compress your binary after building.

Most Linux distributions often comes with OpenGL, X11, Udev, and ALSA but they often don't come with the headers for new programs to compile to these libraries. If you have errors of missing header files, try seeing if you need to install `libx11-dev`, `libasound2-dev`, `libgl-dev`, `libudev-dev` (some distributions may use different names, you'll have to check for yourself). Also UPX can be found in the `upx-ucl` package, though this is optional.

To install these libraries using Ubuntu:

```bash
sudo apt install -y libx11-dev libasound2-dev libgl-dev libudev-dev upx-ucl
```

# But why Mino instead of X?

Libraries like **GLFW**, **SDL2**, **Raylib**, etc. are bloated for what Mino is meant for. Mino is being created for a game jam where binary file size matters immensely (Hard limits of 4MBs for the entire game but my target is 32KBs). Libraries like Raylib also depend on other libraries like GLFW, Miniaudio, etc. Mino uses libraries that exist on the target system (for example, Windows implementations should only uses the win32 API).

Libraries like [**Fenster**](https://github.com/zserge/fenster) are way too bare bones and not easy for people to dig into it's code (and had some subtle problems I had to change anyway). While Mino is trying to be minimalist, it is still trying to be approachable. Mino is a lot more verbose as it encourages people to dig into it's source code. Mino also provides a little more than the bare minimum to make it easier to use such as a graphic api interface (TODO), math types such as vectors and matrices, gamepad support, audio synthesis (TODO), etc.

Libraries like [**GLBind**](https://github.com/mackron/glbind) still require the user to initialize stuff like the window per each platform. Mino is meant to set everything up without having the user touch a single platform specific header.
