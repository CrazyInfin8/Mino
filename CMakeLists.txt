cmake_minimum_required(VERSION 3.0)
project(Mino)

include_directories(
    includes/
)

add_definitions(
    -DPLATFORM_Windows
)

add_executable(game
    src/*.c
)

target_link_libraries(game
    gdi32
    opengl32
    winmm
)