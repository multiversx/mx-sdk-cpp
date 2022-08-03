include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tests/test_common)

add_executable(test_proxyprovider test_proxyprovider.cpp)

target_link_libraries(test_proxyprovider PUBLIC gtest_main)
target_link_libraries(test_proxyprovider PUBLIC src)

add_test(NAME test_proxyprovider COMMAND test_proxyprovider)
