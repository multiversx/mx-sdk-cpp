
# elrond-sdk-erdcpp

Elrond C++ Command Line Tools and SDK for interacting with the Elrond Network (in general) and Smart Contracts (in
particular).

## 1. Installation. How to use it
```bash
./install.sh
```

This script will install all necessary dependencies for erd-cpp.

Afterwards, it will build the solution and copy all **include headers** (`/usr/include/erdcpp`) and **generated static library**(`/usr/lib/libsrc.so`).

### 1.1 SDK
To use the sdk in your project, simply include this header in your project:
```c++
#include "erdcpp/erdsdk.h"
```
Afterwards, make sure to link the `libsrc.so` library.

### CMake integration

To integrate the sdk in your `CMake` project, simply:
1. include `/usr/include/erdcpp`
2. link `libsrc.so`

Example:
```cmake
cmake_minimum_required(VERSION 3.11)
project(main)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include/erdcpp) #-> include header files

add_executable(main main.cpp)
target_link_libraries(main PUBLIC /usr/lib/libsrc.so) #-> link library

```


### 1.2 CLI

To see all available command lines:
```bash
cd cli
./erdcpp -h
```

## 2. Examples

Click [here](examples/examples.md) to see a list with provided features and usage