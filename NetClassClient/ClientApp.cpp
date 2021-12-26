#include <iostream>
#include <string.h>

#include "ClientApp.h"
#include "Client.h"
#include "../core.h"

ClientApp::ClientApp() : _msg_buffer_size (DEFAULT_BUFLEN)
{
    _msg_buffer = new char[DEFAULT_BUFLEN];
}
ClientApp::~ClientApp()
{
    delete[] _msg_buffer;
}

auto ClientApp::run() -> void
{
    _client = new Client();
    _client->run();
    loop();
}

//auto ClientApp::sendToServer(std::string& message, OperationCode operation_code) const -> void
auto ClientApp::sendToServer(const char* message, size_t message_length, OperationCode operation_code) -> void
{
    if (_msg_buffer_size < message_length + HEADER_SIZE)
    {
        _msg_buffer_size = message_length + HEADER_SIZE;
        _client->setBufferSize(_msg_buffer_size); 
        delete[] _msg_buffer;
        _msg_buffer = new char[_msg_buffer_size];
    }
    _current_msg_length = 0;

    addToBuffer(_msg_buffer, _current_msg_length, static_cast<int>(OperationCode::CHECK_SIZE));
    addToBuffer(_msg_buffer, _current_msg_length, message_length);

    auto receive_buf{talkToServer(_msg_buffer, _current_msg_length)};

    //int first_value{-1};
    //getFromBuffer(receive_buf, 0, first_value);
    //int second_value{-1};
    //getFromBuffer(receive_buf, sizeof(first_value), second_value);
    //std::cout << first_value << "_" << second_value << std::endl;

    _current_msg_length = 0;

    addToBuffer(_msg_buffer, _current_msg_length, static_cast<int>(OperationCode::CHECK_NAME));
    addToBuffer(_msg_buffer, _current_msg_length, static_cast<int>(OperationCode::CHECK_SIZE));
    addToBuffer(_msg_buffer, _current_msg_length, message, message_length);

    receive_buf = talkToServer(_msg_buffer, _current_msg_length);

    auto message_size{-1};
    getFromBuffer(receive_buf, sizeof(int), message_size);

    std::cout << "Message Size: " << message_size << std::endl;


   _client->setBufferSize(message_size + HEADER_SIZE);

   _current_msg_length = 0;

   addToBuffer(_msg_buffer, _current_msg_length, static_cast<int>(OperationCode::CHECK_NAME));
   addToBuffer(_msg_buffer, _current_msg_length, static_cast<int>(OperationCode::READY));

   receive_buf = talkToServer(_msg_buffer, _current_msg_length);

   char* str_ptr = new char[message_size + 1];

   getFromBuffer(receive_buf, 0, str_ptr, message_size);

   str_ptr[message_size] = '\0';

   std::cout << str_ptr << std::endl;

   delete[] str_ptr;

    //addToBuffer(_msg_buffer, _current_msg_length, message, message_length);



    //msg_to_srv = std::to_string(static_cast<int>(OperationCode::CHECK_NAME)) + " " +
    //             std::to_string(static_cast<int>(OperationCode::CHECK_SIZE)) + " " + message;
    //msg_from_srv = talkToServer(msg_to_srv);

    //auto msg_size{std::stoi(std::string(msg_from_srv, 2))};

    //_client->setBufferSize(msg_size + HEADER_SIZE);

    //msg_to_srv = std::to_string(static_cast<int>(OperationCode::CHECK_NAME)) + " " +
    //             std::to_string(static_cast<int>(OperationCode::READY)) + " " + message;

    //message = talkToServer(msg_to_srv);
    //return

}

//auto ClientApp::talkToServer(const std::string& message) const -> const std::string& 
auto ClientApp::talkToServer(const char* message, size_t msg_length) const -> const char*
{
    while (_client->getOutMessageReady())
    {
    }
    //std::cout << message << std::endl;
    _client->setMessage(message, msg_length);
    _client->setOutMessageReady(true);
    while (!_client->getInMessageReady())
    {
        if (_client->getServerError())
        {
            break;
        }
    }
    _client->setInMessageReady(false);
    return _client->getMessage();
}

auto ClientApp::addToBuffer(char* buffer, size_t& cur_msg_len, int value)const -> void
{
    auto length{sizeof(value)};
    auto char_ptr{reinterpret_cast<char*>(&value)};

    for (auto i{0}; i < length ; ++i)
    {
        buffer[i + cur_msg_len] = char_ptr[i];
    }
    cur_msg_len += length;
}

auto ClientApp::addToBuffer(char* buffer, size_t& cur_msg_len, const char* string, size_t str_len)const -> void
{
    for (auto i{0}; i < str_len; ++i)
    {
        buffer[i + cur_msg_len] = string[i];
    }
    cur_msg_len += str_len;
}

auto ClientApp::getFromBuffer(const char* buffer, size_t shift, int& value)const -> void
{
    char val_buff[sizeof(value)];
    auto length{sizeof(value)};

    for (auto i{0}; i < length; ++i)
    {
        val_buff[i] = buffer[shift + i];
    }
    value = *(reinterpret_cast<int*>(val_buff));
}

auto ClientApp::getFromBuffer(const char* buffer, size_t shift, char* string, size_t str_len)const -> void 
{
    for (auto i{0}; i < str_len; ++i)
    {
        string[i] = buffer[shift + i];
    }
}

auto ClientApp::loop() -> void
{
    std::string message;
    bool loop_flag = true;
    while (loop_flag)
    {
        std::getline(std::cin, message);
        if (message == "end")
        {
            talkToServer("0", 1);  // TODO std::to_string(static_cast<int>(OperationCode::STOP))
            loop_flag = false;
            break;
        }
        sendToServer(message.c_str(),message.size(), OperationCode::CHECK_NAME);

        std::cout << message << std::endl;
    }
    std::cout << "Client stop!" << std::endl;
    return;
}
