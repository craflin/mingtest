
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
add_library(mingtest::gtest_main ALIAS gtest_main)

target_compile_definitions(gtest_main PRIVATE
    PROJECT_VERSION="${PROJECT_VERSION}"
)

if(WIN32)
    target_include_directories(gtest_main PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    )
endif()

target_link_libraries(gtest_main PRIVATE
    gtest
)
if(MSVC)
    target_compile_options(gtest_main PRIVATE
        /Zl
    )
endif()


install(TARGETS gtest_main DESTINATION lib EXPORT ${PROJECT_NAME}Config)

source_group("" FILES ${sources})
