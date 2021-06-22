include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_wrappers test_wrappers.cpp)

target_link_libraries(test_wrappers PUBLIC gtest_main)
target_link_libraries(test_wrappers PUBLIC src)

add_test(NAME test_wrappers COMMAND test_wrappers)
