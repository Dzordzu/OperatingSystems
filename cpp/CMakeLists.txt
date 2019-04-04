cmake_minimum_required(VERSION 3.13)
project(operating-systems-cpp VERSION 1.0.0 )

#
# Include cmake scripts
#
include(cmake/GTest.cmake)
add_subdirectory(utils)
add_subdirectory(processes-management)
add_subdirectory(disk-management)

find_package(Threads)

add_executable(MAIN main.cpp)
target_link_libraries(MAIN
        PUBLIC
        ProcessesManagement
        DiskManagement
        ${CMAKE_THREAD_LIBS_INIT})
#message(Test)