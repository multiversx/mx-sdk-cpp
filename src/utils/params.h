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

#endif
