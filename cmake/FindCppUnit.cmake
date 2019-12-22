# Try to find CPPUnit
# Once done, this will define
#
# CPPUNIT_FOUND        - system has cppunit
# CPPUNIT_INCLUDE_DIRS - cppunit include directories
# CPPUNIT_LIBRARIES    - libaries needed to use cppunit

include(FindPackageHandleStandardArgs)

find_path(CPPUNIT_INCLUDE_DIR
    NAMES
        cppunit/ui/text/TestRunner.h
    HINTS
        ${CPPUNIT_ROOT_DIR}
        $ENV{CPPUNIT_ROOT_DIR}
)

find_library(CPPUNIT_LIBRARY
    NAMES
        cppunit
    HINTS
        ${CPPUNIT_ROOT_DIR}
        $ENV{CPPUNIT_ROOT_DIR}
)

set(CPPUNIT_INCLUDE_DIRS ${CPPUNIT_INCLUDE_DIR})
set(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY})

find_package_handle_standard_args(CppUnit DEFAULT_MSG
    CPPUNIT_INCLUDE_DIR
    CPPUNIT_LIBRARY
)

mark_as_advanced(CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY)

if(CppUnit_FOUND AND NOT TARGET CppUnit::CppUnit)
    add_library(CppUnit::CppUnit UNKNOWN IMPORTED)
    set_target_properties(CppUnit::CppUnit PROPERTIES
        IMPORTED_LOCATION "${CPPUNIT_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${CPPUNIT_INCLUDE_DIR}"
    )
endif()
