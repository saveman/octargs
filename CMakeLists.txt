cmake_minimum_required(VERSION 3.13)

project(octargs)

include(CTest)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(${PROJECT_NAME}_HEADERS
    include/arguments.hpp
)

install(FILES ${${PROJECT_NAME}_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
