add_executable(PageReplacementTests main.gtest.cpp)
target_link_libraries(PageReplacementTests gmock_main PageReplacement)
add_test(NAME example_test COMMAND PageReplacementTests)

target_link_directories(PageReplacementTests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../src)