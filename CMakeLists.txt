cmake_minimum_required(VERSION 3.16)
project(Inferno1 VERSION 1.0 LANGUAGES CXX)

# Use modern C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 library installed on the system
find_package(SDL2 REQUIRED)

# Automatically collect all .cpp and .h files in the src/ directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
)

# Define the final executable program
add_executable(Inferno1 ${SOURCES})

# Allow #include to find files relative to the src/ directory (e.g. #include "Core/game.h")
target_include_directories(Inferno1 PRIVATE src)

# Link the SDL2 library to our executable
target_link_libraries(Inferno1 PRIVATE SDL2::SDL2)
