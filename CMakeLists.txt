cmake_minimum_required(VERSION 3.11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(ERDCPP)

set(LIBSODIUM_INCLUDE_PATH "/usr/local/include")
set(LIBSODIUM_LIB_PATH "/usr/local/lib")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external)
include_directories(${LIBSODIUM_INCLUDE_PATH})

find_library(LIBSODIUM_LIBRARY libsodium.so.23.3.0 ${LIBSODIUM_LIB_PATH} REQUIRED)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
endif()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(cli)
add_subdirectory(external)

