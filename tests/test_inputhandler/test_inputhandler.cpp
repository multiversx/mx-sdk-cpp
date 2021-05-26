#include "gtest/gtest.h"
#include "../../src/InputHandler/ext.h"
#include "../../src/Utils/ext.h"
#include <sodium.h>

TEST(ArgHandler, getRequestedCmd_getRequestType_noArgument_expectInvalid)
{
  int const argc = 1;
  char* argv[argc];
  argv[0] = "ERDProject.exe";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_randomArgs_expectInvalid)
{
  int const argc = 3;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "dsa";
  argv[2] = "";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_help_expectHelp)
{
  int const argc = 2;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "help";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::help);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_pem_load_someFile_expectLoadPemFile)
{
  int const argc = 4;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "pem";
  argv[2] = "load";
  argv[3] = "--file=someFile";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::loadPemFile);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_pem_load_withoutFile_expectInvalid)
{
  int const argc = 3;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "pem";
  argv[2] = "help";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_pem_withoutSubArgument_expectInvalid)
{
  int const argc = 2;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "pem";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::invalid);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_transaction_new_noData_expectCreateTransaction)
{
  int const argc = 10;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=3";
  argv[4] = "--value=\"31\"";
  argv[5] = "--receiver=\"da\"";
  argv[6] = "--gas-price=31";
  argv[7] = "--gas-limit=31";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::createTransaction);
}

TEST(ArgHandler, getRequestedCmd_getRequestType_transaction_new_withData_expectCreateTransaction)
{
  int const argc = 11;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=3";
  argv[4] = "--value=\"31\"";
  argv[5] = "--receiver=\"da\"";
  argv[6] = "--gas-price=31";
  argv[7] = "--gas-limit=31";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getRequestType(), ih::createTransaction);
}


TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidNonce_expectErrorNonce)
{
  int const argc = 11;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=3f";
  argv[4] = "--value=\"31\"";
  argv[5] = "--receiver=\"da\"";
  argv[6] = "--gas-price=31";
  argv[7] = "--gas-limit=31";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_NONCE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidValue_expectErrorValue)
{
  int const argc = 11;
  char* argv[argc];
  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=3";
  argv[4] = "--value=";
  argv[5] = "--receiver=\"da\"";
  argv[6] = "--gas-price=31";
  argv[7] = "--gas-limit=31";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_VALUE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidReceiver_expectErrorReceiver)
{
  int const argc = 11;
  char* argv[argc];

  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=0";
  argv[4] = "--value=10000";
  argv[5] = "--receiver=";
  argv[6] = "--gas-price=31";
  argv[7] = "--gas-limit=31";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_RECEIVER);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidGasPrice_expectErrorGasPrice)
{
  int const argc = 11;
  char* argv[argc];

  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=0";
  argv[4] = "--value=10000";
  argv[5] = "--receiver=address";
  argv[6] = "--gas-price=abc";
  argv[7] = "--gas-limit=31";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_GAS_PRICE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidGasLimit_expectErrorGasLimit)
{
  int const argc = 11;
  char* argv[argc];

  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=0";
  argv[4] = "--value=10000";
  argv[5] = "--receiver=address";
  argv[6] = "--gas-price=1000";
  argv[7] = "--gas-limit=abc";
  argv[8] = "--pem=\"dd\"";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_GAS_LIMIT);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidPem_expectErrorPem)
{
  int const argc = 11;
  char* argv[argc];

  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=0";
  argv[4] = "--value=10000";
  argv[5] = "--receiver=address";
  argv[6] = "--gas-price=1000";
  argv[7] = "--gas-limit=700";
  argv[8] = "--pem=";
  argv[9] = "--outfile=\"dd\"";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_PEM_INPUT_FILE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidJson_expectErrorJson)
{
  int const argc = 11;
  char* argv[argc];

  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=0";
  argv[4] = "--value=10000";
  argv[5] = "--receiver=address";
  argv[6] = "--gas-price=1000";
  argv[7] = "--gas-limit=700";
  argv[8] = "--pem=someFile";
  argv[9] = "--outfile=";
  argv[10] = "--data=\"dd\"";

  ih::ArgHandler argHandler(argc, argv);

  EXPECT_EQ(argHandler.getRequestedCmd().getErrorCode(), ERROR_JSON_OUT_FILE);
}

TEST(ArgHandler, getRequestedCmd_getErrorCode_transaction_new_invalidData_expectErrorData)
{
  int const argc = 11;
  char* argv[argc];

  argv[0] = "ERDProject.exe";
  argv[1] = "transaction";
  argv[2] = "new";
  argv[3] = "--nonce=0";
  argv[4] = "--value=10000";
  argv[5] = "--receiver=address";
  argv[6] = "--gas-price=1000";
  argv[7] = "--gas-limit=700";
  argv[8] = "--pem=someFile";
  argv[9] = "--outfile=otherFile";
  argv[10] = "--data=";

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


TEST(JsonHandler, writeOutputFile)
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
  ih::wrapper::JsonHandlerInputWrapper const jsonWrapper(input);

  ih::JsonHandler jsonHandler(pemWrapper, jsonWrapper);

  jsonHandler.writeOutputFile();
}

TEST(PemFileReader, printFileContent)
{
  std::map<uint32_t, std::string> inputData;
  inputData[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keys.pem";
  ih::wrapper::PemHandlerInputWrapper const pemWrapper(inputData);
  ih::PemFileHandler pemHandler(pemWrapper);

  pemHandler.printFileContent();
}

TEST(PemFileReader, isPemFileValid_validFile)
{
  std::map<uint32_t, std::string> inputData;
  inputData[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keys.pem";
  ih::wrapper::PemHandlerInputWrapper const pemWrapper(inputData);
  ih::PemFileHandler pemHandler(pemWrapper);

  EXPECT_TRUE(pemHandler.isFileValid());
}

TEST(PemFileReader, isPemFileValid_invalidFileExtension)
{
  std::map<uint32_t, std::string> inputData;
  inputData[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keys.pme";
  ih::wrapper::PemHandlerInputWrapper const pemWrapper(inputData);
  ih::PemFileHandler pemHandler(pemWrapper);

  EXPECT_FALSE(pemHandler.isFileValid());
}

TEST(PemFileReader, isPemFileValid_emptyFile)
{
  std::map<uint32_t, std::string> inputData;
  inputData[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keysEmptyFile.pem";
  ih::wrapper::PemHandlerInputWrapper const pemWrapper(inputData);
  ih::PemFileHandler pemHandler(pemWrapper);

  EXPECT_FALSE(pemHandler.isFileValid());
}

TEST(PemFileReader, getPublicPrivateKeys_expectSameResultFrom_libsodium)
{
  std::map<uint32_t, std::string> inputData;
  inputData[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keys.pem";
  ih::wrapper::PemHandlerInputWrapper const pemWrapper(inputData);
  ih::PemFileHandler pemHandler(pemWrapper);

  unsigned char pemPk[crypto_sign_PUBLICKEYBYTES];
  unsigned char pemSk[crypto_sign_SECRETKEYBYTES];
  unsigned char pemSeed[crypto_sign_SEEDBYTES];

  unsigned char sodiumPk[crypto_sign_PUBLICKEYBYTES];
  unsigned char sodiumSk[crypto_sign_SECRETKEYBYTES];

  pemHandler.getSeed(pemSeed);
  pemHandler.getAddress().getPublicKey(pemPk);
  pemHandler.getPrivateKey(pemSk);

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
  std::map<uint32_t, std::string> inputData;
  inputData[ARGS_TX_IDX_PEM_INPUT_FILE] = "..//testData//keys.pem";
  ih::wrapper::PemHandlerInputWrapper const pemWrapper(inputData);
  ih::PemFileHandler pemHandler(pemWrapper);

  std::string pemAddress = pemHandler.getAddress().getSegwitAddress();
  std::string expectedAdr = "erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4";

  EXPECT_EQ(pemAddress, expectedAdr);
}


