cmake_minimum_required(VERSION 3.13)

project(octargs
    VERSION ${RELEASE_VERSION}
)

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(${PROJECT_NAME}_HEADERS_INTERNAL
    octargs/internal/argument_handler.hpp
    octargs/internal/argument_repository.hpp
    octargs/internal/char_utils.hpp
    octargs/internal/function_helpers.hpp
    octargs/internal/misc.hpp
    octargs/internal/parser_data.hpp
    octargs/internal/results_data.hpp
    octargs/internal/string_utils.hpp
)

set(${PROJECT_NAME}_HEADERS
    octargs/argument_base.hpp
    octargs/argument_group.hpp
    octargs/argument_table.hpp
    octargs/argument_type_handler.hpp
    octargs/argument.hpp
    octargs/converter.hpp
    octargs/exception.hpp
    octargs/exclusive_argument.hpp
    octargs/octargs.hpp
    octargs/parser.hpp
    octargs/parser_error.hpp
    octargs/positional_argument.hpp
    octargs/parser_dictionary.hpp
    octargs/results.hpp
    octargs/subparser_argument.hpp
    octargs/switch_argument.hpp
    octargs/usage_dictionary.hpp
    octargs/usage.hpp
    octargs/valued_argument.hpp
)

install(FILES ${${PROJECT_NAME}_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/octargs
    COMPONENT dev
)

install(FILES ${${PROJECT_NAME}_HEADERS_INTERNAL}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/octargs/internal
    COMPONENT dev
)
