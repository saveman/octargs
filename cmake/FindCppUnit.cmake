# Try to find CPPUnit
# Once done, this will define
#
# CPPUNIT_FOUND        - system has cppunit
# CPPUNIT_INCLUDE_DIRS - cppunit include directories
# CPPUNIT_LIBRARIES    - libaries needed to use cppunit

include(FindPackageHandleStandardArgs)

if (WIN32)

if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(LIB_PATH_SUFFIX "x86")
else()
    set(LIB_PATH_SUFFIX "x64")
endif()

find_path(CPPUNIT_INCLUDE_DIR
    NAMES
        cppunit/ui/text/TestRunner.h
    HINTS
        ${CPPUNIT_ROOT_DIR}
        $ENV{CPPUNIT_ROOT_DIR}
	PATH_SUFFIXES
		Include
)

find_library(CPPUNIT_LIBRARY_DEBUG
    NAMES
        cppunit
    HINTS
        ${CPPUNIT_ROOT_DIR}
        $ENV{CPPUNIT_ROOT_DIR}
	PATH_SUFFIXES
		Lib/Debug-${LIB_PATH_SUFFIX}
)

find_library(CPPUNIT_LIBRARY_RELEASE
    NAMES
        cppunit
    HINTS
        ${CPPUNIT_ROOT_DIR}
        $ENV{CPPUNIT_ROOT_DIR}
	PATH_SUFFIXES
		Lib/Release-${LIB_PATH_SUFFIX}
)

find_package_handle_standard_args(CppUnit DEFAULT_MSG
    CPPUNIT_INCLUDE_DIR
    CPPUNIT_LIBRARY_DEBUG
	CPPUNIT_LIBRARY_RELEASE
)

mark_as_advanced(CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY_DEBUG CPPUNIT_LIBRARY_RELEASE)

if(CppUnit_FOUND AND NOT TARGET CppUnit::CppUnit)
    add_library(CppUnit::CppUnit UNKNOWN IMPORTED)
    set_target_properties(CppUnit::CppUnit PROPERTIES
        IMPORTED_LOCATION_DEBUG "${CPPUNIT_LIBRARY_DEBUG}"
		IMPORTED_LOCATION_RELEASE "${CPPUNIT_LIBRARY_RELEASE}"
        INTERFACE_INCLUDE_DIRECTORIES "${CPPUNIT_INCLUDE_DIR}"
    )
endif()

else(WIN32)

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

endif(WIN32)
