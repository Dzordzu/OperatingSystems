
include(GNUInstallDirs)

install(TARGETS DiskManagement
        EXPORT diskmanagement-export
        ARCHIVE     DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY     DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME     DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(EXPORT diskmanagement-export
        FILE
            DiskManagementTargets.cmake
        NAMESPACE
            OperatingSystems::
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/DiskManaagement
        )

export(EXPORT diskmanagement-export
        FILE
            ${CMAKE_CURRENT_BINARY_DIR}/DiskManagementTargets.cmake
        NAMESPACE
            OperatingSystems::
        )

export(PACKAGE DiskManagement)