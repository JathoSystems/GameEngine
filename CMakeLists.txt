cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Debug/Release optimizations ---
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif ()

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

# --- External libraries (local checkouts) ---
add_subdirectory(external/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL3_image EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL3_TTF EXCLUDE_FROM_ALL)

include(FetchContent)

# Catch2
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# nlohmann/json
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

set(DR_MP3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(DR_MP3_FILE "${DR_MP3_DIR}/dr_mp3.h")

# Ensure Box2D tests/testbed are disabled (applies whether local or fetched)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)

# Allow overriding which Box2D ref to fetch; default to v3.0.0 (modern C API)
if (NOT DEFINED BOX2D_GIT_TAG)
    set(BOX2D_GIT_TAG "v3.0.0" CACHE STRING "Box2D git tag/branch/commit to fetch")
endif()

# Prefer a local checkout if present, otherwise fetch Box2D via FetchContent
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/box2d/CMakeLists.txt")
    message(STATUS "Using local external/box2d")
    add_subdirectory(external/box2d EXCLUDE_FROM_ALL)
else()
    message(STATUS "Fetching Box2D via FetchContent (ref: ${BOX2D_GIT_TAG})")
    FetchContent_Declare(
            box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d.git
            GIT_TAG ${BOX2D_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    # Make sure Box2D options are off for tests/testbed
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(box2d)
endif()

# Download dr_mp3.h if missing
if(NOT EXISTS ${DR_MP3_FILE})
    message(STATUS "Downloading dr_mp3.h...")
    file(MAKE_DIRECTORY ${DR_MP3_DIR})
    file(DOWNLOAD
            "https://raw.githubusercontent.com/mackron/dr_libs/master/dr_mp3.h"
            ${DR_MP3_FILE}
            SHOW_PROGRESS
            STATUS DOWNLOAD_STATUS
    )
    list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if(NOT STATUS_CODE EQUAL 0)
        message(FATAL_ERROR "Failed to download dr_mp3.h (status: ${DOWNLOAD_STATUS})")
    endif()
    message(STATUS "Downloaded dr_mp3.h successfully")
else()
    message(STATUS "dr_mp3.h already exists")
endif()

# --- Engine library ---
add_library(Engine SHARED
        src/Engine/GameEngine.cpp
        src/Engine/TimeManager.cpp
        src/GameObjects/GameObject.cpp
        src/GameObjects/Texture.cpp
        src/GameObjects/Component/Behaviour.cpp
        src/GameObjects/Component/SpriteRenderer.cpp
        src/GameObjects/Component/AudioComponent.cpp
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
        src/UI/HUD.cpp
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
        src/Audio/Decoder/Mp3Decoder.cpp
        src/Audio/Decoder/WavDecoder.cpp
        src/Audio/AudioSystem.cpp
        src/Audio/AudioEngine.cpp
        src/Audio/AudioLoaderFactory.cpp
        src/Audio/SoundCache.cpp
        src/Physics/PhysicsSystem.cpp
        src/Physics/PhysicsComponent.cpp
        src/Physics/Box2DFacade.cpp
        src/Physics/Material.cpp
        src/Physics/RigidBody.cpp
        src/Physics/Collider.cpp
        src/SaveLoad/BaseMemento.cpp
        src/SaveLoad/JsonSaveFormat.cpp
        src/SaveLoad/SaveCaretaker.cpp
        src/SaveLoad/SaveLoadSystem.cpp
)

target_precompile_headers(Engine
        PRIVATE
        <SDL3/SDL.h>
        <SDL3_image/SDL_image.h>
        <SDL3_ttf/SDL_ttf.h>
        <box2d/box2d.h>
)

target_include_directories(Engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(Engine
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        box2d
        nlohmann_json::nlohmann_json
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

# --- Post-build: copy shared libs to executable dir ---
add_custom_command(TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
        $<TARGET_FILE:box2d>
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
        tests/Audio/AudioSystemTest.cpp
        tests/Audio/AudioEngineTest.cpp
        tests/Audio/SoundCacheTest.cpp
        tests/Audio/WavDecoderTest.cpp
        tests/Audio/Mp3DecoderTest.cpp
        tests/Audio/AudioLoaderFactoryTest.cpp
        tests/Events/EventManagerTests.cpp
        tests/Events/EventRegistryTests.cpp
        tests/GameObjects/GameObjectTesten.cpp
        tests/GameObjects/TextureTests.cpp
        tests/GameObjects/Spritesheets/AnimatorTests.cpp
        tests/GameObjects/Spritesheets/SpritesheetTests.cpp
        tests/Input/InputTests.cpp
        tests/UI/ButtonTests.cpp
        tests/UI/FontTests.cpp
        tests/UI/TextTests.cpp
        tests/UI/HUDTests.cpp
        tests/Scenes/SceneTests.cpp
        tests/Scenes/CameraTests.cpp
        tests/Scenes/TestHelper.h
        tests/Animation/AnimationTests.cpp
        tests/Animation/KeyframeTests.cpp
        tests/Animation/Converter/BounceConverterTests.cpp
        tests/Animation/Converter/ElasticConverterTests.cpp
        tests/Engine/GameEngineTests.cpp
        tests/Engine/TimeManagerTests.cpp
        tests/GameObjects/components/SpriteRenderer.cpp
        tests/AI/system/AiSystemTests.cpp
        tests/AI/transitions/LambdaTransitionsTests.cpp
        tests/GameObjects/components/BehaviorTests.cpp
        tests/GameObjects/components/AudioComponentTests.cpp
        tests/UI/ColorTests.cpp
        tests/Scenes/SceneSystemTests.cpp
        tests/SaveLoad/SaveLoadSystemTests.cpp
        tests/SaveLoad/BaseMementoTests.cpp
        tests/Physics/BodyTypeTests.cpp
        tests/Physics/Box2DFacadeTests.cpp
        tests/Physics/ColliderTests.cpp
        tests/Physics/CollisionDataTests.cpp
        tests/Physics/MaterialTests.cpp
        tests/Physics/RigidBodyTests.cpp
        tests/Physics/PhysicsComponentTests.cpp
        tests/Physics/PhysicsSystemTests.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif ()

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
        $<TARGET_FILE:box2d>
        $<TARGET_FILE_DIR:EngineTests>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Catch2::Catch2>
        $<TARGET_FILE:Catch2::Catch2WithMain>
        $<TARGET_FILE_DIR:EngineTests>

        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:EngineTests>/test_resources
)

# Windows-only: Copy MinGW runtime DLLs
if (WIN32 AND MINGW)
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    add_custom_command(TARGET EngineTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:EngineTests>
    )
endif ()

include(CTest)
if (DEFINED Catch2_SOURCE_DIR)
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(EngineTests
            DISCOVERY_MODE PRE_TEST
            REPORTER console
    )
endif()
