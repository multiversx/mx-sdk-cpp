#include "inputhandler/ext.h"
#include "commandhandler.h"

int main(int argc, char *argv[])
{
    if (!cli::init())
    {
        return 0;
    }

    ih::ArgHandler argHandler(argc, argv);

    ih::RequestedCmd const requestedCmd = argHandler.getRequestedCmd();
    errorCode const errCode = requestedCmd.getErrorCode();

    if (errCode != ERROR_NONE)
    {
        cli::reportError(errCode);
        return 0;
    }

    cli::handleRequest(requestedCmd);

    return 0;
}
