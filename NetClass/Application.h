#pragma once
class Server;
class Application
{
public:
    auto run() -> void;
    auto reaction(const std::string& message, std::string& out_message, int thread_num) -> void;

private:
    Server* _server{nullptr};

    auto onCheckSize(const std::string& in_message, std::string& out_message, int thread_num) const -> void;
    auto onCheckName(const std::string& in_message, std::string& out_message, int thread_num) -> void;
    auto onStop(const std::string& in_message, std::string& out_message, int thread_num) -> void;
    auto onError(std::string& out_message) const -> void;

    /////////////////////////DEBUG//////////////////////////////
    auto checkName(const std::string& name) -> const std::string;
    /////////////////////////DEBUG//////////////////////////////
};
