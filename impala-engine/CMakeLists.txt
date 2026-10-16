project(impala_engine VERSION 0.1.0 LANGUAGES CXX)

add_library(impala_engine STATIC
    src/entry.cpp
    src/impala.cpp
    src/assets/asset_database.cpp
    src/assets/sprite_atlas.cpp
    src/assets/sprite.cpp
    include/impala/sprites.h
    include/impala/assets.h
    include/impala/logging.h
)
add_library(Impala::Engine ALIAS impala_engine)

configure_file(src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
target_include_directories(impala_engine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Public headers live in include/impala/ -> games use #include <impala/...>
target_include_directories(impala_engine PUBLIC include)

# pugixml: XML parsing for texture atlases (engine-private, games never see it)
include(FetchContent)
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG        v1.16
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(pugixml)

target_link_libraries(impala_engine PRIVATE SDL3::SDL3-static pugixml::pugixml)

target_precompile_headers(impala_engine PRIVATE
    <string>
    <vector>
    <unordered_map>
    <memory>
    <SDL3/SDL.h>
)