#include "inputhandler/ext.h"
#include "cli_handler.h"

int main(int argc, char *argv[])
{
    try
    {
        cli::init();

        ih::ArgHandler argHandler;
        auto result = argHandler.parse(argc, argv);

        cli::handleRequest(result);
    }
    catch (std::exception const &exception)
    {
        std::cerr<< exception.what() << "\n";
    }

    return 0;
}
