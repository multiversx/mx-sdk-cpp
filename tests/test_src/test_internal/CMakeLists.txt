include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_internal test_internal.cpp)

target_link_libraries(test_internal PUBLIC gtest_main)
target_link_libraries(test_internal PUBLIC src)

add_test(NAME test_internal COMMAND test_internal)
