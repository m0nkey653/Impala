add_executable(golf src/main.cpp)

# Linking the engine pulls in SDL3 transitively
target_link_libraries(golf PRIVATE Impala::Engine)
