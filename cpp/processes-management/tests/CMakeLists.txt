add_executable(Tests main.gtest.cpp)
target_link_libraries(Tests gmock_main ProcessesManagement)
add_test(NAME example_test COMMAND Tests)

target_link_directories(Tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../src)