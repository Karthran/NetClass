#include <iostream>
#include <sstream>

#include "Application.h"
#include "Server.h"
#include "../core.h"
auto Application::run() -> void
{
    _server = new Server(this);
    _server->run();

    std::string msg{};
    while (true)
    {
        std::cin >> msg;
        if (msg == "end")
        {
            _server->setContinueFlag(false);
            break;
        }
    }
}

auto Application::reaction(const std::string& in_message, std::string& out_message, int thread_num) -> void
{
    std::string code_operation_string;
    std::stringstream stream(in_message);
    stream >> code_operation_string;
    std::cout << code_operation_string << std::endl;
    auto code_operation = static_cast<OperationCode>(std::stoi(code_operation_string));
    switch (code_operation)
    {
        case OperationCode::CHECK_SIZE: onCheckSize(in_message, out_message, thread_num); break;
        case OperationCode::CHECK_NAME: onCheckName(in_message, out_message); break;
        case OperationCode::CHECK_LOGIN: break;
        case OperationCode::REGISTRATION: break;
        case OperationCode::SIGN_IN: break;
        default: return onError(out_message); break;
    }
}

auto Application::onCheckSize(const std::string& in_message, std::string& out_message, int thread_num) const -> void
{
    std::string size_string;
    std::stringstream stream(in_message);
    stream >> size_string >> size_string;

    auto message_length{std::stoi(size_string)};

    _server->setBufferSize(thread_num, message_length);

    out_message = in_message;
}

auto Application::onCheckName(const std::string& in_message, std::string& out_message) -> void
{
    std::string code_operation_string;
    std::stringstream stream(in_message);
    stream >> code_operation_string >> code_operation_string;
    auto code_operation = static_cast<OperationCode>(std::stoi(code_operation_string));
    auto msg{getName()};
    switch (code_operation)
    {
        case OperationCode::CHECK_SIZE: 
            out_message = std::to_string(static_cast<int>(OperationCode::CHECK_SIZE)) + " " + std::to_string(msg.size());
            break;
        case OperationCode::READY:
            out_message = std::to_string(static_cast<int>(OperationCode::CHECK_NAME)) + " " + msg;
            break;
        default: return onError(out_message); break;
    }

    auto message_size{getName().size()};
}

auto Application::onError(std::string& out_message) const -> void
{
    out_message = std::to_string(static_cast<int>(OperationCode::ERROR)) + " " + "ERROR";
}
