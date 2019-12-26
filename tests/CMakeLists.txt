cmake_minimum_required(VERSION 3.13)

project(octargs-tests)

find_package(CppUnit REQUIRED)

function(add_test_helper)
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
            octargs::octargs
    )
    target_compile_features(${ADD_TEST_ARGS_NAME} PUBLIC cxx_std_11)

    add_test(NAME ${ADD_TEST_ARGS_NAME} COMMAND $<TARGET_FILE:${ADD_TEST_ARGS_NAME}>)
endfunction()

add_test_helper(NAME exception_test
    SOURCES
        exception_test.cpp
)

add_test_helper(NAME arguments_test
    SOURCES
        arguments_test.cpp
)
