project(dzone VERSION 0.1.0)

add_executable(dzone src/dzone.cpp)

configure_file(src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
target_include_directories(dzone PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Linking the engine pulls in SDL3 transitively
target_link_libraries(dzone PRIVATE Impala::Engine)
