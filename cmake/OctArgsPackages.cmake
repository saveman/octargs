if (WIN32)
    set(CPACK_GENERATOR         "NSIS")
    set(CPACK_SOURCE_GENERATOR  "ZIP")
else()
    set(CPACK_GENERATOR         "DEB;TGZ")
    set(CPACK_SOURCE_GENERATOR  "TGZ")
endif()

set(CPACK_SOURCE_IGNORE_FILES
    "/.git/"
    "/.settings/"
    "/_.*/"
    "/.cproject$"
    "/.gitignore$"
    "/.project$"
    "/.travis.yml$"
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

set(LIB_NAME "octargs")
set(CPACK_PACKAGE_NAME              "lib${LIB_NAME}")
set(CPACK_PACKAGE_VERSION           "${RELEASE_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION       "C++ command line argument parsing library")
set(CPACK_PACKAGE_VENDOR            "OCTAEDR")
set(CPACK_PACKAGE_CONTACT           "Krzysztof Kapuscik <contact@octaedr.info>")
set(CPACK_RESOURCE_FILE_LICENSE     "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README      "${CMAKE_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_PACKAGE_FILE_NAME  "${CPACK_PACKAGE_NAME}-src_${CPACK_PACKAGE_VERSION}-all")

set(CPACK_ARCHIVE_COMPONENT_INSTALL     "ON")
set(CPACK_ARCHIVE_DEV_FILE_NAME         "${CPACK_PACKAGE_NAME}-dev_${CPACK_PACKAGE_VERSION}-all")
set(CPACK_ARCHIVE_EXAMPLES_FILE_NAME    "${CPACK_PACKAGE_NAME}-examples_${CPACK_PACKAGE_VERSION}-${PACKAGE_ARCHITECTURE}")

set(CPACK_DEB_COMPONENT_INSTALL             "ON")
set(CPACK_DEBIAN_DEV_PACKAGE_NAME               "${CPACK_PACKAGE_NAME}-dev")
set(CPACK_DEBIAN_PACKAGE_SECTION            "devel")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE       "${PACKAGE_ARCHITECTURE}")
set(CPACK_DEBIAN_DEV_FILE_NAME              "${CPACK_PACKAGE_NAME}-dev_${CPACK_PACKAGE_VERSION}-all.deb")
set(CPACK_DEBIAN_DEV_PACKAGE_ARCHITECTURE   "all")
set(CPACK_DEBIAN_EXAMPLES_FILE_NAME         "${CPACK_PACKAGE_NAME}-examples_${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")

set(CPACK_RPM_DEV_PACKAGE_NAME                  "${CPACK_PACKAGE_NAME}-devel")

set(PKG_CONFIG_NAME "${LIB_NAME}")
set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PKG_CONFIG_NAME}.pc")
configure_file("${PackagingTemplatesDir}/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
install(FILES "${PKG_CONFIG_FILE_NAME}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR_ARCHIND}/pkgconfig"
	COMPONENT dev
)

include(CPack)

cpack_add_component(dev
    DISPLAY_NAME "Development resources"
    DESCRIPTION "Files needed to develop applications using octargs library"
)

cpack_add_component(examples
    DISPLAY_NAME "Examples"
    DESCRIPTION "Example application using the octargs library"
)
