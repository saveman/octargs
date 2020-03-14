cmake_minimum_required(VERSION 3.13)

project(octargs-doxygen-examples
    VERSION ${RELEASE_VERSION}
)

function(add_doxygen_example)
    cmake_parse_arguments(PARSE_ARGV 0
        ADD_EXAMPLE_ARGS
        ""
        "NAME"
        "SOURCES"
    )

    add_executable(${ADD_EXAMPLE_ARGS_NAME} EXCLUDE_FROM_ALL)

    add_dependencies(doxygen ${ADD_EXAMPLE_ARGS_NAME})

    target_sources(${ADD_EXAMPLE_ARGS_NAME}
        PRIVATE
            ${ADD_EXAMPLE_ARGS_SOURCES}
    )
    target_include_directories(${ADD_EXAMPLE_ARGS_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${ADD_EXAMPLE_ARGS_NAME}
        PRIVATE
            octargs::octargs
    )
    target_compile_features(${ADD_EXAMPLE_ARGS_NAME} PUBLIC cxx_std_11)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${ADD_EXAMPLE_ARGS_NAME} PRIVATE -Wall -Wextra -pedantic -Wno-unused-variable)
    endif()
endfunction()

function(add_doxygen_example_basic)
    cmake_parse_arguments(PARSE_ARGV 0
        ADD_EXAMPLE_ARGS
        ""
        "NAME"
        ""
    )

    add_doxygen_example(NAME "${ADD_EXAMPLE_ARGS_NAME}" SOURCES "${ADD_EXAMPLE_ARGS_NAME}.cpp")
endfunction()

add_doxygen_example_basic(NAME code_allowed)
add_doxygen_example_basic(NAME code_defaults)
add_doxygen_example_basic(NAME code_error_handling)
add_doxygen_example_basic(NAME code_exclusive)
add_doxygen_example_basic(NAME code_multiname)
add_doxygen_example_basic(NAME code_multivalue)
add_doxygen_example_basic(NAME code_positional)
add_doxygen_example_basic(NAME code_required)
add_doxygen_example_basic(NAME code_simple)
add_doxygen_example_basic(NAME code_storing_parser)
add_doxygen_example_basic(NAME code_subparsers)
add_doxygen_example_basic(NAME code_switches)
add_doxygen_example_basic(NAME code_type_cast)
add_doxygen_example_basic(NAME code_usage)
add_doxygen_example_basic(NAME code_valued)
add_doxygen_example_basic(NAME code_value_type_custom)
add_doxygen_example_basic(NAME code_value_type_std)
