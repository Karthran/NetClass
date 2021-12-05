#include <iostream>

#include "ClientApp.h"
#include "Client.h"
#include "../core.h"

auto ClientApp::run() -> void
{
    _client = new Client();
    _client->run();
    loop();
}

auto ClientApp::talk_to_server(const std::string& message, OperationCode operation_code) const -> const std::string&
{
    while (_client->getOutMessageReady())
    {
    }
    _client->setMessage(message);
    _client->setOutMessageReady(true);
    while (!_client->getInMessageReady())
    {
        if (_client->getServerError())
        {
            break;
        }
    }
    _client->setInMessageReady(false);
    return _client->getMessage();
}

auto ClientApp::loop() -> void
{
    std::string message;
    bool loop_flag = true;
    while (loop_flag)
    {
        std::getline(std::cin, message);
        if (message == "end")
        {
            loop_flag = false;
        }
        message = talk_to_server(message, OperationCode::CHECK_NAME);

        std::cout << message << std::endl;
    }
    std::cout << "Client stop!" << std::endl;
    return;
}
