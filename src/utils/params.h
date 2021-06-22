#ifndef ERD_PARAMS_H
#define ERD_PARAMS_H

#include <map>
#include <string>

constexpr auto TX_NONCE = "nonce";
constexpr auto TX_VALUE = "value";
constexpr auto TX_RECEIVER = "receiver";
constexpr auto TX_SENDER = "sender";
constexpr auto TX_RECEIVER_NAME = "receiverUsername";
constexpr auto TX_SENDER_NAME = "senderUsername";
constexpr auto TX_GAS_PRICE = "gasPrice";
constexpr auto TX_GAS_LIMIT = "gasLimit";
constexpr auto TX_DATA = "data";
constexpr auto TX_CHAIN_ID = "chainID";
constexpr auto TX_VERSION = "version";
constexpr auto TX_SIGNATURE = "signature";
constexpr auto TX_OPTIONS = "options";

constexpr auto ARGS_TX_IDX_NONCE = 0U;
constexpr auto ARGS_TX_IDX_VALUE = 1U;
constexpr auto ARGS_TX_IDX_RECEIVER = 2U;
constexpr auto ARGS_TX_IDX_RECEIVER_NAME = 3U;
constexpr auto ARGS_TX_IDX_SENDER_NAME = 4U;
constexpr auto ARGS_TX_IDX_GAS_PRICE = 5U;
constexpr auto ARGS_TX_IDX_GAS_LIMIT = 6U;
constexpr auto ARGS_TX_IDX_PEM_INPUT_FILE = 7U;
constexpr auto ARGS_TX_IDX_JSON_OUT_FILE = 8U;
constexpr auto ARGS_TX_IDX_DATA = 9U;
constexpr auto ARGS_TX_IDX_CHAIN_ID = 10U;
constexpr auto ARGS_TX_IDX_OPTIONS= 11U;

#endif
