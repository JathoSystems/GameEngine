cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS ON)
set(CATCH_CONFIG_DEFAULT_REPORTER "junit")
set(CATCH_CONFIG_CONSOLE_WIDTH 200)


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

include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

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
        src/Animation/Converter/ElasticConvert.cpp
        src/Events/EventManager.cpp
        src/Events/EventRegistry.cpp
        src/AI/pathfinding/AStarPathfinder.cpp
        src/AI/pathfinding/PathfinderFactory.cpp
        src/Scenes/Camera/Camera.cpp
        src/Scenes/Camera/FixedCamera.cpp
        src/Scenes/Camera/AttachedCamera.cpp
        src/Scenes/Camera/Viewport.cpp
        includes/AI/states/State.hpp
        src/AI/states/State.cpp
        includes/AI/transition/ITransition.h
        src/AI/states/StateManager.cpp
        includes/AI/components/AiController.hpp
        src/AI/components/AiController.cpp
        includes/AI/states/StateRegistry.h
        src/AI/states/StateRegistry.cpp
        src/AI/transition/LambdaTransition.cpp
        src/AI/system/AiSystem.cpp
        includes/AI/pathfinding/heuristics/IHeuristicCalculator.hpp
        src/AI/pathfinding/heuristic/EuclideanHeuristicCalculator.cpp
        src/AI/pathfinding/heuristic/ManhattenHeuristicCalculator.cpp
        src/AI/pathfinding/heuristic/HeuristicCalculatorFactory.cpp
)

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
# --- Tests ---
enable_testing()

add_executable(EngineTests
        tests/AI/states/StateManagerTests.cpp
        tests/AI/states/StateRegistryTests.cpp
        tests/AI/pathfinding/AStarPathfinderTests.cpp
        tests/AI/components/AiComponentTests.cpp
        tests/Events/EventManagerTests.cpp
        tests/Events/EventRegistryTests.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

target_link_libraries(EngineTests
        PRIVATE
        Engine
        Catch2::Catch2WithMain
)

target_include_directories(EngineTests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Copy DLLs BEFORE test discovery runs
add_custom_command(TARGET EngineTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Engine>
        $<TARGET_FILE_DIR:EngineTests>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
        $<TARGET_FILE_DIR:EngineTests>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Catch2::Catch2>
        $<TARGET_FILE:Catch2::Catch2WithMain>
        $<TARGET_FILE_DIR:EngineTests>
)

# Windows-only: Copy MinGW runtime DLLs
if(WIN32 AND MINGW)
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    add_custom_command(TARGET EngineTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:EngineTests>
    )
endif()

include(CTest)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# Use PRE_TEST discovery mode to avoid DLL issues during CMake configuration
# Tests will be discovered when you actually run CTest, not during build
catch_discover_tests(EngineTests
        DISCOVERY_MODE PRE_TEST
)
