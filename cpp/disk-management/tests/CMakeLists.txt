add_executable(DiskManagementTests main.gtest.cpp)
target_link_libraries(DiskManagementTests gmock_main DiskManagement)
add_test(NAME example_test COMMAND DiskManagement)

target_link_directories(DiskManagement
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../src)