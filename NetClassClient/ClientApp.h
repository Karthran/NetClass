#pragma once
#include <string>

class Client;
enum class OperationCode;

class ClientApp
{
public:
    ClientApp();
    ~ClientApp();

    auto run() -> void;
    //   auto sendToServer(std::string& message, OperationCode operation_code) const -> void;
    auto sendToServer(const char* message, size_t message_length, OperationCode operation_code) -> const char*;

private:
    Client* _client{nullptr};
    char* _msg_buffer{nullptr};
    size_t _msg_buffer_size{0};
    size_t _current_msg_length{0};
    auto loop() -> void;
    // auto talkToServer(const std::string& message) const -> const std::string&;
    auto talkToServer(const char* message, size_t msg_length) const -> char*;

    auto addToBuffer(char* buffer, size_t& cur_msg_len, int value)const -> void;
    auto addToBuffer(char* buffer, size_t& cur_msg_len, const char* string, size_t str_len)const -> void;

    auto getFromBuffer(const char* buffer, size_t shift, int& value)const -> void;
    auto getFromBuffer(const char* buffer, size_t shift, char* string, size_t str_len)const -> void;
};
