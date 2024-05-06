cmake_minimum_required(VERSION 3.5)

project(mingtest VERSION 0.2.0)

if(MSVC)
    string(REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

include(CDeploy)
include(CPack)

enable_testing()

add_subdirectory(src)

install(FILES LICENSE DESTINATION .)

install_deploy_export()
