#include <iostream>

#include "ClientApp.h"
#include "Client.h"

auto ClientApp::run() -> void
{
    _client = new Client();
    _client->run();
    loop();
}

auto ClientApp::talk_to_server(const std::string& message)const -> const std::string&
{
    while (_client->getOutMessageReady())
    {
    }
    // std::cin >> message;
    //    std::getline(std::cin, message);
    _client->setMessage(message);
    _client->setOutMessageReady(true);
    // if (message == "end")
    //{
    //    loop_flag = false;
    //}
    while (!_client->getInMessageReady())
    {
        if (_client->getServerError())
        {
            //            loop_flag = false;
            break;
        }
    }
    // std::cout << message << std::endl;
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
        message = talk_to_server(message);

        std::cout << message << std::endl;
    }
    std::cout << "Client stop!" << std::endl;
    return;
}
