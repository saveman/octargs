cmake_minimum_required(VERSION 3.13)

project(octargs_win_head)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        win_head.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        octargs::octargs
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT examples
)
