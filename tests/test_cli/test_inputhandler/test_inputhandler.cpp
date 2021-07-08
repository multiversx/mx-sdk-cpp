#include <filehandler/pemreader.h>
#include <fstream>
#include "gtest/gtest.h"
#include "inputhandler/ext.h"
#include "utils/ext.h"

template <typename T>
void EXPECT_PARSE_ERROR_MISSING_ARG(int const &argc, char *const argv[], errorMessage const &errMsg, std::string arg)
{
    EXPECT_THROW({
                     try
                     {
                         ih::ArgHandler handler;

                         handler.parse(argc, argv);
                     }
                     catch(const T &e)
                     {
                         std::string const err = e.what();
                         EXPECT_TRUE(err.find(errMsg) != std::string::npos );
                         EXPECT_TRUE(err.find(arg) != std::string::npos );
                         throw;
                     }
                 }, T );
}

TEST(ArgHandler, parse_requestType_noArgument_expectInvalid)
{
    int const argc = 1;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::invalid);
}

TEST(ArgHandler, parse_requestType_randomArgs_expectInvalid)
{
    int const argc = 3;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "dsa";
    argv[2] = (char *) "";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::invalid);
}

TEST(ArgHandler, parse_requestType_help_expectHelp)
{
    int const argc = 2;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "help";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::help);
}

TEST(ArgHandler, parse_requestType_pem_load_someFile_expectLoadPemFile)
{
    int const argc = 4;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "pem";
    argv[2] = (char *) "load";
    argv[3] = (char *) "--file=someFile";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::loadPemFile);
}

TEST(ArgHandler, parse_requestType_pem_load_withoutFile_expectInvalid)
{
    int const argc = 3;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "pem";
    argv[2] = (char *) "help";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::help);
}

TEST(ArgHandler, parse_requestType_pem_withoutSubArgument_expectInvalid)
{
    int const argc = 2;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "pem";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::invalid);
}

TEST(ArgHandler, parse_requestType_transaction_new_noData_expectCreateTransaction)
{
    int const argc = 10;
    char *argv[argc];
    argv[0] = (char *) "ERDProject.exe";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=31";
    argv[5] = (char *) "--receiver=foo";
    argv[6] = (char *) "--gas-price=4";
    argv[7] = (char *) "--gas-limit=44";
    argv[8] = (char *) "--pem=file1";
    argv[9] = (char *) "--outfile=file2";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::createSignedTransactionWithPemFile);
}

TEST(ArgHandler, parse_requestType_transaction_new_withData_expectCreateTransaction)
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

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::createSignedTransactionWithPemFile);
}


TEST(ArgHandler, parse_transaction_new_invalidNonce_expectErrorNonce)
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

    ih::ArgHandler argHandler;

    EXPECT_THROW(argHandler.parse(argc, argv),cxxopts::OptionException);
}

TEST(ArgHandler, parse_transaction_new_invalidValue_expectErrorValue)
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

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "value");
}

TEST(ArgHandler, parse_transaction_new_invalidReceiver_expectErrorReceiver)
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

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "receiver");
}

TEST(ArgHandler, parse_transaction_new_invalidGasPrice_expectErrorGasPrice)
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

    ih::ArgHandler argHandler;

    EXPECT_THROW(argHandler.parse(argc, argv),cxxopts::OptionException);
}

TEST(ArgHandler, parse_transaction_new_invalidGasLimit_expectErrorGasLimit)
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

    ih::ArgHandler argHandler;

    EXPECT_THROW(argHandler.parse(argc, argv),cxxopts::OptionException);
}

TEST(ArgHandler, parse_transaction_new_invalidPem_expectErrorPem)
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

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "pem");
}

TEST(ArgHandler, parse_transaction_new_invalidJson_expectErrorJson)
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

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "outfile");
}

TEST(ArgHandler, parse_transaction_new_invalidData_expectErrorData)
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

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "data");
}


TEST(JsonFileHandler, writeOutputFile)
{
    int const argc = 14;
    char *argv[argc];

    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=5";
    argv[4] = (char *) "--value=10000000000000000000";
    argv[5] = (char *) "--receiver=erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt";
    argv[6] = (char *) "--gas-price=1000000000";
    argv[7] = (char *) "--gas-limit=50000";
    argv[8] = (char *) "--pem=someFile";
    argv[9] = (char *) "--outfile=otherFile";
    argv[10] = (char *) "--data=test";
    argv[11] = (char *) "--chainID=T";
    argv[12] = (char *) "--pem=..//..//testData//keysValid1.pem";
    argv[13] = (char *) "--outfile=..//..//testData//outputJson.json";

    ih::ArgHandler argHandler;

    auto res = argHandler.parse(argc, argv);

    ih::wrapper::PemHandlerInputWrapper const pemWrapper(res.result);
    ih::wrapper::TransactionInputWrapper const transactionWrapper(res.result);

    PemFileReader pemHandler(pemWrapper.getPemFilePath());
    ih::JsonFile jsonFile(transactionWrapper.getOutputFile());

    Transaction transaction(transactionWrapper.getNonce(), transactionWrapper.getValue(),
                            transactionWrapper.getReceiver(), pemHandler.getAddress(),
                            transactionWrapper.getReceiverName(), transactionWrapper.getSenderName(),
                            transactionWrapper.getGasPrice(), transactionWrapper.getGasLimit(),
                            transactionWrapper.getData(),
                            transactionWrapper.getSignature(), transactionWrapper.getChainId(),
                            transactionWrapper.getVersion(), transactionWrapper.getOptions());

    Signer signer(pemHandler.getSeed());
    transaction.sign(signer);
    std::string const txSerialized = transaction.serialize();

    jsonFile.writeDataToFile(txSerialized);

    std::string writtenTx;
    std::ifstream inFile(transactionWrapper.getOutputFile());
    std::getline(inFile, writtenTx);

    std::string const expectedTxSerialized = "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}";
    EXPECT_EQ(txSerialized, expectedTxSerialized);
    EXPECT_EQ(writtenTx, expectedTxSerialized);
}

//TODO: Create CMake function to automatically run all tests
//TODO: Design CMake to automatically ->link all libraries + include all directories<- for every newly added test file
//TODO: Split tests file in headers + CPP
//TODO: Put every API inside erd namespace

