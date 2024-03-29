
set(sources
    gtest.cpp
    ../../include/gtest/gtest.h
)

add_library(gtest STATIC
    ${sources}
)
add_library(mingtest::gtest ALIAS gtest)

target_include_directories(gtest PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>"
    "$<INSTALL_INTERFACE:include>"
)

if(MSVC)
    target_compile_definitions(gtest PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
endif()

install(TARGETS gtest DESTINATION lib EXPORT ${PROJECT_NAME}Config)
install(DIRECTORY ../../include DESTINATION .)

source_group("" FILES ${sources})

add_subdirectory(tests)
