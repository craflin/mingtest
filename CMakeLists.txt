cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)

project(mingtest VERSION 0.1.9)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Ob0 /Od /RTC1" CACHE STRING "" FORCE) # remove /Zi
endif()

include(CDeploy)
include(CPack)

enable_testing()

add_subdirectory(src)

install(FILES LICENSE DESTINATION .)

install_deploy_export()
