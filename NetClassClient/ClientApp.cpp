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

auto ClientApp::sendToServer(std::string& message, OperationCode operation_code) const -> void
{
    std::string msg_to_srv = std::to_string(static_cast<int>(OperationCode::CHECK_SIZE)) + " " + std::to_string(message.size() + HEADER_SIZE);
    std::string msg_from_srv = talkToServer(msg_to_srv);

    msg_to_srv = std::to_string(static_cast<int>(OperationCode::CHECK_NAME)) + " " +
                 std::to_string(static_cast<int>(OperationCode::CHECK_SIZE)) + " " + message;
    msg_from_srv = talkToServer(msg_to_srv);

    auto msg_size{std::stoi(std::string(msg_from_srv, 2))};

    _client->setBufferSize(msg_size + HEADER_SIZE);

    msg_to_srv = std::to_string(static_cast<int>(OperationCode::CHECK_NAME)) + " " +
                 std::to_string(static_cast<int>(OperationCode::READY)) + " " + message;

    message = talkToServer(msg_to_srv);
    //return

}

auto ClientApp::talkToServer(const std::string& message) const -> const std::string&
{
    while (_client->getOutMessageReady())
    {
    }
    //std::cout << message << std::endl;
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
            talkToServer(std::to_string(static_cast<int>(OperationCode::STOP)));
            loop_flag = false;
            break;
        }
        sendToServer(message, OperationCode::CHECK_NAME);

        std::cout << message << std::endl;
    }
    std::cout << "Client stop!" << std::endl;
    return;
}
