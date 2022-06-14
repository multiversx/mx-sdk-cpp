include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tests/test_common)

add_executable(test_filehandler
        ${PROJECT_SOURCE_DIR}/tests/test_common/test_common.h
        test_filehandler.cpp)

target_link_libraries(test_filehandler PUBLIC gtest_main)
target_link_libraries(test_filehandler PUBLIC src)

add_test(NAME test_filehandler COMMAND test_filehandler)
