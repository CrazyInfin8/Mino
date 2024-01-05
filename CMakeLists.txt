cmake_minimum_required(VERSION 3.25)

# Configuration variables
set(VERBOSE true) # Print some information on this CMake script.
set(MINO_GAME_SOURCE "project") # Build a game executable using this folder.

if(NOT DEFINED MINO_OUT_DIR)
    file(REAL_PATH "out" MINO_OUT_DIR)
else()
    file(REAL_PATH ${MINO_OUT_DIR} MINO_OUT_DIR)
endif()

if(NOT DEFINED VERBOSE)
    set(VERBOSE true)
endif()

if(DEFINED MINO_GAME_SOURCE)
    file(REAL_PATH ${MINO_GAME_SOURCE} MINO_GAME_SOURCE)
endif()

# Configure the actual project now
project(Mino)

file(GLOB MINO_SOURCES "src/*.c")

# Configure Mino for each platforms that Mino supports
if(APPLE)
    message(STATUS "Enabling Objective-C for macOS")
    enable_language(OBJC)

    add_definitions(-DPlatform_MacOS)
    file(GLOB MINO_PLATFORM_SOURCES "src/macos/*.m")
    set(MINO_PLATFORM_DIR "macos")
    
    find_library(LIB_COCOA Cocoa)
    find_library(LIB_METAL Metal)
    set(LIBRARIES ${LIB_COCOA} ${LIB_METAL})
elseif(LINUX)
    # set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -nostdlib")
    enable_language(CXX)
    
    add_definitions(-DPlatform_Linux)
    file(GLOB MINO_PLATFORM_SOURCES "src/linux/*.c")
    set(MINO_PLATFORM_DIR "linux")

    # libraries
    find_library(LIB_MATH m)
    find_library(LIB_X11 X11)
    find_library(LIB_UDEV udev)
    find_library(LIB_GL GL)
    find_Library(LIB_ASOUND asound)
    set(LIBRARIES  ${LIB_MATH} ${LIB_X11} ${LIB_UDEV} ${LIB_GL} ${LIB_ASOUND})

elseif(WIN32 OR MINGW OR MSYS)
    if(MINGW OR MSYS)
        message(STATUS "Configuring for MSYS/MinGW")
        # set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -nostdlib")
        link_directories("C:/Windows/System32")
    else()
        message(STATUS "Configuring for MSVC")
        # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")

    endif()

    enable_language(CXX)

    add_definitions(-DPlatform_Windows)
    file(GLOB MINO_PLATFORM_SOURCES "src/windows/*.c")
    set(MINO_PLATFORM_DIR "windows")

    # libraries
    # find_library(LIB_GDI32 gdi32)
    find_library(LIB_GDI32 gdi32)
    find_library(LIB_WINMM winmm)
    find_library(LIB_OPENGL32 opengl32)
    find_library(LIB_XINPUT xinput)
    set(LIBRARIES ${LIB_GDI32} ${LIB_WINMM} ${LIB_OPENGL32} ${LIB_XINPUT})

else()
    message(FATAL_ERROR "🛑 — Mino does not currently support the platform: ${CMAKE_SYSTEM_NAME}.")

endif()

if(VERBOSE)
    string(REPLACE ";" "\n\t" MINO_SOURCES_VERBOSE "${MINO_SOURCES}")
    message(STATUS "🐭 — Sources to compile:\n\t${MINO_SOURCES_VERBOSE}")
    string(REPLACE ";" "\n\t" MINO_PLATFORM_SOURCES_VERBOSE "${MINO_PLATFORM_SOURCES}")
    message(STATUS "🐭 — Platform specific sources to compile:\n\t${MINO_PLATFORM_SOURCES_VERBOSE}")
    string(REPLACE ";" "\n\t" LIBRARIES_VERBOSE "${LIBRARIES}")
    message(STATUS "🐭 — Libraries to include: \n\t${LIBRARIES_VERBOSE}")
endif()

# Include Mino's header files
include_directories("includes")

# Build Mino's libraries

add_library(Mino STATIC ${MINO_SOURCES} ${MINO_PLATFORM_SOURCES})
target_link_libraries(Mino ${LIBRARIES})

add_library(MinoShared SHARED ${MINO_SOURCES} ${MINO_PLATFORM_SOURCES})
target_link_libraries(MinoShared ${LIBRARIES})

set(MINO_LIB_DIR_RELEASE "${MINO_OUT_DIR}/${MINO_PLATFORM_DIR}/lib")
set(MINO_LIB_DIR_DEBUG "${MINO_OUT_DIR}/${MINO_PLATFORM_DIR}/debug/lib")
set(MINO_BIN_DIR_RELEASE "${MINO_OUT_DIR}/${MINO_PLATFORM_DIR}/bin")
set(MINO_BIN_DIR_DEBUG "${MINO_OUT_DIR}/${MINO_PLATFORM_DIR}/debug/bin")

if(VERBOSE)
    message(STATUS "🐭 — Outputting libraries to: \n\t${MINO_LIB_DIR_RELEASE}")
    message(STATUS "🐭 — Debug libraries to: \n\t${MINO_LIB_DIR_DEBUG})")
endif()

set_target_properties(Mino MinoShared
PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${MINO_LIB_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MINO_LIB_DIR_DEBUG}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MINO_LIB_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${MINO_LIB_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MINO_BIN_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY "${MINO_LIB_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MINO_LIB_DIR_DEBUG}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MINO_LIB_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${MINO_LIB_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MINO_BIN_DIR_RELEASE}"
)

if(NOT DEFINED MINO_GAME_SOURCE)
    message(STATUS "⚠️ — No executable source found. Nothing else to do.")
    return()
endif()

if(VERBOSE)
    message(STATUS "🕹️ — Game found! configuring...")
endif()

file(GLOB MINO_GAME_SOURCES "${MINO_GAME_SOURCE}/*.c")

if(VERBOSE)
    string(REPLACE ";" "\n\t" MINO_GAME_SOURCES_VERBOSE "${MINO_GAME_SOURCES}")
    message(STATUS "🕹️ — Game sources to compile:\n\t${MINO_GAME_SOURCES_VERBOSE}")
    message(STATUS "🕹️ — Outputting executables to: \n\t${MINO_LIB_DIR_RELEASE}")
    message(STATUS "🕹️ — Debug executables to: \n\t${MINO_LIB_DIR_DEBUG})")
endif()

add_executable(MinoGame ${MINO_GAME_SOURCES})
target_link_libraries(MinoGame Mino)

set_target_properties(MinoGame
PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${MINO_BIN_DIR_RELEASE}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MINO_BIN_DIR_DEBUG}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MINO_BIN_DIR_RELEASE}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${MINO_BIN_DIR_RELEASE}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MINO_BIN_DIR_RELEASE}"
)

find_program(UPX upx)
if(UPX)
    message(STATUS "📦 — UPX found in: \n\t${UPX}")
    message(STATUS "📦 — UPX will be used to compress your game further in the `Compress` target.")

    set(GAME_FILE "${MINO_BIN_DIR_RELEASE}/MinoGame${CMAKE_EXECUTABLE_SUFFIX}")
    set(UPX_FILE "${MINO_BIN_DIR_RELEASE}/MinoGame.upx${CMAKE_EXECUTABLE_SUFFIX}")

    add_custom_command(
        OUTPUT ${UPX_FILE}
        COMMAND ${CMAKE_COMMAND} -E rm -f ${UPX_FILE}
        COMMAND ${UPX} --ultra-brute -o${UPX_FILE} $<TARGET_FILE:MinoGame>
        DEPENDS MinoGame
    )
    add_custom_target(Compress
        DEPENDS MinoGame
        DEPENDS ${UPX_FILE}
    )

    add_dependencies(Compress MinoGame)

else()
    message(STATUS "⚠️ — Could not find UPX. No compression feature available.")
endif()
