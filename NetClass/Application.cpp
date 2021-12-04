#include <iostream>

#include "Application.h"
#include "Server.h"

auto Application::run() -> void 
{
    server = new Server(this);
    server->run();

    std::string msg{};
    while (true)
    {
        std::cin >> msg;
        if (msg == "end")
        {
            server->setContinueFlag(false);
            break;
        }
    }
}

auto Application::reaction(const std::string& message) -> const std::string&
{
    return message;
}
