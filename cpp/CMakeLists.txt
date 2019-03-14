cmake_minimum_required(VERSION 3.13)
project(operating-systems-cpp VERSION 1.0.0 )

#
# Include cmake scripts
#
include(cmake/GTest.cmake)
add_subdirectory(processes-management)
find_package(Threads)

add_executable(MAIN main.cpp)
target_link_libraries(MAIN
        PUBLIC
        ProcessesManagement
        ${CMAKE_THREAD_LIBS_INIT})
#message(Test)