# 1. Features

| Feature         | Usage                                                              |
|-----------------|--------------------------------------------------------------------|
| **Address**     | Create, compute bech32 address, get public key from bech32 address |
| **Key File**    | Read pem and json + password file                                  |
| **Signer**      | Sign any string message or ERD prefixed messages                   |
| **Transaction** | Create, Sign, Serialize, Deserialize                               |
| **Proxy**       | Get account, send transaction, get ESDT balances                   |
| **ESDT**        | Issue, transfer, transfer with SC call                             |


## 1.1 Examples. SDK

### 1.1.1 Address

```c++
try
{
    Address adr1("erd1sj...");
    Address adr2(bytes{...});
    
    bytes const pubKey1 = adr1.getPublicKey();
    std::string const bech32Addr2 = adr1.getBech32Address();
    
    bytes const pubKey2 = adr2.getPublicKey();
    std::string const bech32Addr2 = adr2.getBech32Address();
    
    if (adr1 == adr2){
        // ...
    }
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

### 1.1.2 Key File (pem, json)

```c++
try
{
    PemFileReader const pem("alice.pem");
    KeyFileReader const keyFile("bob.json", "password");

    bytes const seedAlice = pem.getSeed();
    bytes const addressAlice = pem.getAddress();
    
    bytes const seedBob = pem.getSeed();
    bytes const addressBob = pem.getAddress();
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
    // Invalid bytes in file, e.g.: in pem file expected 64 bytes (32 bytes SEED + 32 bytes PUBLIC KEY)
}
```

### 1.1.3 Signer

```c++
try
{
    // Get seed from a pem file
    PemFileReader const pemReader("keys.pem");
    bytes const seed = pemHandler.getSeed();

    // Create signer and sign/verify any type of message
    Signer signer(seed);
    std::string const message = "Hello, world!";
    std::string const signature = signer.getSignature(message);
    if (!signer.verify(signature, message)){
        // Something went wrong
    }
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

### 1.1.4 Transaction

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
    
    // Deserialize transaction into current object
    transaction.deserialize("{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1...}");
    
    // Sign/verify transaction
    Signer signer(seed); 
    transaction.sign(signer);
    if (!transaction.verify()){
        // Something went wrong
    }
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

### 1.1.5 Proxy

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

### 1.1.6 ESDT transfer with func call

```c++
try
{
    // Get address from json
    KeyFileReader keyFile("alice.json", "password");
    Address const address = keyFile.getAddress();
    
    // Load account from proxy
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Account const account = proxy.getAccount(address);
    
    // Create transaction
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(account);
    transaction.m_receiver = std::make_shared<Address>("erd1qqq..."); // contract address
    transaction.m_chainID = "T";
    transaction.m_nonce = account.getNonce();
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 150000000;

    // Define function args
    SCArguments args;
    args.add("param1");
    args.add(BigUInt("400000"));
    
    // Prepare transaction for ESDT transfer with func call + arguments
    prepareTransactionForESDTTransfer(transaction, 
                                      "TOKEN-b9cba1", // token id to transfer
                                      "func",         // function to call
                                      args);          // SC args
                                      
    // Sign transaction                                 
    Signer const signer(keyFile.getSeed());
    transaction.sign(signer);
    
    // Send transaction and get tx hash
    std::string const txHash = proxy.send(transaction).hash;
    
    // Check tx status
    TransactionStatus const txStatus = proxy.getTransactionStatus(txHash);
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

## 1.2 Examples. CLI

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
