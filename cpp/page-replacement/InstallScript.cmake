
include(GNUInstallDirs)

install(TARGETS PageReplacement
        EXPORT pagereplacement-export
        ARCHIVE     DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY     DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME     DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(EXPORT pagereplacement-export
        FILE
            PageReplacementTargets.cmake
        NAMESPACE
            OperatingSystems::
        DESTINATION
            ${CMAKE_INSTALL_LIBDR}/cmake/PageReplacement
        )

export(EXPORT pagereplacement-export
        FILE
            ${CMAKE_CURRENT_BINARY_DIR}/PageReplacementTargets.cmake
        NAMESPACE
            OperatingSystems::
        )

export(PACKAGE PageReplacement)