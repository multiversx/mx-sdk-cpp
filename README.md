# elrond-sdk-erdcpp

Elrond C++ Command Line Tools and SDK for interacting with the Elrond Network (in general) and Smart Contracts (in
particular).

## 1. Instalation. How to use it

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

### 1.3 Compile and build with CMake

```bash
cmake .
cmake --build .
```

### 1.4 Usage

#### 1.4.1 SDK

After building with cmake, you can use the SDK by:

- Adding the include folder to your project. Then you only need to include one header to your project:

```c++
#include "erdsdk.h"
```

- Add the library generated in src file

# 2. Features

## 2.1 SDK

| Feature         | Usage                                                              |
|-----------------|--------------------------------------------------------------------|
| **Signer**      | Sign any string message or ERD prefixed messages                   |
| **Transaction** | Create, Sign, Serialize, Deserialize                               |
| **Key File**    | Read pem and json + password file                                  |
| **Address**     | Create, compute bech32 address, get public key from bech32 address |
| **ESDT**        | Issue, transfer, transfer with SC call                             |
| **Proxy**       | Get account, send transaction, get ESDT balances                   |


# 2. Usage. Examples

## 2.1. 

## 2.1 Pem File

```c++
try
{
    // Read data from a file
    PemFileReader const pemReader("keys.pem");

    bytes const seed = pemHandler.getSeed();
    bytes const pubKey = pemHandler.getAddress().getPublicKey();
    std::string const bech32Address = pemHandler.getAddress().getBech32Address();
}
catch (std::invalid_argument const &err)
{
    // Invalid argument, e.g.:
    // -File does not exist
    // -File is empty
    // -Invalid file extension
    // -Invalid bech32 address
}
catch (std::length_error const &err)
{
    // Invalid bytes in file, e.g.: expected  64 bytes (32 bytes SEED + 32 bytes PUBLIC KEY)
}
```

## 2.2 Signer

```c++
try
{
    // Get seed from a pem file
    PemFileReader const pemReader("keys.pem");
    bytes const seed = pemHandler.getSeed();

    // Create signer and sign any type of message
    Signer signer(seed);
    std::string const signature = signer.getSignature("Hello, world!");
}
catch (std::invalid_argument const &err)
{
    // ...
}
catch (std::length_error const &err)
{
    // ...
}
```

## 2.3 Address

```c++
try
{
    Address adr("erd1sj...");

    bytes const pubKey = adr.getPublicKey();
    std::string const bech32Addr = adr.getBech32Address();
}
catch (std::invalid_argument const &err)
{
    // Invalid bech32 address
}
catch (std::length_error const &err)
{
    // Invalid bytes array size in public key
}
```

## 2.4 Transaction

```c++
try
{
    Address sender("erd1l4...");
    Address receiver("erd1cx...");

    // Create transaction
    Transaction transaction(4,          // nonce
                           "0",         // value
                           receiver,    // receiver
                           sender,      // sender
                           nullptr,     // receiver user name
                           nullptr,     // sender user name
                           1000000000,  // gas price
                           50000,       // gas limit
                           nullptr,     // data
                           nullptr,     // signature
                           "1",         // chain id
                            1,          // version
                           nullptr);    // options
                           
    // Get serialized transaction
    std::string const txSerialized = transaction.serialize(); 
    
    // Deserialize transaction
    transaction.deserialize("{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1...}");
    
    // Sign transaction
    Signer signer(seed); 
    transaction.sign(signer);
}
catch (std::invalid_argument const &err)
{
    // Invalid argument, e.g.:
    // -Missing/invalid transaction field
    // -Invalid serialized transaction
}
catch (std::length_error const &err)
{
    // ...  
}

```

## 2.4 Proxy

```c++
try
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Address const address("erd1qq...");
    Account const account = proxy.getAccount(address);
}
catch (std::invalid_argument const &err)
{
    // ...
}
catch (std::runtime_error const &err)
{
    // ...  
}

```

## 2.5 ESDT transfer with func call

```c++
try
{
    KeyFileReader keyFile("alice.json", "password");
    Address const address = keyFile.getAddress();
    
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Account const account = proxy.getAccount(address);
    
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(account);
    transaction.m_receiver = std::make_shared<Address>("erd1qqq..."); // contract address
    transaction.m_chainID = "T";
    transaction.m_nonce = account.getNonce();
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 150000000;

    SCArguments args;
    args.add("param1");
    args.add(BigUInt("400000"));
    prepareTransactionForESDTTransfer(transaction, 
                                      "TOKEN-b9cba1", // token id to transfer
                                      "func",         // function to call
                                      args);          // SC args
                                      
    Signer const signer(keyFile.getSeed());
    transaction.sign(signer);
    
    std::string const txHash = m_proxy.send(transaction).hash;
    TransactionStatus const txStatus = m_proxy.getTransactionStatus(txHash);
    if (txStatus.isSuccessful())
    {
        // ...
    }
    
}
catch (...)
{
    // ...
}
```

## 2.2 CLI

To see all available command line commands:
```bash
cd cli
./erdcpp -h
```

```
Create signed transactions
[command]: transaction
[subcommand]: new
Usage:
  erdcpp transaction new [OPTION...]

 new options:
      --nonce arg          Transaction nonce
      --value arg          Transaction value
      --receiver arg       Receiver's address
      --receiver-name arg  Receiver's username (default: "")
      --sender-name arg    Sender's username (default: "")
      --gas-price arg      Transaction gas price
      --gas-limit arg      Transaction gas limit
      --data arg           Transaction data/payload (default: "")
      --version arg        Transaction version (default: 1)
      --options arg        Transaction options (default: 0)
      --key arg            File containing the private key (pem or keyfile)
      --password arg       Password for key file, not applicable for pem 
                           (default: "")
      --outfile arg        Json file where the output will be stored

Transfer ESDT
[command]: esdt
[subcommand]: transfer
Usage:
  erdcpp esdt transfer [OPTION...]

 transfer options:
      --nonce arg      Sender account's nonce
      --gas-price arg  Gas price
      --gas-limit arg  Gas limit
      --receiver arg   Receiver's address
      --token arg      Token id
      --value arg      ESDT value to transfer
      --function arg   Smart contract function (default: "")
      --args arg       Smart contract function args, hex encoded, comma 
                       separated, e.g.: 0a,3f6f (default: "")
      --key arg        Sender's private key (pem or keyfile)
      --password arg   Password for key file, not applicable for pem 
                       (default: "")

Issue ESDT
[command]: esdt
[subcommand]: issue
Usage:
  erdcpp esdt issue [OPTION...]

 issue options:
      --token arg         Token id
      --ticker arg        Token ticker
      --supply arg        Initial supply
      --dec arg           Number of decimals
      --gas-price arg     Gas price
      --key arg           Sender's private key (pem or keyfile)
      --password arg      Password for key file, not applicable for pem 
                          (default: "")
      --nonce arg         Sender account's nonce
      --can-freeze        [Can freeze] property (default: false)
      --can-wipe          [Can wipe] property (default: false)
      --can-pause         [Can pause] property (default: false)
      --can-mint          [Can mint] property (default: false)
      --can-burn          [Can burn] property (default: false)
      --can-change-owner  [Can change owner] property (default: false)
      --can-upgrade       [Can upgrade] property (default: false)
      --can-add-roles     [Can add special roles] property (default: false)

```
