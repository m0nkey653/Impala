project(golf VERSION 0.1.0)

add_executable(golf src/golf.cpp)

configure_file(src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
target_include_directories(golf PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Linking the engine pulls in SDL3 transitively
target_link_libraries(golf PRIVATE Impala::Engine)
