#pragma once
#include "DataBase.h"

class Server;
class DataBase;
class Application
{
public:
    auto run() -> void;
    auto reaction(char * message, int thread_num) -> void;

private:
    Server* _server{nullptr};
    std::unique_ptr<DataBase> _data_base{nullptr};

    auto onCheckSize(char* message, int thread_num) const -> void;
    auto onCheckName(char* message, int thread_num) -> void;
    auto onStop(char* message, int thread_num) -> void;
    auto onError(char* message, int thread_num) const -> void;

    auto addToBuffer(char* buffer, size_t& cur_msg_len, int value) const -> void;
    auto addToBuffer(char* buffer, size_t& cur_msg_len, const char* string, size_t str_len) const -> void;

    auto getFromBuffer(const char* buffer, size_t shift, int& value) const -> void;
    auto getFromBuffer(const char* buffer, size_t shift, char* string, size_t str_len) const -> void;

    /////////////////////////DEBUG//////////////////////////////
    auto checkName(char* name, size_t name_size, int thread_num) -> void;
    /////////////////////////DEBUG//////////////////////////////
};
