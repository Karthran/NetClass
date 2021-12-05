#pragma once
#include <string>

class Client;
enum class OperationCode;

class ClientApp
{
public:
    auto run() -> void;
    auto sendToServer(std::string& message, OperationCode operation_code) const -> void;

private:
    Client* _client{nullptr};

    auto loop() -> void;
    auto talkToServer(const std::string& message) const -> const std::string&;
};
