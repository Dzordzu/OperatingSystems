cmake_minimum_required(VERSION 3.13)
project(disk-management VERSION 1.0.0 )

add_library(DiskManagement SHARED

        )

target_include_directories(DiskManagement
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(DiskManagement
        PUBLIC
        CppUtils)

target_compile_features(DiskManagement PRIVATE cxx_std_14)
add_subdirectory(tests)
