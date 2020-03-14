cmake_minimum_required(VERSION 3.13)

find_package(Doxygen)

project(octargs-doxygen
    VERSION ${RELEASE_VERSION}
)

if (DOXYGEN_FOUND)
    set(DOXYGEN_RECURSIVE               NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED    NO)
    set(DOXYGEN_REPEAT_BRIEF            YES)
    set(DOXYGEN_ENUM_VALUES_PER_LINE    1)
    
    set(DOXYGEN_EXAMPLE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/examples")

    doxygen_add_docs(doxygen
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/octargs
    )

    add_subdirectory(examples)
endif()
