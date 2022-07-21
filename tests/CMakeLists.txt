enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(googletest)
add_subdirectory(test_cli)
add_subdirectory(test_src)
add_subdirectory(test_integration)
