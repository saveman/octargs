cmake_minimum_required(VERSION 3.13)

if (NOT BUILTIN_EXAMPLES_BUILD)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(OctArgs REQUIRED)
endif()

add_subdirectory(calc)
add_subdirectory(cat)
add_subdirectory(getfile)
add_subdirectory(head)
add_subdirectory(sum)
add_subdirectory(win_head)
