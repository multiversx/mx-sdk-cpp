
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
A quick look into: 

```c++
    // Read data from wallet
    PemFileReader myWallet("wallet.pem");
    Address myAddress = myWallet.getAddress();
    bytes mySeed = myWallet.getSeed();

    // Get updated account from proxy
    ProxyProvider proxy("https://gateway.elrond.com");
    Account myAccount = proxy.getAccount(myAddress);

    // Create a transaction factory, which lets you easily build signed/unsigned transactions
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    TransactionFactory transactionFactory(networkConfig);

    // Build ESDT transactions
    TokenPayment mexTokens =
            TokenPayment::fungibleFromAmount("MEX-455c57", // Token ID
                                             "100",        // Amount = 100 MEX
                                             18);          // Num of token decimals
    Transaction transaction = transactionFactory.createESDTTransfer(
                    mexTokens,            // Token to transfer
                    myAccount.getNonce(), // Nonce
                    myAddress,            // Sender's address
                    Address("erd1..."),   // Receiver's address
                    1000000000)           // Gas Price
            ->buildSigned(mySeed);

    // Send transaction and check its status
    std::string txHash = proxy.send(transaction);
    TransactionStatus txStatus = proxy.getTransactionStatus(txHash);
    if (txStatus.isPending())
    {
        // ...
    }
```

Click [here](examples/examples.md) to see a list with provided features and usage

## 3. External libraries
This repository uses `google test` as submodule, as well as `external` sources:

- **aes_128_ctr**: License: **The Unlicense**. From: https://github.com/kokke/tiny-AES-c
- **bech32**: License: **MIT**. Author: Pieter Wuille
- **bigint**: License: **MIT**. From: https://github.com/calccrypto/integer
- **cliparser**: License: **MIT**. From: https://github.com/jarro2783/cxxopts
- **http**: License: **MIT**. From: https://github.com/yhirose/cpp-httplib
- **json**: License: **MIT**. From: https://github.com/nlohmann/json
- **keccak**: License: **MIT**. From: https://github.com/mjosaarinen/tiny_sha3
- **toml**: License: **MIT**. From: https://github.com/skystrife/cpptoml
- **libsodium**: License: **MIT**. From: https://github.com/jedisct1/libsodium
