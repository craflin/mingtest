cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)

project(mingtest VERSION 0.1.6)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

enable_testing()

add_subdirectory(src)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake" COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "lib/cmake/${PROJECT_NAME}")
install(EXPORT ${PROJECT_NAME}Config DESTINATION "lib/cmake/${PROJECT_NAME}" NAMESPACE ${PROJECT_NAME}::)
