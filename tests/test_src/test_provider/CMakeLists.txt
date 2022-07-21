include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tests/test_common)

add_executable(test_data_transaction test_data_transaction.cpp)
add_executable(test_apiresponse test_apiresponse.cpp)

target_link_libraries(test_data_transaction PUBLIC gtest_main)
target_link_libraries(test_data_transaction PUBLIC src)

target_link_libraries(test_apiresponse PUBLIC gtest_main)
target_link_libraries(test_apiresponse PUBLIC src)

add_test(NAME test_data_transaction COMMAND test_data_transaction)
add_test(NAME test_apiresponse COMMAND test_apiresponse)
