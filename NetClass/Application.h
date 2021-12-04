#pragma once
class Server;
class Application
{
public:
    auto run() -> void;
    auto reaction(const std::string& message) -> const std::string&;
private:
    Server* server{nullptr};
};
