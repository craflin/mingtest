
if(DISABLE_TESTS)
    return()
endif()

set(sources
    test.cpp
)

add_executable(mingtest_test ${sources})

target_link_libraries(mingtest_test PRIVATE gtest)

source_group("" FILES ${sources})

add_test(NAME mingtest_test COMMAND mingtest_test)
