#pragma once
#include <string>

const int DEFAULT_BUFLEN = 1024;

class Client
{
public:
	auto run() -> void;
    auto getOutMessageReady()const -> bool;
    auto setOutMessageReady(bool flag) -> void;
    auto getInMessageReady()const -> bool;
    auto setInMessageReady(bool flag) -> void;
    auto getMessage() -> const char*;
    auto setMessage(const char* msg, size_t msg_length) -> void;
    auto getServerError()const -> bool;
    auto setBufferSize(size_t size)  -> void;

private:
    volatile bool out_message_ready{false};
    volatile bool in_message_ready{false};
    volatile bool server_error{false};
    char* recvbuf{nullptr};
    std::string message{};
    volatile size_t buffer_size{DEFAULT_BUFLEN};
    volatile bool need_buffer_resize{true};
    size_t message_length{0};

    auto client_thread() -> int;
};

