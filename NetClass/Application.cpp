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

auto Application::reaction(char* message, int thread_num) -> void
{
    auto code{-1};
    getFromBuffer(message, 0, code);
    std::cout << "CODE: " << code << std::endl;
    try
    {
        auto code_operation = static_cast<OperationCode>(code);
        switch (code_operation)
        {
            case OperationCode::STOP: onStop(message, thread_num); break;
            case OperationCode::CHECK_SIZE: onCheckSize(message, thread_num); break;
            case OperationCode::CHECK_NAME: onCheckName(message, thread_num); break;
            case OperationCode::CHECK_LOGIN: break;
            case OperationCode::REGISTRATION: break;
            case OperationCode::SIGN_IN: break;
            default: return onError(message, thread_num); break;
        }
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Invalid argument!" << std::endl;
        const char* inv_arg = "Invalid argument!";
        _server->getMessageSizeRef(thread_num) = 0;
        addToBuffer(message, _server->getMessageSizeRef(thread_num), inv_arg, strlen(inv_arg));
    }
}

auto Application::onCheckSize(char* message, int thread_num) const -> void
{
    auto message_length{-1};
    getFromBuffer(message, sizeof(int), message_length);
    std::cout << "Message length: " << message_length << std::endl;

    _server->setBufferSize(thread_num, message_length + HEADER_SIZE);
    _server->getMessageSizeRef(thread_num) = 2 * sizeof(message_length);  // first int CHECK_SIZE
    _server->setMsgFromClientSize(message_length, thread_num);
}

auto Application::onCheckName(char* message, int thread_num) -> void
{
    auto code_operation{-1};
    getFromBuffer(message, sizeof(int), code_operation);
    std::cout << "CODE: " << code_operation << std::endl;

    auto code = static_cast<OperationCode>(code_operation);
    switch (code)
    {
        case OperationCode::CHECK_SIZE:
        {
            checkName(message + 2 * sizeof(int), _server->getMsgFromClientSize(thread_num), thread_num);
            _server->getMessageSizeRef(thread_num) = 0;
            addToBuffer(message, _server->getMessageSizeRef(thread_num), static_cast<int>(OperationCode::CHECK_SIZE));
            addToBuffer(message, _server->getMessageSizeRef(thread_num), _server->getCashMessageSizeRef(thread_num));
            break;
        }
        case OperationCode::READY:
            _server->getMessageSizeRef(thread_num) = 0;
            addToBuffer(message, _server->getMessageSizeRef(thread_num), _server->getCashMessagePtr(thread_num),
                _server->getCashMessageSizeRef(thread_num));
            break;
        default: return onError(message, thread_num); break;
    }
}

auto Application::onStop(char* message, int thread_num) -> void {}

auto Application::onError(char* message, int thread_num) const -> void
{
    addToBuffer(message, _server->getMessageSizeRef(thread_num), static_cast<int>(OperationCode::ERROR));
    const char* err = "ERROR";
    addToBuffer(message, _server->getMessageSizeRef(thread_num), err, strlen(err));
    // out_message = std::to_string(static_cast<int>(OperationCode::ERROR)) + " " + "ERROR";
}

auto Application::checkName(char* name, size_t name_size, int thread_num) -> void
{
    const char* check_name = "Jhon";
    auto flag{true};
    for (auto i{0}; i < strlen(check_name); ++i)
    {
        if (check_name[i] != name[i])
        {
            flag = false;
            break;
        }
    }
    if (strlen(check_name) != name_size) flag = false;
    const char* result{nullptr};
    if (flag)
        result = "OK";
    else
        result = "ERROR";

    _server->resizeCashMessageBuffer(thread_num, strlen(result) + HEADER_SIZE);

    _server->getCashMessageSizeRef(thread_num) = 0;
    addToBuffer(_server->getCashMessagePtr(thread_num), _server->getCashMessageSizeRef(thread_num), result, strlen(result));
}
auto Application::addToBuffer(char* buffer, size_t& cur_msg_len, int value) const -> void
{
    auto length{sizeof(value)};
    auto char_ptr{reinterpret_cast<char*>(&value)};

    for (auto i{0}; i < length; ++i)
    {
        buffer[i + cur_msg_len] = char_ptr[i];
    }
    cur_msg_len += length;
}

auto Application::addToBuffer(char* buffer, size_t& cur_msg_len, const char* string, size_t str_len) const -> void
{
    for (auto i{0}; i < str_len; ++i)
    {
        buffer[i + cur_msg_len] = string[i];
    }
    cur_msg_len += str_len;
}

auto Application::getFromBuffer(const char* buffer, size_t shift, int& value) const -> void
{
    char val_buff[sizeof(value)];
    auto length{sizeof(value)};

    for (auto i{0}; i < length; ++i)
    {
        val_buff[i] = buffer[shift + i];
    }
    value = *(reinterpret_cast<int*>(val_buff));
}

auto Application::getFromBuffer(const char* buffer, size_t shift, char* string, size_t str_len) const -> void
{
    for (auto i{0}; i < str_len; ++i)
    {
        string[i] = buffer[shift + i];
    }
}
