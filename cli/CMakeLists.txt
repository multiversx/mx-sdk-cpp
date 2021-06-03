include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(inputhandler)

add_subdirectory(inputhandler)

set(HEADER_FILES commandhandler.h)

add_executable(main main.cpp ${HEADER_FILES})

target_link_libraries(main PUBLIC inputhandler)
target_link_libraries(main PUBLIC utils)
target_link_libraries(main PUBLIC src)
target_link_libraries(main PUBLIC external)
target_link_libraries(main LINK_PUBLIC ${LIBSODIUM_LIBRARY})

