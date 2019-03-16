cmake_minimum_required(VERSION 3.13)
project(CppUtils VERSION 1.0.0 )

add_library(CppUtils SHARED
        src/Logger.cpp
        )

target_include_directories(CppUtils
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_compile_features(CppUtils PRIVATE cxx_std_14)
add_subdirectory(tests)
