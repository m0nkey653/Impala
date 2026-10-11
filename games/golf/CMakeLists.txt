add_executable(golf src/golf.cpp)

# Linking the engine pulls in SDL3 transitively
target_link_libraries(golf PRIVATE Impala::Engine)
