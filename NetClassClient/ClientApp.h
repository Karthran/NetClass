#pragma once
#include <string>

class Client;

class ClientApp
{
public:
    auto run() -> void;
    auto talk_to_server(const std::string& message)const -> const std::string&;

private:
    Client* _client{nullptr};

    auto loop() -> void;

};
