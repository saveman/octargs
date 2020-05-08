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
    octargs/internal/argument_base_impl.hpp
    octargs/internal/argument_group_impl.hpp
    octargs/internal/argument_handler.hpp
    octargs/internal/argument_repository.hpp
    octargs/internal/argument_type_handler.hpp
    octargs/internal/argument.hpp
    octargs/internal/char_utils.hpp
    octargs/internal/exclusive_argument_impl.hpp
    octargs/internal/function_helpers.hpp
    octargs/internal/memory.hpp
    octargs/internal/name_checker.hpp
    octargs/internal/parser_data.hpp
    octargs/internal/parser_engine.hpp
    octargs/internal/positional_argument_impl.hpp
    octargs/internal/results_data.hpp
    octargs/internal/string_utils.hpp
    octargs/internal/subparser_argument_impl.hpp
    octargs/internal/switch_argument_impl.hpp
    octargs/internal/valued_argument_impl.hpp
)

set(${PROJECT_NAME}_HEADERS
    octargs/argument_base.hpp
    octargs/argument_group.hpp
    octargs/argument_table.hpp
    octargs/converter.hpp
    octargs/dictionary.hpp
    octargs/exception.hpp
    octargs/exclusive_argument.hpp
    octargs/names.hpp
    octargs/octargs.hpp
    octargs/parser_error.hpp
    octargs/parser.hpp
    octargs/positional_argument.hpp
    octargs/results.hpp
    octargs/subparser_argument.hpp
    octargs/switch_argument.hpp
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
