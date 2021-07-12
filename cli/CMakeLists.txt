include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(inputhandler)

add_subdirectory(inputhandler)

set(HEADER_FILES commandhandler.h 
                 config/networkconfig.h)

add_executable(erdcpp main.cpp config/networkconfig.cpp ${HEADER_FILES})

target_link_libraries(erdcpp PUBLIC inputhandler)
target_link_libraries(erdcpp PUBLIC utils)
target_link_libraries(erdcpp PUBLIC src)
target_link_libraries(erdcpp PUBLIC external)
target_link_libraries(erdcpp LINK_PUBLIC ${LIBSODIUM_LIBRARY})

