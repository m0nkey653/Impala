cmake_minimum_required(VERSION 3.16)
project(Impala C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put all executables and shared libs (incl. SDL3) in one folder so the
# games can find SDL at runtime without any post-build copying
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch SDL3 once at the root level
include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.18
    GIT_SHALLOW    TRUE
)
set(SDL_SHARED ON)
set(SDL_STATIC OFF)
FetchContent_MakeAvailable(SDL3)

add_subdirectory(impala-engine)
add_subdirectory(games/golf)
add_subdirectory(games/dzone)
