cmake_minimum_required(VERSION 3.16)
project(Impala C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compile_commands.json lets clangd and clang-tidy see the real compiler flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MSVC otherwise reads source files in the system code page; we use UTF-8 everywhere
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

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

# `cmake --build build --target format` reformats every C/C++ source in the repo
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE IMPALA_FORMAT_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/impala-engine/*.cpp ${CMAKE_SOURCE_DIR}/impala-engine/*.h
        ${CMAKE_SOURCE_DIR}/games/*.cpp ${CMAKE_SOURCE_DIR}/games/*.h
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${IMPALA_FORMAT_SOURCES}
        COMMENT "Formatting sources with clang-format"
        VERBATIM
    )
endif()
