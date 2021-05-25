add_executable(test_inputhandler test_inputhandler.cpp)
#add_executable(test2 test2.cpp)

target_link_libraries (test_inputhandler PUBLIC gtest_main)
target_link_libraries (test_inputhandler PUBLIC utils)
target_link_libraries (test_inputhandler PUBLIC inputhandler)
target_link_libraries (test_inputhandler LINK_PUBLIC ${LIBSODIUM_LIBRARY})

add_test(NAME test_inputhandler COMMAND test_inputhandler)
#add_test(NAME t2 COMMAND test2)

