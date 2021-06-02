#ifndef ERD_PARAMS_H
#define ERD_PARAMS_H

#include <map>
#include <string>

constexpr auto STR_JSON_NONCE = "nonce";
constexpr auto STR_JSON_VALUE = "value";
constexpr auto STR_JSON_RECEIVER = "receiver";
constexpr auto STR_JSON_SENDER = "sender";
constexpr auto STR_JSON_GAS_PRICE = "gasPrice";
constexpr auto STR_JSON_GAS_LIMIT = "gasLimit";
constexpr auto STR_JSON_DATA = "data";
constexpr auto STR_JSON_CHAIN_ID = "chainID";
constexpr auto STR_JSON_VERSION = "version";
constexpr auto STR_JSON_SIGNATURE = "signature";

constexpr auto JSON_TX_IDX_NONCE = 0U;
constexpr auto JSON_TX_IDX_VALUE = 1U;
constexpr auto JSON_TX_IDX_RECEIVER = 2U;
constexpr auto JSON_TX_IDX_SENDER = 3U;
constexpr auto JSON_TX_IDX_GAS_PRICE = 4U;
constexpr auto JSON_TX_IDX_GAS_LIMIT = 5U;
constexpr auto JSON_TX_IDX_DATA = 6U;
constexpr auto JSON_TX_IDX_CHAIN_ID = 7U;
constexpr auto JSON_TX_IDX_VERSION = 8U;

constexpr auto JSON_TX_DEFAULT_CHAIN_ID = "T";
constexpr auto JSON_TX_DEFAULT_VERSION = 1U;

constexpr auto ARGS_TX_IDX_NONCE = 0U;
constexpr auto ARGS_TX_IDX_VALUE = 1U;
constexpr auto ARGS_TX_IDX_RECEIVER = 2U;
constexpr auto ARGS_TX_IDX_GAS_PRICE = 3U;
constexpr auto ARGS_TX_IDX_GAS_LIMIT = 4U;
constexpr auto ARGS_TX_IDX_PEM_INPUT_FILE = 5U;
constexpr auto ARGS_TX_IDX_JSON_OUT_FILE = 6U;
constexpr auto ARGS_TX_IDX_DATA = 7U;

typedef unsigned long errorCode;
typedef std::string errorMessage;

errorCode const ERROR_NONE = 0UL;

errorCode const ERROR_NONCE = 1U;
errorCode const ERROR_VALUE = 1UL << 1;
errorCode const ERROR_RECEIVER = 1UL << 2U;
errorCode const ERROR_GAS_PRICE = 1UL << 3U;
errorCode const ERROR_GAS_LIMIT = 1UL << 4U;
errorCode const ERROR_PEM_INPUT_FILE = 1UL << 5U;
errorCode const ERROR_JSON_OUT_FILE = 1UL << 6U;
errorCode const ERROR_DATA = 1UL << 7U;
errorCode const ERROR_SODIUM_INIT = 1UL << 8U;

errorMessage const ERROR_MSG_NONCE = "Invalid nonce.";
errorMessage const ERROR_MSG_VALUE = "Invalid value.";
errorMessage const ERROR_MSG_RECEIVER = "Invalid receiver.";
errorMessage const ERROR_MSG_GAS_PRICE = "Invalid gas price.";
errorMessage const ERROR_MSG_GAS_LIMIT = "Invalid gas limit.";
errorMessage const ERROR_MSG_PEM_INPUT_FILE = "Invalid pem file.";
errorMessage const ERROR_MSG_JSON_OUT_FILE = "Invalid json file.";
errorMessage const ERROR_MSG_DATA = "Invalid data.";
errorMessage const ERROR_MSG_SODIUM_INIT = "Could not initialize sodium library.";

std::map<errorCode, errorMessage> const errors =
        {
                {ERROR_NONCE,          ERROR_MSG_NONCE},
                {ERROR_VALUE,          ERROR_MSG_VALUE},
                {ERROR_RECEIVER,       ERROR_MSG_RECEIVER},
                {ERROR_GAS_PRICE,      ERROR_MSG_GAS_PRICE},
                {ERROR_GAS_LIMIT,      ERROR_MSG_GAS_LIMIT},
                {ERROR_PEM_INPUT_FILE, ERROR_MSG_PEM_INPUT_FILE},
                {ERROR_JSON_OUT_FILE,  ERROR_MSG_JSON_OUT_FILE},
                {ERROR_DATA,           ERROR_MSG_DATA},
                {ERROR_SODIUM_INIT,    ERROR_MSG_SODIUM_INIT}
        };

#endif
