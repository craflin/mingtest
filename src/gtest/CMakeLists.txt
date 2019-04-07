
set(sources
    gtest.cpp
    ../../include/gtest/gtest.h
)

add_library(gtest STATIC
    ${sources}
)

target_include_directories(gtest PUBLIC
    ../../include
)

source_group("" FILES ${sources})

add_subdirectory(tests)
