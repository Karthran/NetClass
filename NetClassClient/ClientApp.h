#pragma once
#include <string>

class Client;
enum class OperationCode;

class ClientApp
{
public:
    auto run() -> void;
    auto talk_to_server(const std::string& message, OperationCode operation_code) const -> const std::string&;

private:
    Client* _client{nullptr};

    auto loop() -> void;

};
