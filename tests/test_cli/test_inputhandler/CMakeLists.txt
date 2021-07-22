include_directories(${PROJECT_SOURCE_DIR}/cli)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_inputhandler
		${PROJECT_SOURCE_DIR}/cli/cli_handler.cpp
		${PROJECT_SOURCE_DIR}/cli/cli_utility.cpp
		${PROJECT_SOURCE_DIR}/cli/secretkeyprovider.cpp
		${PROJECT_SOURCE_DIR}/cli/config/cliconfig.cpp
		test_inputhandler.cpp)

target_link_libraries(test_inputhandler PUBLIC gtest_main)
target_link_libraries(test_inputhandler PUBLIC src)
target_link_libraries(test_inputhandler PUBLIC inputhandler)


add_test(NAME test_inputhandler COMMAND test_inputhandler)

