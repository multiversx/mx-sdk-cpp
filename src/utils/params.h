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

constexpr auto NETWORK_CONFIG_DEFAULT_CHAIN_ID = "T";
constexpr auto NETWORK_CONFIG_DEFAULT_PROXY_URL = "https://testnet-gateway.elrond.com";
constexpr auto NETWORK_CONFIG_DEFAULT_ESDT_ISSUE_SC_ADDRESS = "erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u";

#endif
