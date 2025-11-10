cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Forceer static libraries voor beide
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)

# Voeg SDL3 toe
add_subdirectory(external/SDL3)

# Forceer SDL3_image ook static
set(SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)

# Voeg SDL3_image toe
add_subdirectory(external/SDL3_image)

# Source bestanden
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        game/*.cpp
)

# Executable
add_executable(GameEngine ${SOURCES}
        includes/GameObjects/Spritesheet/Spritesheet.h
        includes/GameObjects/Spritesheet/Frame.h
        includes/GameObjects/Texture.h
        src/GameObjects/Texture.cpp
        src/GameObjects/Spritesheet/Frame.cpp
        src/GameObjects/Spritesheet/SpriteSheet.cpp
        includes/GameObjects/Spritesheet/Animator.h
        src/GameObjects/Spritesheet/Animator.cpp)

# Link libraries - gebruik expliciet de static versies
target_link_libraries(GameEngine PRIVATE
        SDL3::SDL3-static
        SDL3_image::SDL3_image-static
)

# Include directories
target_include_directories(GameEngine
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Compiler opties
target_compile_options(GameEngine PRIVATE -Wall -Wextra -Wpedantic)

# Windows-specifiek
if(WIN32)
    target_compile_definitions(GameEngine PRIVATE SDL_MAIN_HANDLED)
endif()