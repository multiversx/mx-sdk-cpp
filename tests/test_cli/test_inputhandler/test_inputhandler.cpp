#include "gtest/gtest.h"
#include "inputhandler/ext.h"
#include "filehandler/pemreader.h"
#include "utils/ext.h"
#include "wrappers/cryptosignwrapper.h"
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
    ih::JsonFile jsonFile(transactionWrapper.getOutputFile());

    Transaction transaction(transactionWrapper.getNonce(), transactionWrapper.getValue(),
                            Address(transactionWrapper.getReceiver()), pemHandler.getAddress(),
                            transactionWrapper.getGasPrice(), transactionWrapper.getGasLimit(),
                            transactionWrapper.getData(), transactionWrapper.getChainId(),
                            transactionWrapper.getVersion());
    Signer signer(pemHandler.getSeed());
    transaction.applySignature(signer);
    jsonFile.writeDataToFile(transaction.getSerializedTransaction());
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


TEST(PemFileReader, getPublicSecretKeys_expectSameResultFrom_libsodiumWrapper)
{
    ih::PemFileReader pemHandler("..//testData//keys.pem");

    bytes seedBytes = pemHandler.getSeed();
    bytes skPem = pemHandler.getSecretKey();
    bytes pkPem = pemHandler.getAddress().getPublicKey();

    bytes const skWrapper = wrapper::crypto::getSecretKey(seedBytes);
    bytes const pkWrapper = wrapper::crypto::getPublicKey(seedBytes);

    EXPECT_EQ(pkWrapper, pkPem);
    EXPECT_EQ(skWrapper, skPem);
}

TEST(PemFileReader, getSegwitAddress)
{
    ih::PemFileReader pemHandler("..//testData//keys.pem");

    std::string pemAddress = pemHandler.getAddress().getBech32Address();
    std::string expectedAdr = "erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4";

    EXPECT_EQ(pemAddress, expectedAdr);
}

//TODO: This test will be moved in another unit test file, when the unit test ticket is started.
TEST(Address, getPubKey)
{
    ih::PemFileReader pemHandler("..//testData//keys.pem");

    bytes pubKeyFromPem = pemHandler.getAddress().getPublicKey();
    Address adr("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");

    EXPECT_EQ(adr.getPublicKey(),pubKeyFromPem);
}

//TODO: - Change this ugly tests in future ticket, because Transaction class is going to be completely changed.
// -Move these tests in separate file in the tests ticket
// -Add more tests for different signatures/transactions
TEST(Signer, getSignature)
{
    bytes seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    Signer signer(seed);

    std::string msg = "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}";
    std::string actualSignature = signer.getSignature(msg);
    std::string expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";

    EXPECT_EQ(util::stringToHex(actualSignature), expectedSignature);
}

TEST(Signer, getSignature2)
{
    bytes seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    Signer signer(seed);

    Address sender("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");

    Transaction transaction(0,"0",receiver,sender,1000000000,50000,"foo","1",1);
    std::string expectedSerializedTx = "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}";
    EXPECT_EQ(expectedSerializedTx,transaction.getSerializedTransaction());

    std::string expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";
    std::string actualSignature = signer.getSignature(expectedSerializedTx);
    EXPECT_EQ(util::stringToHex(actualSignature),expectedSignature);
}

