#ifndef ERD_ERRORS_H
#define ERD_ERRORS_H

#include <map>
#include <string>

typedef std::string errorMessage;

errorMessage const ERROR_MSG_EMPTY_VALUE = "Empty value: ";
errorMessage const ERROR_MSG_NONCE = "Invalid nonce.";
errorMessage const ERROR_MSG_VALUE = "Invalid value: ";
errorMessage const ERROR_MSG_RECEIVER = "Invalid receiver.";
errorMessage const ERROR_MSG_SENDER = "Invalid sender.";
errorMessage const ERROR_MSG_GAS_PRICE = "Invalid gas price.";
errorMessage const ERROR_MSG_GAS_LIMIT = "Invalid gas limit.";
errorMessage const ERROR_MSG_CHAIN_ID = "Invalid chain id.";
errorMessage const ERROR_MSG_VERSION = "Invalid version.";
errorMessage const ERROR_MSG_SIGNATURE = "Missing signature.";
errorMessage const ERROR_MSG_SODIUM_INIT = "Could not initialize sodium library.";

errorMessage const ERROR_MSG_BECH32 = "Invalid bech32 address.";
errorMessage const ERROR_MSG_HEX = "Invalid hex digit format.";
errorMessage const ERROR_MSG_CONVERT_BITS = "Cannot convert bits";
errorMessage const ERROR_MSG_FILE_EMPTY = "File is empty.";
errorMessage const ERROR_MSG_FILE_DOES_NOT_EXIST = "File does not exists: ";
errorMessage const ERROR_MSG_FILE_EXTENSION_INVALID = "File extension invalid.";
errorMessage const ERROR_MSG_KEY_BYTES_SIZE = "Key bytes size invalid.";
errorMessage const ERROR_MSG_JSON_SERIALIZED = "Invalid serialized json string: ";
errorMessage const ERROR_MSG_JSON_SERIALIZE_EMPTY = "Empty json.";
errorMessage const ERROR_MSG_JSON_KEY_NOT_FOUND = "Json does not contain key: ";
errorMessage const ERROR_MSG_JSON_SET = "Json can not insert key:  ";
errorMessage const ERROR_MSG_HTTP_REQUEST_FAILED = "Request failed with message: ";
errorMessage const ERROR_MSG_REASON = "Error reason: ";
errorMessage const ERROR_MSG_KEY_FILE = "Invalid keyfile.";
errorMessage const ERROR_MSG_MAC = "MAC mismatch, possibly wrong password.";
errorMessage const ERROR_MSG_SCRYPTSY = "Scrypt function failed. Could not derive keys, possible cause: operating system refused to allocate the amount of requested memory.";
errorMessage const ERROR_MSG_KEY_FILE_VERSION = "Key file version not supported.";
errorMessage const ERROR_MSG_KEY_FILE_DERIVATION_FUNCTION = "Key file derivation function not supported.";
errorMessage const ERROR_MSG_KEY_FILE_CIPHER = "Key file cipher algorithm not supported.";
errorMessage const ERROR_MSG_MISSING_PASSWORD = "Missing key file password.";

errorMessage const ERROR_MSG_NEGATIVE_VALUE = "Received negative value";
errorMessage const ERROR_MSG_CANNOT_CONVERT_TO_BASE = "Cannot convert number to base: ";

#endif
