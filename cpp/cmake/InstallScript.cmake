
# Install Script
include(GNUInstallDirs)

# This dir will handle our config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/OperatingSystems)

# We will install this lib in the following dirs
install(TARGETS MAIN
        EXPORT operatingsystems-export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR} # This is for Windows
        )

# Now install config
install(EXPORT operatingsystems-export
        FILE
        OperatingSystemsConfig.cmake
        NAMESPACE
            OperatingSystems::
        DESTINATION
            ${INSTALL_CONFIGDIR}
        )

# We export our config (with namespce)
export(EXPORT operatingsystems-export
        FILE
            ${CMAKE_CURRENT_BINARY_DIR}/OperatingSystemsConfig.cmake
        NAMESPACE
            OperatingSystems::
        )

# And export whole package
export(PACKAGE OperatingSystems)

