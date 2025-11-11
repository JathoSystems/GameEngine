cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

# C++ standaard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Voeg toe na project()
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
endif()

# Build type default (indien niet gezet)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler optimalisaties
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Release optimalisaties
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    # Debug opties
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()

# === SDL3 Configuratie ===
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL test library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL tests" FORCE)

# SDL3_image configuratie
set(SDL3IMAGE_VENDORED ON CACHE BOOL "Use vendored libraries" FORCE)
set(SDL3IMAGE_INSTALL OFF CACHE BOOL "Install SDL3_image" FORCE)
set(SDL3IMAGE_SAMPLES OFF CACHE BOOL "Build SDL3_image samples" FORCE)

# Optioneel: Specifieke image formaten uitschakelen die je niet nodig hebt
# set(SDL3IMAGE_PNG ON CACHE BOOL "" FORCE)
# set(SDL3IMAGE_JPG OFF CACHE BOOL "" FORCE)
# set(SDL3IMAGE_WEBP OFF CACHE BOOL "" FORCE)

# SDL3_TTF configuratie
set(SDL3TTF_VENDORED ON CACHE BOOL "Use vendored libraries" FORCE)
set(SDL3TTF_INSTALL OFF CACHE BOOL "Install SDL3_TTF" FORCE)
set(SDL3TTF_SAMPLES OFF CACHE BOOL "Build SDL3_TTF samples" FORCE)
set(SDL3TTF_FREETYPE_VENDORED ON CACHE BOOL "Use vendored FreeType" FORCE)

# SDL bibliotheken toevoegen
add_subdirectory(external/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL3_image EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL3_TTF EXCLUDE_FROM_ALL)

# === Source Files ===
# Gebruik expliciete lijsten in plaats van GLOB voor betere hercompilatie
# GLOB_RECURSE kan problemen veroorzaken bij nieuwe bestanden
file(GLOB_RECURSE SOURCES
        CONFIGURE_DEPENDS
        src/*.cpp
        src/**/*.cpp
)

file(GLOB_RECURSE GAME_SOURCES
        CONFIGURE_DEPENDS
        game/*.cpp
        game/**/*.cpp
)

file(GLOB_RECURSE HEADERS
        CONFIGURE_DEPENDS
        includes/*.h
        includes/**/*.h
)

# === Executable ===
add_executable(GameEngine
        ${SOURCES}
        ${GAME_SOURCES}
        ${HEADERS}  # Headers toevoegen voor IDE ondersteuning
)

# === Link Libraries ===
target_link_libraries(GameEngine
        PRIVATE
        SDL3::SDL3-static
        SDL3_image::SDL3_image-static
        SDL3_ttf::SDL3_ttf-static
)

# === Include Directories ===
target_include_directories(GameEngine
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# === Platform-specifieke instellingen ===
if(WIN32)
    target_compile_definitions(GameEngine PRIVATE SDL_MAIN_HANDLED)

    # Console verbergen in Release builds (optioneel)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(GameEngine PROPERTIES
                WIN32_EXECUTABLE ON
        )
    endif()

    # Windows subsysteem linker opties
    if(MSVC)
        target_link_options(GameEngine PRIVATE /SUBSYSTEM:CONSOLE)
    endif()
endif()

# === Link-time optimalisatie (LTO) ===
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

if(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "IPO/LTO enabled")
    set_target_properties(GameEngine PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE
    )
endif()

# === Precompiled Headers (optioneel, voor snellere compilatie) ===
# target_precompile_headers(GameEngine PRIVATE
#     <SDL3/SDL.h>
#     <vector>
#     <memory>
#     <string>
# )

# === Output directories ===
set_target_properties(GameEngine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# === Asset copying (optioneel) ===
# Kopieer assets naar build directory
# add_custom_command(TARGET GameEngine POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_DIR}/assets
#         ${CMAKE_BINARY_DIR}/bin/assets
#     COMMENT "Copying assets to build directory"
# )

# === Build informatie ===
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")