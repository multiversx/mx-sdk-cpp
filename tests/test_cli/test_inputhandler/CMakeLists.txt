include_directories(${PROJECT_SOURCE_DIR}/cli)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_inputhandler test_inputhandler.cpp)

target_link_libraries (test_inputhandler PUBLIC gtest_main)
target_link_libraries (test_inputhandler PUBLIC src)
target_link_libraries (test_inputhandler PUBLIC utils)
target_link_libraries (test_inputhandler PUBLIC inputhandler)
target_link_libraries (test_inputhandler PUBLIC external)
target_link_libraries (test_inputhandler LINK_PUBLIC ${LIBSODIUM_LIBRARY})

add_test(NAME test_inputhandler COMMAND test_inputhandler)

