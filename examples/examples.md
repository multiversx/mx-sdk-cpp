# 1. Features

| Feature         | Usage                                                              |
|-----------------|--------------------------------------------------------------------|
| **Address**     | Create, compute bech32 address, get public key from bech32 address |
| **Key File**    | Read pem and json + password file                                  |
| **Signer**      | Sign any string message or ERD prefixed messages                   |
| **Transaction** | Create, Sign, Serialize, Deserialize                               |
| **Proxy**       | Get account, send transaction, get ESDT balances                   |
| **ESDT**        | Issue, transfer, transfer with SC call                             |
| **Builders**    | Build ESDT/ESDTNFT/MultiESDTNFT Transfer payload/transaction       |

# 2. Examples

## 2.1 Examples. SDK

### 2.1.1 Address

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

### 2.1.2 Key File (pem, json)

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

### 2.1.3 Signer

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

### 2.1.4 Transaction

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
    
    // Sign/verify signed transaction
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

### 2.1.5 Proxy

```c++
try
{
    ProxyProvider proxy("https://testnet-gateway.multiversx.com");
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

### 2.1.6 Token payments

```c++
// Fungible tokens
std::string identifierESDT = "ESDT-c76f1f";
uint32_t numDecimalsESDT = 3;
TokenPayment fungible1 = TokenPayment::fungibleFromAmount(identifierESDT, "4.11", numDecimalsESDT);
TokenPayment fungible2 = TokenPayment::fungibleFromBigUInt(identifierESDT, BigUInt("411"), numDecimalsESDT);

// Semi-fungible tokens
std::string identifierSFT = "SFT-c76f1f";
uint64_t nonceSFT = 4;
BigUInt quantity(50);
TokenPayment sft = TokenPayment::semiFungible(identifierSFT, nonceSFT, quantity);

// Non-fungible tokens
std::string identifierNFT = "NFT-c76f1f";
uint64_t nonceNFT = 5;
TokenPayment nft = TokenPayment::nonFungible(identifierNFT, nonceNFT);

// Meta ESDT tokens
std::string identifierMetaESDT = "META-c76f1f";
uint64_t nonceMetaESDT = 5;
uint32_t numDecimalsMetaESDT = 2;
TokenPayment metaESDT1 = TokenPayment::metaESDTFromAmount(identifierMetaESDT, nonceMetaESDT, "101.3", numDecimalsMetaESDT);
TokenPayment metaESDT2 = TokenPayment::metaESDTFromBigUInt(identifierMetaESDT, nonceMetaESDT, BigUInt(10130), numDecimalsMetaESDT);

std::cout << fungible1.toString(); // will output 4110
std::cout << fungible1.toPrettyString(); // will output 4.110 ESDT-c76f1f
```

### 2.1.7 Payload builders

```c++
// Single ESDT
TokenPayment payment = TokenPayment::fungibleFromAmount("COUNTER-8b028f", "100.00", 0);
std::string data = ESDTTransferPayloadBuilder()
        .setPayment(payment)
        .build();

// Single NFT
TokenPayment payment = TokenPayment::nonFungible("ERDCPP-38f249", 1);
std::string data = ESDTNFTTransferPayloadBuilder()
        .setPayment(payment)
        .setDestination(Address("erd1..."))
        .build();

// Single SFT
TokenPayment payment = TokenPayment::semiFungible("SEMI-9efd0f", 1, BigUInt(5));
std::string data = ESDTNFTTransferPayloadBuilder()
        .setPayment(payment)
        .setDestination(Address("erd1..."))
        .build();

// Multi ESDT/NFT
TokenPayment paymentOne = TokenPayment::nonFungible("ERDCPP-38f249", 1);
TokenPayment paymentTwo = TokenPayment::fungibleFromAmount("BAR-c80d29", "10.00", 18);
std::string data = MultiESDTNFTTransferPayloadBuilder()
        .setPayments({paymentOne, paymentTwo})
        .setDestination(Address("erd1..."))
        .build();
```

### 2.1.8 Transaction builders

```c++
try
{
    // Create a transaction factory to build different transaction types
    ProxyProvider proxy("https://gateway.multiversx.com");
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    TransactionFactory transactionFactory(networkConfig);
    
    // -> Create EGLD unsigned transaction
    Transaction transaction1 = transactionFactory.createEGLDTransfer(
                    2,                    // Nonce
                    BigUInt(10000000000), // Value
                    Address("erd1..."),   // Sender's address
                    Address("erd1..."),   // Receiver's address
                    1000000000,           // Gas price
                    "hello")              // Data
            ->withVersion(2)
            .withOptions(1)
            .build();                     // Build unsigned transaction with version = 2
    
    // -> Create ESDT transaction with SC call
    SCArguments args;
    args.add(Address("erd1..."));
    args.add("foo");
    args.add(BigUInt(100));
    ContractCall contractCall("swap", args);
    
    TokenPayment mexTokens = TokenPayment::fungibleFromAmount("MEX-455c57", // Token ID
                                                              "100",        // Amount = 100 MEX
                                                              18);          // Num of token decimals
    Transaction transaction2 = transactionFactory.createESDTTransfer(
                    mexTokens,            // Token to transfer
                    3,                    // Nonce
                    myAddress,            // Sender's address
                    Address("erd1..."),   // Receiver's address
                    1000000000)           // Gas Price
            ->withContractCall(contractCall)
            .buildSigned(mySeed);         // Build signed tx with SC call
            
     // -> Create ESDTNFT transaction       
    TokenPayment nft = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    Transaction transaction3 = transactionFactory.createESDTNFTTransfer(
                    nft,                  // Token (esdt/nft)
                    4,                    // Nonce
                    Address("erd1..."),   // Sender's address
                    Address("erd1..."),   // Destination address
                    1000000000)           // Gas price
            ->buildSigned(mySeed);
    
    // -> Create MultiESDTNFT transaction
    TokenPayment token1 = TokenPayment::nonFungible("ERDCPP-38f249", 3);
    TokenPayment token2 = TokenPayment::fungibleFromAmount("MEX-455c57", "123", 18);
    Transaction transaction4 = transactionFactory.createMultiESDTNFTTransfer(
                    {token1, token2},   // Tokens(esdt/nft/sft/metaESDT)
                    5,                  // Nonce
                    Address("erd1..."), // Sender's address
                    Address("erd1..."), // Destination address
                    1000000000)         // Gas price
            ->buildSigned(mySeed);
    
    // -> Create issue ESDT transaction
    Transaction transaction5 = transactionFactory.createESDTIssue(
                    6,                  // Nonce
                    Address("erd1..."), // Sender's address
                    1000000000,         // Gas price
                    "Alice",            // Token's name
                    "ALC",              // Token's ticker
                    BigUInt(444000),    // Initial supply
                    6,                  // Num of decimals
                    ESDTProperties{     // ESDT Properties
                            .canFreeze = true,
                            .canWipe = true,
                            .canMint = true})
            ->buildSigned(mySeed);
}
catch (...)
{
    // ...
}
```

## 2.2 Examples. CLI

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

Set network
[command]: network
[subcommand]: set
Usage:
  erdcpp network set [OPTION...]

 set options:
      --config arg  Set network config used to interact with ERDCPP CLI. 
                    Valid: mainnet, testnet, devnet, local (not case 
                    sensitive)
```
