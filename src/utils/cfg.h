#ifndef ERD_CFG_H
#define ERD_CFG_H

#define ON true
#define OFF false

// Switch flag for tests rely on some the following conditions:
// 1. Testnet is up and running
// 2. Used accounts to send transactions have enough funds
// 3. There are no concurrent transactions using the same account on testnet
// 4. Testnet has not been reset (e.g.: tested hash transaction still exists)
// If all of the above conditions are satisfied, one might turn this flag ON
// resulting on successfully executed tests, otherwise the behavior is undefined
#define HTTP_PRECONDITIONS OFF

#endif
