#include "gtest/gtest.h"
#include "inputhandler/ext.h"
#include "filehandler/pemhandler.h"
#include "utils/ext.h"
#include <sodium.h>

TEST(ArgHandler, getRequestedCmd_getRequestType_noArgument_expectInvalid)
{
    int const argc = 1;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_randomArgs_expectInvalid)
{
    int const argc = 3;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "dsa";
    argv[2] = (char *) "";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_help_expectHelp)
{
    int const argc = 2;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "help";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::help);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_pem_load_someFile_expectLoadPemFile)
{
    int const argc = 4;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "pem";
    argv[2] = (char *) "load";
    argv[3] = (char *) "--file=someFile";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::loadPemFile);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_pem_load_withoutFile_expectInvalid)
{
    int const argc = 3;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "pem";
    argv[2] = (char *) "help";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_pem_withoutSubArgument_expectInvalid)
{
    int const argc = 2;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "pem";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_transaction_new_noData_expectCreateTransaction)
{
    int const argc = 10;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=\"31\"";
    argv[5] = (char *) "--receiver=\"da\"";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::createSignedTransactionWithPemFile);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_transaction_new_withData_expectCreateTransaction)
{
    int const argc = 11;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=\"31\"";
    argv[5] = (char *) "--receiver=\"da\"";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::createSignedTransactionWithPemFile);
}


TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidNonce_expectErrorNonce)
{
    int const argc = 11;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3f";
    argv[4] = (char *) "--value=\"31\"";
    argv[5] = (char *) "--receiver=\"da\"";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_NONCE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidValue_expectErrorValue)
{
    int const argc = 11;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=";
    argv[5] = (char *) "--receiver=\"da\"";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_VALUE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidReceiver_expectErrorReceiver)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_RECEIVER);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidGasPrice_expectErrorGasPrice)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=abc";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_GAS_PRICE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidGasLimit_expectErrorGasLimit)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=abc";
    argv[8] = (char *) "--pem=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_GAS_LIMIT);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidPem_expectErrorPem)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=700";
    argv[8] = (char *) "--pem=";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_PEM_INPUT_FILE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidJson_expectErrorJson)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=700";
    argv[8] = (char *) "--pem=someFile";
    argv[9] = (char *) "--outfile=";
    argv[10] = (char *) "--data=\"dd\"";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_JSON_OUT_FILE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidData_expectErrorData)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=700";
    argv[8] = (char *) "--pem=someFile";
    argv[9] = (char *) "--outfile=otherFile";
    argv[10] = (char *) "--data=";

    ih::ArgHandler argHandler(argc, argv);

    EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_DATA);
}

//class ErrorReportParametrized : public ::testing::TestWithParam<errorCode> {};
//
//INSTANTIATE_TEST_CASE_P(
//  AllErrors,
//  ErrorReportParametrized,
//  ::testing::Values(ERROR_PEM_INPUT_FILE, ERROR_VALUE, ERROR_NONCE, ERROR_GAS_PRICE, ERROR_RECEIVER,
//                    ERROR_GAS_LIMIT, ERROR_PEM_INPUT_FILE, ERROR_JSON_OUT_FILE, ERROR_DATA ,
//                    ERROR_SODIUM_INIT, 111U));
//
//TEST_P(ErrorReportParametrized, reportError_differentErrors)
//{
//  int const argc = 1;
//  char* argv[argc];
//
//  argv[0] = "ERDProject.exe";
//
//  errorCode const& currParam = GetParam();
//
//  reportError(currParam);
//}


TEST(JsonFileHandler, writeOutputFile)
{
    std::map<uint32_t, std::string> input;

    input[ARGS_TX_IDX_NONCE] = "5";
    input[ARGS_TX_IDX_VALUE] = "10000000000000000000";
    input[ARGS_TX_IDX_RECEIVER] = "erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt";
    input[ARGS_TX_IDX_GAS_PRICE] = "1000000000";
    input[ARGS_TX_IDX_GAS_LIMIT] = "50000";
    input[ARGS_TX_IDX_DATA] = "test";
    input[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keys.pem";
    input[ARGS_TX_IDX_JSON_OUT_FILE] = "..//testData//outputJson.json";

    ih::wrapper::PemHandlerInputWrapper const pemWrapper(input);
    ih::wrapper::TransactionInputWrapper const transactionWrapper(input);

    ih::PemFileReader pemHandler(pemWrapper.getPemFilePath());
    ih::TransactionInputHandler transactionHandler(transactionWrapper);

    Transaction transaction(transactionWrapper.getNonce(), transactionWrapper.getValue(),
                            transactionWrapper.getReceiver(), pemHandler.getAddress(),
                            transactionWrapper.getGasPrice(), transactionWrapper.getGasLimit(),
                            transactionWrapper.getData(), transactionWrapper.getChainId(),
                            transactionWrapper.getVersion());
    Signer signer(pemHandler.getPrivateKey());
    transaction.applySignature(signer);
    transactionHandler.writeTransactionToJsonFile(transaction);
}

class PemFileReaderConstructorFixture : public ::testing::Test
{
public:
    template <typename T>
    void expectException(std::string const &filePath, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             ih::PemFileReader pemHandler(filePath);
                         }
                         catch(const T &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, T );
    }

};

TEST_F(PemFileReaderConstructorFixture, validFile)
{
    EXPECT_NO_THROW(ih::PemFileReader pemHandler("..//testData//keys.pem"));
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_NotEnoughBytes)
{
    expectException<std::length_error>("..//testData//keysNotEnoughBytes.pem",ERROR_MSG_KEY_BYTES_SIZE);
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_invalidFileExtension)
{
    expectException<std::invalid_argument>("..//testData//keys.pme",ERROR_MSG_FILE_EXTENSION_INVALID);
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_emptyFile)
{
    expectException<std::invalid_argument>("..//testData//keysEmptyFile.pem",ERROR_MSG_FILE_EMPTY);
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_notExisting)
{
    expectException<std::invalid_argument>("..//testData//thisFileDoesNotExist.pem",ERROR_MSG_FILE_DOES_NOT_EXIST);
}


TEST(PemFileReader, getPublicPrivateKeys_expectSameResultFrom_libsodium)
{
    ih::PemFileReader pemHandler("..//testData//keys.pem");

    unsigned char pemPk[crypto_sign_PUBLICKEYBYTES];
    unsigned char pemSk[crypto_sign_SECRETKEYBYTES];
    unsigned char pemSeed[crypto_sign_SEEDBYTES];

    unsigned char sodiumPk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sodiumSk[crypto_sign_SECRETKEYBYTES];


    bytes seedBytes = pemHandler.getSeed();
    bytes skBytes = pemHandler.getPrivateKey();
    bytes pkBytes = pemHandler.getAddress().getPublicKey();

    std::copy(seedBytes.begin(), seedBytes.end(), pemSeed);
    std::copy(skBytes.begin(), skBytes.end(), pemSk);
    std::copy(pkBytes.begin(), pkBytes.end(), pemPk);


    crypto_sign_seed_keypair(sodiumPk, sodiumSk, pemSeed);

    for (int currByte = 0; currByte < crypto_sign_PUBLICKEYBYTES; currByte++)
    {
        EXPECT_EQ(sodiumPk[currByte], pemPk[currByte]);
    }

    for (int currByte = 0; currByte < crypto_sign_SECRETKEYBYTES; currByte++)
    {
        EXPECT_EQ(sodiumSk[currByte], pemSk[currByte]);
    }
}

TEST(PemFileReader, getSegwitAddress)
{
    ih::PemFileReader pemHandler("..//testData//keys.pem");

    std::string pemAddress = pemHandler.getAddress().getBech32Address();
    std::string expectedAdr = "erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4";

    EXPECT_EQ(pemAddress, expectedAdr);
}
