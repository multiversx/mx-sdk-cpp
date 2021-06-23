#ifndef ERD_ERRORS_H
#define ERD_ERRORS_H

#include <map>
#include <string>

typedef unsigned long errorCode;
typedef std::string errorMessage;

errorCode const ERROR_NONE = 0UL;

errorCode const ERROR_NONCE = 1U;
errorCode const ERROR_VALUE = 2U;
errorCode const ERROR_RECEIVER = 3U;
errorCode const ERROR_GAS_PRICE = 4U;
errorCode const ERROR_GAS_LIMIT = 5U;
errorCode const ERROR_PEM_INPUT_FILE = 6U;
errorCode const ERROR_JSON_OUT_FILE = 7U;
errorCode const ERROR_DATA = 8U;
errorCode const ERROR_SODIUM_INIT = 9U;

errorMessage const ERROR_MSG_NONCE = "Invalid nonce.";
errorMessage const ERROR_MSG_VALUE = "Invalid value.";
errorMessage const ERROR_MSG_RECEIVER = "Invalid receiver.";
errorMessage const ERROR_MSG_SENDER = "Invalid sender.";
errorMessage const ERROR_MSG_GAS_PRICE = "Invalid gas price.";
errorMessage const ERROR_MSG_GAS_LIMIT = "Invalid gas limit.";
errorMessage const ERROR_MSG_CHAIN_ID = "Invalid chain id.";
errorMessage const ERROR_MSG_VERSION = "Invalid version.";
errorMessage const ERROR_MSG_PEM_INPUT_FILE = "Invalid pem file.";
errorMessage const ERROR_MSG_JSON_OUT_FILE = "Invalid json file.";
errorMessage const ERROR_MSG_DATA = "Invalid data.";
errorMessage const ERROR_MSG_SODIUM_INIT = "Could not initialize sodium library.";

errorMessage const ERROR_MSG_BECH32 = "Invalid bech32 address.";
errorMessage const ERROR_MSG_HEX = "Invalid hex digit format.";
errorMessage const ERROR_MSG_CONVERT_BITS = "Cannot convert bits";
errorMessage const ERROR_MSG_FILE_EMPTY = "File is empty.";
errorMessage const ERROR_MSG_FILE_DOES_NOT_EXIST = "File does not exists.";
errorMessage const ERROR_MSG_FILE_EXTENSION_INVALID = "File extension invalid.";
errorMessage const ERROR_MSG_KEY_BYTES_SIZE = "Key bytes size invalid.";
errorMessage const ERROR_MSG_JSON_SERIALIZED = "Invalid serialized json string: ";
errorMessage const ERROR_MSG_JSON_SERIALIZE_EMPTY = "Empty json.";
errorMessage const ERROR_MSG_JSON_KEY_NOT_FOUND = "Json does not contain key: ";
errorMessage const ERROR_MSG_JSON_SET = "Json can not insert key:  ";

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
