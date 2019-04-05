add_executable(ProcessManagementTests main.gtest.cpp)
target_link_libraries(ProcessManagementTests gmock_main ProcessesManagement)
add_test(NAME example_test COMMAND Tests)

target_link_directories(ProcessManagementTests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../src)