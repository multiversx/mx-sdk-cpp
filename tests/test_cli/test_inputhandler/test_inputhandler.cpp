#include <fstream>

#include "gtest/gtest.h"
#include "inputhandler/ext.h"
#include "cli_handler.h"
#include "utils/ext.h"
#include "test_common.h"

template <typename T>
void EXPECT_PARSE_ERROR_MISSING_ARG(int const &argc, char *const argv[], errorMessage const &errMsg, std::string const &arg)
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

TEST(ArgHandler, parse_noArgument_expectInvalid)
{
    int const argc = 1;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";

    ih::ArgHandler argHandler;
    auto const res = argHandler.parse(argc, argv);

    EXPECT_EQ(res.requestType, ih::invalid);
    EXPECT_TRUE(res.result.arguments().empty());
    EXPECT_TRUE(res.help.empty());
}

TEST(ArgHandler, parse_randomArgs_expectInvalid)
{
    int const argc = 3;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "dsa";
    argv[2] = (char *) "";

    ih::ArgHandler argHandler;
    auto const res = argHandler.parse(argc, argv);

    EXPECT_EQ(res.requestType, ih::invalid);
    EXPECT_TRUE(res.result.arguments().empty());
    EXPECT_TRUE(res.help.empty());
}

TEST(ArgHandler, parse_help_expectHelp)
{
    int const argc = 2;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "help";

    ih::ArgHandler argHandler;
    CLIOptions options;

    auto const res = argHandler.parse(argc, argv);

    EXPECT_EQ(res.requestType, ih::help);
    EXPECT_EQ(res.help, options.help());
}

TEST(ArgHandler, parse_transaction_help_expectHelp)
{
    int const argc = 3;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "help";

    ih::ArgHandler argHandler;
    CLIOptions options;

    auto const res = argHandler.parse(argc, argv);

    EXPECT_EQ(res.requestType, ih::help);
    EXPECT_EQ(res.help, options.transaction().help());
}

TEST(ArgHandler, parse_transaction_withoutSubArgument_expectInvalid)
{
    int const argc = 2;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";

    ih::ArgHandler argHandler;

    EXPECT_EQ(argHandler.parse(argc, argv).requestType, ih::invalid);
}

TEST(ArgHandler, parse_transaction_new_noData_expectCreateTransaction)
{
    int const argc = 12;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=31";
    argv[5] = (char *) "--receiver=erd1";
    argv[6] = (char *) "--gas-price=4";
    argv[7] = (char *) "--gas-limit=44";
    argv[8] = (char *) "--key=file1";
    argv[9] = (char *) "--outfile=file2";
    argv[10] = (char *) "--sender-name=Joe";
    argv[11] = (char *) "--receiver-name=Doe";

    ih::ArgHandler argHandler;
    auto const res = argHandler.parse(argc, argv);

    EXPECT_EQ(res.requestType, ih::createSignedTransaction);
    EXPECT_EQ(res.result["nonce"].as<uint64_t>(), 3U);
    EXPECT_EQ(res.result["value"].as<std::string>(), "31");
    EXPECT_EQ(res.result["receiver"].as<std::string>(), "erd1");
    EXPECT_EQ(res.result["gas-price"].as<uint64_t>(), 4U);
    EXPECT_EQ(res.result["gas-limit"].as<uint64_t>(), 44U);
    EXPECT_EQ(res.result["key"].as<std::string>(), "file1");
    EXPECT_EQ(res.result["outfile"].as<std::string>(), "file2");
    EXPECT_EQ(res.result["sender-name"].as<std::string>(), "Joe");
    EXPECT_EQ(res.result["receiver-name"].as<std::string>(), "Doe");
}

TEST(ArgHandler, parse_transaction_new_withData_expectCreateTransaction)
{
    int const argc = 11;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=31";
    argv[5] = (char *) "--receiver=erd1";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--key=test1";
    argv[9] = (char *) "--outfile=test2";
    argv[10] = (char *) "--data=testData";

    ih::ArgHandler argHandler;

    auto const res = argHandler.parse(argc, argv);

    EXPECT_EQ(res.requestType, ih::createSignedTransaction);
    EXPECT_EQ(res.result["nonce"].as<uint64_t>(), 3U);
    EXPECT_EQ(res.result["value"].as<std::string>(), "31");
    EXPECT_EQ(res.result["receiver"].as<std::string>(), "erd1");
    EXPECT_EQ(res.result["gas-price"].as<uint64_t>(), 31U);
    EXPECT_EQ(res.result["gas-limit"].as<uint64_t>(), 31U);
    EXPECT_EQ(res.result["key"].as<std::string>(), "test1");
    EXPECT_EQ(res.result["outfile"].as<std::string>(), "test2");
    EXPECT_EQ(res.result["data"].as<std::string>(), "testData");
}

TEST(ArgHandler, parse_transaction_new_invalidNonce_expectErrorNonce)
{
    int const argc = 11;
    char *argv[argc];
    argv[0] = (char *) "erdcpp";
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
    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=3";
    argv[4] = (char *) "--value=";
    argv[5] = (char *) "--receiver=\"da\"";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--key=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "value");
}

TEST(ArgHandler, parse_transaction_new_invalidReceiver_expectErrorReceiver)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=";
    argv[6] = (char *) "--gas-price=31";
    argv[7] = (char *) "--gas-limit=31";
    argv[8] = (char *) "--key=\"dd\"";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "receiver");
}

TEST(ArgHandler, parse_transaction_new_invalidGasPrice_expectErrorGasPrice)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "erdcpp";
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

    argv[0] = (char *) "erdcpp";
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

TEST(ArgHandler, parse_transaction_new_invalidPemInput_expectErrorPem)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=700";
    argv[8] = (char *) "--key=";
    argv[9] = (char *) "--outfile=\"dd\"";
    argv[10] = (char *) "--data=\"dd\"";

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "key");
}

TEST(ArgHandler, parse_transaction_new_invalidJsonOutput_expectErrorJson)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=700";
    argv[8] = (char *) "--key=someFile";
    argv[9] = (char *) "--outfile=";
    argv[10] = (char *) "--data=\"dd\"";

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "outfile");
}

TEST(ArgHandler, parse_transaction_new_invalidData_expectErrorData)
{
    int const argc = 11;
    char *argv[argc];

    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=0";
    argv[4] = (char *) "--value=10000";
    argv[5] = (char *) "--receiver=address";
    argv[6] = (char *) "--gas-price=1000";
    argv[7] = (char *) "--gas-limit=700";
    argv[8] = (char *) "--key=someFile";
    argv[9] = (char *) "--outfile=otherFile";
    argv[10] = (char *) "--data=";

    EXPECT_PARSE_ERROR_MISSING_ARG<std::invalid_argument>(argc, argv, ERROR_MSG_EMPTY_VALUE, "data");
}

// Warning: This test only passes if default config network chainID is set to Testnet
// Reason: Expected signature is for a serialized transaction which has chainID = "T"
TEST(HandleCreateSignedTransaction, withPemFile_expectCorrectWrittenTx)
{
    int const argc = 11;
    char *argv[argc];

    std::string keyFile = "--key=" + getCanonicPath("testData/keysValid1.pem");
    std::string outFile = "--outfile=" + getCanonicPath("testData/outputJson.json");

    argv[0] = (char *) "erdcpp";
    argv[1] = (char *) "transaction";
    argv[2] = (char *) "new";
    argv[3] = (char *) "--nonce=5";
    argv[4] = (char *) "--value=10000000000000000000";
    argv[5] = (char *) "--receiver=erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt";
    argv[6] = (char *) "--gas-price=1000000000";
    argv[7] = (char *) "--gas-limit=50000";
    argv[8] = (char *) "--data=test";
    argv[9] = (char *) keyFile.data();
    argv[10] = (char *) outFile.data();

    ih::ArgHandler argHandler;
    auto const res = argHandler.parse(argc, argv).result;

    cli::handleCreateSignedTransaction(res);

    std::string writtenTx;
    std::ifstream inFile(res["outfile"].as<std::string>());
    std::getline(inFile, writtenTx);

    EXPECT_EQ(writtenTx, "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}");
}

//TODO: Create CMake function to automatically run all tests
//TODO: Design CMake to automatically ->link all libraries + include all directories<- for every newly added test file
//TODO: Split tests file in headers + CPP
//TODO: Put every API inside erd namespace

