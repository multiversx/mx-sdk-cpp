# elrond-sdk-erdcpp

Elrond C++ Command Line Tools and SDK for interacting with the Elrond Network (in general) and Smart Contracts (in
particular).

## 1. Installation. How to use it

### 1.1 Update submodules

Elrond C++ SDK CLI uses google test which is configured as submodule. After cloning the repository, make sure to
initialize and update the submodule:

```bash
git submodule update --init --recursive
```

### 1.2 Dependencies

Elrond C++ SDK CLI uses:
- Libsodium shared library. Make sure to [install](https://doc.libsodium.org/installation) the
latest version.

For ubuntu users:
```bash
sudo apt install libsodium-dev
```

### 1.3 Compile and build with CMake

```bash
cmake .
cmake --build .
```

### 1.4 Usage

#### 1.4.1 SDK

After building with cmake, you can use the SDK by:

- Adding **include** folder to your project. Then you only need to include one header to your project:

```c++
#include "erdsdk.h"
```

- Add generated library in src file

#### 1.4.2 CLI

To see all available command line commands:
```bash
cd cli
./erdcpp -h
```

## 2. Examples

Click [here](examples/examples.md) to see a list with provided features and usage