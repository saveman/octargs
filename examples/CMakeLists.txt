cmake_minimum_required(VERSION 3.13)

if (STANDALONE_EXAMPLES_BUILD)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(OctArgs REQUIRED)
endif()

add_subdirectory(calc)
add_subdirectory(cat)
add_subdirectory(head)
