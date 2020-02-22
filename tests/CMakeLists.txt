cmake_minimum_required(VERSION 3.13)

project(octargs-tests)

find_package(CppUnit REQUIRED)

function(add_cppunit_test)
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
            runner.cpp
    )
    target_include_directories(${ADD_TEST_ARGS_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${ADD_TEST_ARGS_NAME}
        PRIVATE
            CppUnit::CppUnit
    )
    target_compile_features(${ADD_TEST_ARGS_NAME} PUBLIC cxx_std_11)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${ADD_TEST_ARGS_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()

    add_test(NAME ${ADD_TEST_ARGS_NAME} COMMAND $<TARGET_FILE:${ADD_TEST_ARGS_NAME}>)
endfunction()

function(add_cppunit_test_basic)
    cmake_parse_arguments(PARSE_ARGV 0
        ADD_TEST_ARGS
        ""
        "NAME"
        ""
    )

    add_cppunit_test(NAME "${ADD_TEST_ARGS_NAME}" SOURCES "${ADD_TEST_ARGS_NAME}.cpp")
endfunction()

add_cppunit_test_basic(NAME argument_table_test)
add_cppunit_test_basic(NAME argument_test)
add_cppunit_test_basic(NAME char_utils_test)
add_cppunit_test_basic(NAME converter_test)
add_cppunit_test_basic(NAME exception_test)
add_cppunit_test_basic(NAME parser_test)
add_cppunit_test_basic(NAME parser_usage_test)
add_cppunit_test_basic(NAME positional_args_test)
add_cppunit_test_basic(NAME storage_args_test)
add_cppunit_test_basic(NAME string_utils_test)
add_cppunit_test_basic(NAME switch_args_test)
add_cppunit_test_basic(NAME valued_args_test)
add_cppunit_test_basic(NAME wchar_test)
