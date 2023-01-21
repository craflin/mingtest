cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)

project(mingtest VERSION 0.1.8)

include(CDeploy)
include(CPack)

enable_testing()

add_subdirectory(src)

install(FILES LICENSE DESTINATION .)

install_deploy_export()
