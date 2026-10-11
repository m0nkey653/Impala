add_executable(dzone src/main.cpp)

# Linking the engine pulls in SDL3 transitively
target_link_libraries(dzone PRIVATE Impala::Engine)
