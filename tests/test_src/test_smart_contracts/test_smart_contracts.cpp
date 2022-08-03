#include "gtest/gtest.h"
#include "smartcontracts/contract_call.h"

TEST(SCArguments, add_empty_asOnData)
{
    SCArguments args;

    EXPECT_TRUE(args.empty());
    EXPECT_TRUE(args.asOnData().empty());

    BigUInt const bigUInt("10");
    Address const address("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th");
    std::string const str("foo");

    args.add(bigUInt);
    args.add(address);
    args.add(str);

    EXPECT_FALSE(args.empty());
    EXPECT_EQ(args.asOnData(), "@0a@0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1@666f6f");
}

TEST(ContractCall, setArgs_asOnData)
{
    ContractCall contractCall1("enterFarmProxy");
    EXPECT_EQ(contractCall1.asOnData(), "@656e7465724661726d50726f7879");

    SCArguments args;
    args.add(Address("erd1qqqqqqqqqqqqqpgq7qhsw8kffad85jtt79t9ym0a4ycvan9a2jps0zkpen"));
    args.add(BigUInt(3983756));
    contractCall1.setArgs(args);
    EXPECT_EQ(contractCall1.asOnData(), "@656e7465724661726d50726f7879@00000000000000000500f02f071ec94f5a7a496bf156526dfda930ceccbd5483@3cc98c");

    ContractCall contractCall2("enterFarmProxy", args);
    EXPECT_EQ(contractCall2.asOnData(), "@656e7465724661726d50726f7879@00000000000000000500f02f071ec94f5a7a496bf156526dfda930ceccbd5483@3cc98c");
}
