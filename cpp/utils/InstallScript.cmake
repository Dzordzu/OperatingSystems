
# Install Script
include(GNUInstallDirs)

# This dir will handle our config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/CppUtils)

# We will install this lib in the following dirs
install(TARGETS CppUtils
        EXPORT cpputils-export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        )  # This is for Windows

# This will handle our installed include dir
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Now install config
install(EXPORT cpputils-export
        FILE
        CppUtilsTargets.cmake
        NAMESPACE
        CppUtils::
        DESTINATION
        ${INSTALL_CONFIGDIR}
        )

# We export our config (with namespce)
export(EXPORT cpputils-export
        FILE
        ${CMAKE_CURRENT_BINARY_DIR}/CppUtilsTargets.cmake
        NAMESPACE
        CppUtils::
        )

# And export whole package
export(PACKAGE CppUtils)

