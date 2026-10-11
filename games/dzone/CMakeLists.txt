add_executable(dzone src/dzone.cpp)

# Linking the engine pulls in SDL3 transitively
target_link_libraries(dzone PRIVATE Impala::Engine)
