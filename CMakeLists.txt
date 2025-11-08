cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(includes)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        game/*.cpp
)

add_executable(GameEngine ${SOURCES}
        includes/SDL/Window.h
        src/SDL/Window.cpp
        includes/GameObjects/SpriteRenderer.h
        src/GameObjects/SpriteRenderer.cpp
        includes/GameObjects/Behaviour.h
        src/GameObjects/Behaviour.cpp
)

target_compile_options(GameEngine PRIVATE -Wall -Wextra -Wpedantic)

# --- Fetch SDL2 ---
include(FetchContent)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.28.5
)
FetchContent_MakeAvailable(SDL2)

# --- Fetch SDL2_image ---
FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
)
FetchContent_MakeAvailable(SDL2_image)

# --- Link dependencies ---
target_link_libraries(GameEngine
        PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
)
