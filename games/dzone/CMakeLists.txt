project(dzone VERSION 0.1.0)

add_executable(dzone src/dzone.cpp)

# Each game gets its own folder so assets don't collide
set_target_properties(dzone PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/dzone)

# Copy assets next to the exe. A custom target with no outputs runs on every
# build, so asset-only edits get picked up; _if_different skips unchanged files
add_custom_target(dzone_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:dzone>/assets
    COMMENT "Copying dzone assets"
    VERBATIM
)
add_dependencies(dzone dzone_assets)

configure_file(src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
target_include_directories(dzone PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Linking the engine pulls in SDL3 transitively
target_link_libraries(dzone PRIVATE Impala::Engine)
