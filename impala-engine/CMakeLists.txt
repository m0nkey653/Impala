add_library(impala_engine STATIC
    src/impala.cpp
)
add_library(Impala::Engine ALIAS impala_engine)

# Public headers live in include/impala/ -> games use #include <impala/...>
target_include_directories(impala_engine PUBLIC include)

target_link_libraries(impala_engine PUBLIC SDL3::SDL3)
