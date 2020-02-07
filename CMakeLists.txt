cmake_minimum_required(VERSION 3.13)

option(BUILD_EXAMPLES  "Build the examples" ON)
option(ENABLE_COVERAGE "Enable coverage analysis" ON)

project(octargs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CTest)
include(GNUInstallDirs)

if (ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()

    # Defines a target for running and collection code coverage information
    # Builds dependencies, runs the given executable and outputs reports.
    # NOTE! The executable should always have a ZERO as exit code otherwise
    # the coverage generation will not complete.
    #
    setup_target_for_coverage_gcovr_html(
        NAME ctest_coverage                             # New target name
        EXECUTABLE ctest -j ${PROCESSOR_COUNT}          # Executable in PROJECT_BINARY_DIR
        # DEPENDENCIES ${PROJECT_NAME}::${PROJECT_NAME} # Dependencies to build first
        # BASE_DIRECTORY "./"                           # Base directory for report
        #                                               #  (defaults to PROJECT_SOURCE_DIR)
        # EXCLUDE "src/dir1/*" "src/dir2/*"             # Patterns to exclude (can be relative
        #                                               #  to BASE_DIRECTORY, with CMake 3.4+)
    )
endif()

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/octargs
)

set(${PROJECT_NAME}_HEADERS
    include/octargs/internal/basic_parser_data.hpp
    include/octargs/internal/basic_results_data.hpp
    include/octargs/internal/char_utils.hpp
    include/octargs/internal/string_utils.hpp
    include/octargs/basic_argument.hpp
    include/octargs/basic_argument_table.hpp
    include/octargs/basic_parser.hpp
    include/octargs/basic_results.hpp
    include/octargs/exception.hpp
    include/octargs/octargs.hpp
    include/octargs/traits.hpp
)

install(FILES ${${PROJECT_NAME}_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/octargs
)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
