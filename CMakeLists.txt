cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Debug/Release optimalisaties ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# --- SDL3 shared ---
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)

# --- SDL3_image shared ---
set(SDL3IMAGE_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(SDL3IMAGE_BUILD_STATIC_LIBS OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_TESTS OFF CACHE BOOL "" FORCE)

# --- SDL3_ttf shared, always vendored on Windows ---
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3TTF_HARFBUZZ ON CACHE BOOL "" FORCE)
set(SDL3TTF_HARFBUZZ_SHARED OFF CACHE BOOL "" FORCE)
set(SDL3TTF_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(SDL3TTF_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL3TTF_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL3TTF_TESTS OFF CACHE BOOL "" FORCE)

# --- Externe libraries ---
add_subdirectory(external/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL3_image EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL3_TTF EXCLUDE_FROM_ALL)

# --- Engine library ---
add_library(Engine SHARED
        src/Engine/GameEngine.cpp
        src/Engine/TimeManager.cpp
        src/GameObjects/GameObject.cpp
        src/GameObjects/Texture.cpp
        src/GameObjects/Component/Behaviour.cpp
        src/GameObjects/Component/SpriteRenderer.cpp
        src/GameObjects/Spritesheet/Animator.cpp
        src/GameObjects/Spritesheet/Frame.cpp
        src/GameObjects/Spritesheet/SpriteSheet.cpp
        src/GameObjects/Transform/Position.cpp
        src/GameObjects/Transform/Rotation.cpp
        src/GameObjects/Transform/Scale.cpp
        src/GameObjects/Transform/Size.cpp
        src/GameObjects/Transform/Transform.cpp
        src/Input/InputSystem.cpp
        src/Input/KeyInputComponent.cpp
        src/Input/MouseInputComponent.cpp
        src/Scenes/Scene.cpp
        src/Scenes/SceneManager.cpp
        src/Scenes/SceneSystem.cpp
        src/SDL/Window.cpp
        src/UI/Button.cpp
        src/UI/Color.cpp
        src/UI/Font.cpp
        src/UI/Text.cpp
        src/Animation/Animation.cpp
        src/Animation/Keyframe.cpp
        src/Animation/Converter/ConverterFactory.cpp
        src/Animation/Converter/LerpConverter.cpp
        src/Animation/Converter/EieoConverter.cpp
        src/Animation/Converter/BounceConverter.cpp
        src/Animation/Converter/ElasticConvert.cpp)

target_precompile_headers(Engine
        PRIVATE
        <SDL3/SDL.h>
        <SDL3_image/SDL_image.h>
        <SDL3_ttf/SDL_ttf.h>
)

target_include_directories(Engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(Engine
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)

# --- Game executable ---
add_executable(GameEngine
        game/Main.cpp
)

target_precompile_headers(GameEngine
        REUSE_FROM Engine
)

target_link_libraries(GameEngine
        PRIVATE
        Engine
)

target_include_directories(GameEngine
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# --- Post-build: DLLs koppiëren ---
add_custom_command(TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
        $<TARGET_FILE_DIR:GameEngine>
)

# --- copy resources directory ---
add_custom_command(TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:GameEngine>/resources
)