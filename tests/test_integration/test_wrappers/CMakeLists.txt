include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_wrapper_client test_wrapper_client.cpp)

target_link_libraries(test_wrapper_client PUBLIC gtest_main)
target_link_libraries(test_wrapper_client PUBLIC src)

add_test(NAME test_wrapper_client COMMAND test_wrapper_client)
