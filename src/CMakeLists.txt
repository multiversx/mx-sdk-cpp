add_subdirectory(Utils)
add_subdirectory(InputHandler)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC utils)
target_link_libraries(main PUBLIC inputhandler)
target_link_libraries(main LINK_PUBLIC ${LIBSODIUM_LIBRARY})


