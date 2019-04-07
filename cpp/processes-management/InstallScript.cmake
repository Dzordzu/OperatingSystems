
include(GNUInstallDirs)

install(TARGETS ProcessesManagement
        EXPORT processesmanagement-export
        ARCHIVE     DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY     DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME     DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(EXPORT processesmanagement-export
        FILE
            ProcessesManagementTargets.cmake
        NAMESPACE
            OperatingSystems::
        DESTINATION
            ${CMAKE_INSTALL_LIBDR}/cmake/ProcessesManagement
        )

export(EXPORT processesmanagement-export
        FILE
            ${CMAKE_CURRENT_BINARY_DIR}/ProcessesManagementTargets.cmake
        NAMESPACE
            OperatingSystems::
        )

export(PACKAGE ProcessesManagement)