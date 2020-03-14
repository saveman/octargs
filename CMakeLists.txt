cmake_minimum_required(VERSION 3.13)

option(BUILD_EXAMPLES  "Build the examples"       ON)
option(ENABLE_COVERAGE "Enable coverage analysis" ON)

if("${RELEASE_VERSION}" STREQUAL "")
    set(RELEASE_VERSION "0.0.0")
endif()

project(octargs_main
    VERSION ${RELEASE_VERSION}
    DESCRIPTION "C++ command line argument parsing library"
    HOMEPAGE_URL "https://github.com/saveman/octargs"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CTest)
include(GNUInstallDirs)

if (ENABLE_COVERAGE AND NOT WIN32)
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
        EXCLUDE                                         # Patterns to exclude (can be relative
                                                        # to BASE_DIRECTORY, with CMake 3.4+)
            "3rdparty/*"
    )
endif()

add_subdirectory(doxygen)
add_subdirectory(include)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(3rdparty/googletest)
    add_subdirectory(tests)
endif()

#------------- PACKAGING

include(OctArgsPackages)
