#pragma once
#include <vector>
#include <thread>
#include <string>

const int DEFAULT_BUFLEN = 1024;

class Application;
class Server
{
public:
    Server(Application* app);
    ~Server();
    auto run() -> void;
    auto setContinueFlag(bool flag) -> void;
    auto setBufferSize(int index, size_t size) -> void;
    auto setCashMessage(const std::string& msg, int thread_num) -> void;
    auto getCashMessage(int thread_num) const -> const std::string&;

private:
    Application* _app;
    bool _continue_flag{true};

    std::vector<std::thread> clients;
    std::vector<std::string> cash_message{};
    std::vector<size_t> buffer_size{};
    std::vector<bool> need_buffer_resize{};
    std::vector<std::string> in_message{};
    std::vector<bool> in_message_ready{};
    std::vector<std::string> out_message{};
    std::vector<bool> out_message_ready{};

    int thread_count{0};
    volatile bool continue_flag{true};

#ifdef _WIN32
    auto server_thread(int thread_number) -> void;
#elif defined __linux__
    auto client_loop(int thread_number, int connection) -> void;
    auto server_thread() -> int;
#endif
    auto main_loop(Application* app) -> void;
};
