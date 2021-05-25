enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(googletest)
add_subdirectory(test_inputhandler)
add_subdirectory(test_utils)

#source_group("test1" FILES test1.cpp)
#source_group("test2" FILES test2.cpp)