cmake_minimum_required(VERSION 3.13)
project(operating-systems-cpp VERSION 1.0.0 )

#
# Include cmake scripts
#
include(cmake/GTest.cmake)

#find_package(OperatingSystems::CppUtils)

add_subdirectory(utils)
add_subdirectory(processes-management)
add_subdirectory(disk-management)
add_subdirectory(page-replacement)


add_executable(MAIN main.cpp
        Simulations.cpp
        Simulations.h
        )

target_link_libraries(MAIN
        PUBLIC
        CppUtils
        ProcessesManagement
        DiskManagement
        PageReplacement)

include(cmake/InstallScript.cmake)
#message(Test)