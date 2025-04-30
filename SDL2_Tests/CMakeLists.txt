cmake_minimum_required(VERSION 3.28)
project(SDL2PerformanceTests LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# Pobranie SDL2
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.10 
    GIT_SHALLOW ON
    SYSTEM
)

# Pobranie SDL2_image
FetchContent_Declare(SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.4 
    GIT_SHALLOW ON
    SYSTEM
)

FetchContent_MakeAvailable(SDL2 SDL2_image)

add_executable(main src/main.cpp "src/StaticTest.cpp" "src/MoveTest.cpp" "src/RotateTest.cpp" "src/AlphaTest.cpp")

target_compile_features(main PRIVATE cxx_std_17)

# Łączenie bibliotek
target_link_libraries(main PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)

# Ścieżka nagłówków (jeśli masz osobny folder)
target_include_directories(main PRIVATE include)
