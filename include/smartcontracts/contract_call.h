#ifndef ERD_CONTRACT_CALL_H
#define ERD_CONTRACT_CALL_H

#include "sc_arguments.h"

class ContractCall
{
public:
    explicit ContractCall(std::string function);

    explicit ContractCall(std::string function, SCArguments args);

    void setArgs(SCArguments args);

    std::string asOnData() const;

private:
    std::string m_function;
    SCArguments m_args;
};

#endif //ERD_CONTRACT_CALL_H
