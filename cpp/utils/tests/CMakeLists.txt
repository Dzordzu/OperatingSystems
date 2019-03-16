add_executable(CppUtilsTests main.gtest.cpp)
target_link_libraries(CppUtilsTests gmock_main CppUtils)
add_test(NAME example_test COMMAND Tests)

target_link_directories(CppUtilsTests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../src)