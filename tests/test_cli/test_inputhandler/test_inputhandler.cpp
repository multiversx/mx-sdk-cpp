#include "gtest/gtest.h"
#include "inputhandler/ext.h"
#include "filehandler/pemreader.h"
#include "utils/ext.h"
#include "wrappers/cryptosignwrapper.h"
#include <sodium.h>
#include <fstream>

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

TEST(JsonFileHandler, writeOutputFile)
{
    std::map<uint32_t, std::string> input;

    input[ARGS_TX_IDX_NONCE] = "5";
    input[ARGS_TX_IDX_VALUE] = "10000000000000000000";
    input[ARGS_TX_IDX_RECEIVER] = "erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt";
    input[ARGS_TX_IDX_GAS_PRICE] = "1000000000";
    input[ARGS_TX_IDX_GAS_LIMIT] = "50000";
    input[ARGS_TX_IDX_DATA] = "test";
    input[ARGS_TX_IDX_CHAIN_ID] = "T";
    input[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//..//testData//keysValid1.pem";
    input[ARGS_TX_IDX_JSON_OUT_FILE] = "..//..//testData//outputJson.json";

    ih::wrapper::PemHandlerInputWrapper const pemWrapper(input);
    ih::wrapper::TransactionInputWrapper const transactionWrapper(input);

    ih::PemFileReader pemHandler(pemWrapper.getPemFilePath());
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


