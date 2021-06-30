include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_account test_account.cpp)

target_link_libraries(test_account PUBLIC gtest_main)
target_link_libraries(test_account PUBLIC src)

add_test(NAME test_account COMMAND test_account)
