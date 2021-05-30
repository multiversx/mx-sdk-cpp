include_directories(../src)

#include_directories(${PROJECT_SOURCE_DIR}/src/utils)
include_directories(inputhandler)

add_subdirectory(inputhandler)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC inputhandler)
target_link_libraries(main PUBLIC utils)
target_link_libraries(main PUBLIC src)
target_link_libraries(main PUBLIC external)
target_link_libraries(main LINK_PUBLIC ${LIBSODIUM_LIBRARY})
