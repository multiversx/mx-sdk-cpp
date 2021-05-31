include_directories(${PROJECT_SOURCE_DIR}/src/utils)

add_executable(test_utils test_utils.cpp)

target_link_libraries(test_utils PUBLIC gtest_main)
target_link_libraries(test_utils PUBLIC utils)

add_test(NAME test_utils COMMAND test_utils)


