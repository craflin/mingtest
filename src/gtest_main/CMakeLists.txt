
set(sources
    main.cpp
)

add_library(gtest_main STATIC
    ${sources}
)

target_link_libraries(gtest_main PRIVATE
    gtest
)

source_group("" FILES ${sources})
