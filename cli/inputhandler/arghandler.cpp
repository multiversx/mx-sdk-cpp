#include "arghandler.h"
#include <iostream>
#include <stdint.h>
#include <limits>


namespace internal
{
    // Generic template function to check for user input value.
    // Expects input to be an unsigned long int.
    template<typename T>
    bool isUserInputValid(std::string arg)
    {
        if ((arg.size() == 0) || (arg.size() > std::numeric_limits<uint64_t>::digits)) return false;

        std::string::const_iterator it = arg.begin();
        while (it != arg.end() && isdigit(*it)) ++it;
        return  it == arg.end();
    }

    // Specialization template function to check for user input value.
    // Expects input to be a non-empty string.
    template<>
    bool isUserInputValid<std::string>(std::string arg)
    {
        return (arg.size() != 0);
    }
}

namespace ih
{

  RequestedCmd::RequestedCmd(std::map<uint32_t, std::string> const userInputs,
                             RequestType const reqType, errorCode const errCode) :
    m_userInputs(userInputs),
    m_requestType(reqType),
    m_errCode(errCode) {}

  const std::map<uint32_t, std::string>& RequestedCmd::getUserInputs() const
  {
    return m_userInputs;
  }

  const RequestType& RequestedCmd::getRequestType() const
  {
    return m_requestType;
  }

  const errorCode& RequestedCmd::getErrorCode() const
  {
    return m_errCode;
  }


  ArgHandler::ArgHandler(int const& argc, char* const argv[]) : m_errCode(ERROR_NONE)
  {
    if (argc > 1)
    {
      for (int ct = 1; ct < argc; ++ct)
      {
        m_arguments.push_back(std::string(argv[ct]));
      }
    }
  }

  ArgHandler::commandGroupMap const ArgHandler::m_commandGroupMap =
  {
    {{"transaction"} , {"new"}},
    {"pem" , {"load"}},
    {"help" , {}}
  };

  bool ArgHandler::isSubCmd(uint32_t const subCmdIdx, std::string const subCmd) const
  {
    if (argCount() <= subCmdIdx) return false;
    return (m_arguments[subCmdIdx] == subCmd);
  }

  bool ArgHandler::isCmdGroup(std::string const arg) const
  {
    if (argCount() == 0) return false;
    return (m_arguments[0] == arg);
  }


  template <typename T>
  bool ArgHandler::checkAndSetUserInput(uint32_t const argIdx, std::string const arg,
                                        std::map<uint32_t, std::string>& userInputs, uint32_t userInputIdx,
                                        errorCode errCode)
  {
    // If user didn't provide enough arguments OR
    // size(user arg) <= size(required arg)
    if (argCount() <= argIdx) return false;
    if (m_arguments[argIdx].size() <= arg.size())
    {
      m_errCode |= errCode;
      return false;
    }

    bool ret = false;

    std::string const userArg = m_arguments[argIdx];
    std::string const userCmd = userArg.substr(0, arg.size());
    bool const isCmdValid = (userCmd == arg) ? (true) : (false);

    if (isCmdValid)
    {
      std::string const userInput = userArg.substr(arg.size(), userArg.size());
      if (internal::isUserInputValid<T>(userInput))
      {
        userInputs[userInputIdx] = userInput;
        ret = true;
      }
      else
      {
        m_errCode |= errCode;
      }
    }

    return ret;
  }

  RequestedCmd ArgHandler::getRequestedCmd()
  {
    m_errCode = ERROR_NONE;
    RequestType reqType = invalid;
    std::map<uint32_t, std::string> userInputs;

    // TODO: No magic numbers/strings
    if ((argCount() == 1) && isCmdGroup("help"))
    {
      reqType = help;
    }
    else if ((argCount() == 3) && isCmdGroup("pem") && isSubCmd(1U, "load") &&
             (checkAndSetUserInput<std::string>(2U, "--file=", userInputs, ARGS_TX_IDX_PEM_INPUT_FILE, ERROR_PEM_INPUT_FILE)))
    {
      reqType = loadPemFile;
    }
    else if (((argCount() == 9) || (argCount() == 10)) && isCmdGroup("transaction") && isSubCmd(1U, "new") &&
      checkAndSetUserInput<uint64_t>(2U, "--nonce=", userInputs, ARGS_TX_IDX_NONCE, ERROR_NONCE) &&
      checkAndSetUserInput<std::string>(3U, "--value=", userInputs, ARGS_TX_IDX_VALUE, ERROR_VALUE) &&
      checkAndSetUserInput<std::string>(4U, "--receiver=", userInputs, ARGS_TX_IDX_RECEIVER, ERROR_RECEIVER) &&
      checkAndSetUserInput<uint64_t>(5U, "--gas-price=", userInputs, ARGS_TX_IDX_GAS_PRICE, ERROR_GAS_PRICE) &&
      checkAndSetUserInput<uint64_t>(6U, "--gas-limit=", userInputs, ARGS_TX_IDX_GAS_LIMIT, ERROR_GAS_LIMIT) &&
      checkAndSetUserInput<std::string>(7U, "--pem=", userInputs, ARGS_TX_IDX_PEM_INPUT_FILE, ERROR_PEM_INPUT_FILE) &&
      checkAndSetUserInput<std::string>(8U, "--outfile=", userInputs, ARGS_TX_IDX_JSON_OUT_FILE, ERROR_JSON_OUT_FILE))
    {
      reqType = createSignedTransactionWithPemFile;

      if ((argCount() == 10) && (!checkAndSetUserInput<std::string>(9U, "--data=", userInputs, ARGS_TX_IDX_DATA, ERROR_DATA)))
      {
        reqType = invalid;
      }
    }

    return RequestedCmd(userInputs, reqType ,m_errCode);
  }

  int ArgHandler::argCount() const
  {
    return m_arguments.size();
  }

  void ArgHandler::showSubGroupAvailableCmds(std::string cmdGroup) const
  {
    std::vector<std::string> cmd = m_commandGroupMap.at(cmdGroup);

    if (cmd.size() == 0) std::cerr << "-";

    else
    {
      for (std::string subCmd : cmd)
      {
        std::cerr << subCmd << " ";
      }
    }
  }

  void ArgHandler::showInfo() const
  {
    std::cerr << "----\nInfo\n----\n\nCommand groups: Avaiable arguments\n";

    for (auto it = m_commandGroupMap.begin(); it != m_commandGroupMap.end(); ++it)
    {
      std::cerr << it->first << ": ";
      showSubGroupAvailableCmds(it->first);
      std::cerr << "\n";
    }
  }
}
