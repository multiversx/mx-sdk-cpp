include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_smart_contracts test_smart_contracts.cpp)

target_link_libraries(test_smart_contracts PUBLIC gtest_main)

target_link_libraries(test_smart_contracts PUBLIC src)

add_test(NAME test_smart_contracts COMMAND test_smart_contracts)
