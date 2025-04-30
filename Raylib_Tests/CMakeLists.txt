cmake_minimum_required(VERSION 3.24...3.30)
project(raylib-game-template)

include(FetchContent)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

add_executable(main src/main.cpp "src/MoveTest.cpp" "src/RotateTest.cpp" "src/AlphaTest.cpp" "src/StaticTest.cpp")
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE raylib)
target_include_directories(main PRIVATE include)


