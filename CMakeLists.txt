cmake_minimum_required(VERSION 3.13)

option(BUILD_EXAMPLES  "Build the examples" ON)
option(ENABLE_COVERAGE "Enable coverage analysis" ON)

if("${RELEASE_VERSION}" STREQUAL "")
    set(RELEASE_VERSION "0.0.0")
endif()

project(octargs
    VERSION ${RELEASE_VERSION}
    DESCRIPTION "C++ command line argument parsing library"
    HOMEPAGE_URL "https://github.com/saveman/octargs"
)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(${PROJECT_NAME}_HEADERS_INTERNAL
    include/octargs/internal/argument_handler.hpp
    include/octargs/internal/char_utils.hpp
    include/octargs/internal/misc.hpp
    include/octargs/internal/parser_data.hpp
    include/octargs/internal/results_data.hpp
    include/octargs/internal/string_utils.hpp
)

set(${PROJECT_NAME}_HEADERS
    include/octargs/argument_base.hpp
    include/octargs/argument_table.hpp
    include/octargs/argument_type_handler.hpp
    include/octargs/argument.hpp
    include/octargs/converter.hpp
    include/octargs/exception.hpp
    include/octargs/exclusive_argument.hpp
    include/octargs/octargs.hpp
    include/octargs/parser.hpp
    include/octargs/positional_argument.hpp
    include/octargs/parser_dictionary.hpp
    include/octargs/results.hpp
    include/octargs/switch_argument.hpp
    include/octargs/usage_dictionary.hpp
    include/octargs/usage.hpp
    include/octargs/valued_argument.hpp
)

install(FILES ${${PROJECT_NAME}_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/octargs
    COMPONENT dev
)

install(FILES ${${PROJECT_NAME}_HEADERS_INTERNAL}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/octargs/internal
    COMPONENT dev
)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

#------------- PACKAGING

if (WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_SOURCE_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_SOURCE_GENERATOR "TGZ")
endif()

set(CPACK_SOURCE_IGNORE_FILES
    "/.git/"
    "/.settings/"
    "/_.*/"
    "/.cproject$"
    "/.gitignore$"
    "/.project$"
    "/.travis.yml$"
    "/build.sh$"
    "/format-all.sh$"
    "/Makefile$"
    "/prepare-release.sh$"
)

if(WIN32)
    set(PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_NAME}")
else()
    execute_process(COMMAND dpkg --print-architecture
        OUTPUT_VARIABLE PACKAGE_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "OCTAEDR")
set(CPACK_PACKAGE_CONTACT "Krzysztof Kapuscik <contact@octaedr.info>")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-src_${CPACK_PACKAGE_VERSION}-all")

set(CPACK_ARCHIVE_COMPONENT_INSTALL "ON")
set(CPACK_ARCHIVE_DEV_FILE_NAME "${CPACK_PACKAGE_NAME}-dev_${CPACK_PACKAGE_VERSION}-all")
set(CPACK_ARCHIVE_EXAMPLES_FILE_NAME "${CPACK_PACKAGE_NAME}-examples_${CPACK_PACKAGE_VERSION}-${PACKAGE_ARCHITECTURE}")

set(CPACK_DEB_COMPONENT_INSTALL "ON")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${PACKAGE_ARCHITECTURE})
set(CPACK_DEBIAN_DEV_FILE_NAME "${CPACK_PACKAGE_NAME}-dev_${CPACK_PACKAGE_VERSION}-all.deb")
set(CPACK_DEBIAN_DEV_PACKAGE_ARCHITECTURE "all")
set(CPACK_DEBIAN_EXAMPLES_FILE_NAME "${CPACK_PACKAGE_NAME}-examples_${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")

include(CPack)
cpack_add_component(dev
    DISPLAY_NAME "Development resources"
    DESCRIPTION "Files needed to develop applications using octargs library"
)

cpack_add_component(examples
    DISPLAY_NAME "Examples"
    DESCRIPTION "Example application using the octargs library"
)
