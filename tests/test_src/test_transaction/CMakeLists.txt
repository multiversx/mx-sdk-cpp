include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_transaction test_transaction.cpp)

target_link_libraries(test_transaction PUBLIC gtest_main)
target_link_libraries(test_transaction PUBLIC src)

add_test(NAME test_transaction COMMAND test_transaction)
