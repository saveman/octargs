include(FindPackageHandleStandardArgs)

if(DEFINED OCTARGS_ROOT_DIR)
    set(OCTARGS_ROOT_INCLUDE_DIR "${OCTARGS_ROOT_DIR}/include") 
endif()

find_path(OCTARGS_INCLUDE_DIR
    NAMES
        octargs/octargs.hpp
    HINTS
        ${OCTARGS_ROOT_INCLUDE_DIR}
)

set(OCTARGS_INCLUDE_DIRS ${OCTARGS_INCLUDE_DIR})

find_package_handle_standard_args(OctArgs DEFAULT_MSG
    OCTARGS_INCLUDE_DIR
)

mark_as_advanced(OCTARGS_INCLUDE_DIR)

if(OctArgs_FOUND AND NOT TARGET octargs::octargs)
    add_library(octargs::octargs INTERFACE IMPORTED)
    target_include_directories(octargs::octargs INTERFACE "${OCTARGS_INCLUDE_DIR}")
#    set_target_properties(octargs::octargs PROPERTIES
#        INTERFACE_INCLUDE_DIRECTORIES "${OCTARGS_INCLUDE_DIR}"
#    )
endif()
