
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

if(MSVC)
    target_compile_definitions(gtest PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
endif()

source_group("" FILES ${sources})

add_subdirectory(tests)
