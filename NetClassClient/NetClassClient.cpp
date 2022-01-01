
#include <iostream>
#include "ClientApp.h"

#ifdef _WIN32
#include <cstdio>
#include <windows.h>
#pragma execution_character_set("utf-8")
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // UTF8
#endif

    ClientApp client_app;
    client_app.run();
}
