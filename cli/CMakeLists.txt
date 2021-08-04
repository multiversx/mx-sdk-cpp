include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(inputhandler)

add_subdirectory(inputhandler)

set(HEADER_FILES cli_handler.h
                 cli_utility.h 
                 secretkeyprovider.h
                 config/cliconfig.h)
                 
set(SOURCE_FILES main.cpp
                 cli_handler.cpp
                 cli_utility.cpp
                 secretkeyprovider.cpp
                 config/cliconfig.cpp )
                 
add_executable(erdcpp ${HEADER_FILES}
                      ${SOURCE_FILES})

target_link_libraries(erdcpp PUBLIC inputhandler)
target_link_libraries(erdcpp PUBLIC src)

