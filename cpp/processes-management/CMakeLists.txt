cmake_minimum_required(VERSION 3.13)
project(processes-management VERSION 1.0.0 )

add_library(ProcessesManagement SHARED
        src/Manager.cpp
        src/Process.cpp
        )

target_include_directories(ProcessesManagement
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(ProcessesManagement
        PUBLIC
        CppUtils)

target_compile_features(ProcessesManagement PRIVATE cxx_std_14)
add_subdirectory(tests)
