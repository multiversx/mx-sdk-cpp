#ifndef MX_CLI_UTILITY_H
#define MX_CLI_UTILITY_H

#include "inputhandler/ext.h"
#include "config/cliconfig.h"
#include "erdsdk.h"

namespace cli
{
namespace utility
{
Transaction createTransaction(cxxopts::ParseResult const &result, Config const &config);

void signTransaction(Transaction &transaction, std::shared_ptr<ISecretKey> const &keyFile);
}
}

#endif
