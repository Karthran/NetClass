#include <iostream>
#include <sstream>
#include <exception>

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
    std::cout << "CODE: " << code_operation_string << std::endl;
    try{
    auto code_operation = static_cast<OperationCode>(std::stoi(code_operation_string));
    switch (code_operation)
    {
        case OperationCode::STOP: onStop(in_message, out_message, thread_num); break;
        case OperationCode::CHECK_SIZE: onCheckSize(in_message, out_message, thread_num); break;
        case OperationCode::CHECK_NAME: onCheckName(in_message, out_message, thread_num); break;
        case OperationCode::CHECK_LOGIN: break;
        case OperationCode::REGISTRATION: break;
        case OperationCode::SIGN_IN: break;
        default: return onError(out_message); break;
    }
    }
    catch (const std::invalid_argument & e) {
    //    throw NoNumber(input);
      out_message = in_message;
    }
}

auto Application::onCheckSize(const std::string& in_message, std::string& out_message, int thread_num) const -> void
{
    std::string size_string;
    std::stringstream stream(in_message);
    stream >> size_string >> size_string;

    std::cout << "SIZE: " << size_string << std::endl;

    auto message_length{std::stoi(size_string)};

    _server->setBufferSize(thread_num, message_length + HEADER_SIZE);

    out_message = in_message;
}

auto Application::onCheckName(const std::string& in_message, std::string& out_message, int thread_num) -> void
{
    std::string code_operation_string;
    std::string name;
    std::stringstream stream(in_message);
    stream >> code_operation_string >> code_operation_string >> name;
    auto code_operation = static_cast<OperationCode>(std::stoi(code_operation_string));
    switch (code_operation)
    {
        case OperationCode::CHECK_SIZE:
        {

            auto msg{checkName(name)};
            _server->setCashMessage(checkName(name), thread_num);
            out_message = std::to_string(static_cast<int>(OperationCode::CHECK_SIZE)) + " " + std::to_string(msg.size() + HEADER_SIZE);
            break;
        }
        case OperationCode::READY:
            out_message = /*std::to_string(static_cast<int>(OperationCode::CHECK_NAME)) + " " + */_server->getCashMessage(thread_num);
            break;
        default: return onError(out_message); break;
    }

}

auto Application::onStop(const std::string& in_message, std::string& out_message, int thread_num) -> void {}

auto Application::onError(std::string& out_message) const -> void
{
    out_message = std::to_string(static_cast<int>(OperationCode::ERROR)) + " " + "ERROR";
}

auto Application::checkName(const std::string& name) -> const std::string
{
    if (name == "Jhon") return "OK";
    return "ERROR";
}
                    