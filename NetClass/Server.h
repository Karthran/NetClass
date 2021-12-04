#pragma once
#include <vector>
#include <thread>
#include <string>

const int DEFAULT_BUFLEN = 1024;

class Application;
class Server
{
public:
    Server(Application* app) : _app(app) {}
    ~Server(){};
    auto run() -> void;
    auto setContinueFlag(bool flag) -> void;

private:
    Application* _app;
    bool _continue_flag{true};
};

