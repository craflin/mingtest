
set(sources
    main.cpp
)
if(WIN32)
    list(APPEND sources
        getopt.cpp
        getopt.h
    )
endif()

add_library(gtest_main STATIC
    ${sources}
)

target_compile_definitions(gtest_main PRIVATE
    PROJECT_VERSION="${PROJECT_VERSION}"
)

if(WIN32)
    target_include_directories(gtest_main PRIVATE
        .
    )
endif()

target_link_libraries(gtest_main PRIVATE
    gtest
)

source_group("" FILES ${sources})
