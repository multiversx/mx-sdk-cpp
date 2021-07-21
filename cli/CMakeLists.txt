include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(inputhandler)

add_subdirectory(inputhandler)

set(HEADER_FILES cli_handler.h
                 cli_utility.h 
                 secretkeyprovider.h
                 config/cliconfig.h)
                 
set(SOURCE_FILES main.cpp
                 cli_handler.cpp
                 secretkeyprovider.cpp
                 config/cliconfig.cpp )
                 
add_executable(erdcpp ${SOURCE_FILES}
                      ${HEADER_FILES})

target_link_libraries(erdcpp PUBLIC inputhandler)
target_link_libraries(erdcpp PUBLIC utils)
target_link_libraries(erdcpp PUBLIC src)
target_link_libraries(erdcpp PUBLIC external)
target_link_libraries(erdcpp LINK_PUBLIC ${LIBSODIUM_LIBRARY})

