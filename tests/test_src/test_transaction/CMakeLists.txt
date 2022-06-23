include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_transaction test_transaction.cpp)
add_executable(test_gas_estimator test_gas_estimator.cpp)

target_link_libraries(test_transaction PUBLIC gtest_main)
target_link_libraries(test_gas_estimator PUBLIC gtest_main)

target_link_libraries(test_transaction PUBLIC src)
target_link_libraries(test_gas_estimator PUBLIC src)

add_test(NAME test_transaction COMMAND test_transaction)
add_test(NAME test_gas_estimator COMMAND test_gas_estimator)