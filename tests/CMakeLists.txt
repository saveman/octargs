cmake_minimum_required(VERSION 3.13)

project(octargs-tests
    VERSION ${RELEASE_VERSION}
)

function(add_gtest_test)
    cmake_parse_arguments(PARSE_ARGV 0
        ADD_TEST_ARGS
        ""
        "NAME"
        "SOURCES"
    )

    add_executable(${ADD_TEST_ARGS_NAME})

    target_sources(${ADD_TEST_ARGS_NAME}
        PRIVATE
            ${ADD_TEST_ARGS_SOURCES}
    )
    target_include_directories(${ADD_TEST_ARGS_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${ADD_TEST_ARGS_NAME}
        PRIVATE
            gtest_main
    )
    target_compile_features(${ADD_TEST_ARGS_NAME} PUBLIC cxx_std_11)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${ADD_TEST_ARGS_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()

    add_test(NAME ${ADD_TEST_ARGS_NAME} COMMAND $<TARGET_FILE:${ADD_TEST_ARGS_NAME}>)
endfunction()

function(add_gtest_test_basic)
    cmake_parse_arguments(PARSE_ARGV 0
        ADD_TEST_ARGS
        ""
        "NAME"
        ""
    )

    add_gtest_test(NAME "${ADD_TEST_ARGS_NAME}" SOURCES "${ADD_TEST_ARGS_NAME}.cpp")
endfunction()

add_gtest_test_basic(NAME argument_table_test)
add_gtest_test_basic(NAME argument_test)
add_gtest_test_basic(NAME char_utils_test)
add_gtest_test_basic(NAME converter_test)
add_gtest_test_basic(NAME exclusive_args_test)
add_gtest_test_basic(NAME parser_test)
add_gtest_test_basic(NAME positional_args_test)
add_gtest_test_basic(NAME storage_args_test)
add_gtest_test_basic(NAME string_utils_test)
add_gtest_test_basic(NAME subparser_test)
add_gtest_test_basic(NAME switch_args_test)
add_gtest_test_basic(NAME usage_test)
add_gtest_test_basic(NAME valued_args_test)
add_gtest_test_basic(NAME wchar_test)
