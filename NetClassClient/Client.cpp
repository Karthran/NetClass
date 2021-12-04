#include "Client.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment(lib, "AdvApi32.lib")

const char* DEFAULT_PORT = "27777";

#elif defined __linux__
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int DEFAULT_PORT = 27777;  // �
#endif  //  _WIN32

const int DEFAULT_BUFLEN = 1024;

volatile bool out_message_ready{false};
volatile bool in_message_ready{false};
volatile bool server_error{false};
std::string message{};

#ifdef _WIN32

int client_thread()
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    // const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    while (true)
    {
        // Send an initial buffer
        // std::string msg("this is a test");
        // std::cin >> msg;
        while (!out_message_ready)
        {
        }

        // std::cout <<"OutMessage: " << message << std::endl;

        std::copy(message.begin(), message.end(), sendbuf);
        iResult = send(ConnectSocket, sendbuf, message.size(), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            server_error = true;
            return 1;
        }
        out_message_ready = false;
        // printf("Bytes Sent: %ld\n", iResult);

        if (message == "end")
        {
            // shutdown the connection since no more data will be sent
            iResult = shutdown(ConnectSocket, SD_SEND);
            if (iResult == SOCKET_ERROR)
            {
                printf("shutdown failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                server_error = true;
                return 1;
            }
        }
        // Receive until the peer closes the connection
        //       do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            // printf("Bytes received: %d\n", iResult);
            message = std::string(recvbuf, iResult);
            in_message_ready = true;

            // std::cout << "InMessage: " << message << std::endl;
        }
        else if (iResult == 0)
        {
            printf("Connection closed\n");
            break;
        }
        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            break;
        }

        //       } while (iResult > 0);
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

#elif defined __linux__
int client_thread()
{

    int socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;

    char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // �������� �����
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        std::cout << "Creation of Socket failed!" << std::endl;
        exit(1);
    }

    // ��������� ����� �������
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // ������� ����� �����
    serveraddress.sin_port = htons(DEFAULT_PORT);
    // ���������� IPv4
    serveraddress.sin_family = AF_INET;
    // ��������� ���������� � ��������
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1)
    {
        std::cout << "Connection with the server failed.!" << std::endl;
        exit(1);
    }
    // �������������� � ��������
    while (1)
    {
        while (!out_message_ready)
        {
        }

        bzero(sendbuf, sizeof(sendbuf));
        std::copy(message.begin(), message.end(), sendbuf);

        ssize_t bytes = write(socket_file_descriptor, sendbuf, message.size());
        if ((strncmp(sendbuf, "end", 3)) == 0)
        {
            write(socket_file_descriptor, sendbuf, message.size());
            std::cout << "Client Exit." << std::endl;
            server_error = true;
            break;
        }
        // ���� �������� >= 0  ����, ������ ��������� ������ �������
        if (bytes >= 0)
        {
            out_message_ready = false;
        }
        bzero(sendbuf, sizeof(sendbuf));
        // ���� ������ �� �������
        ssize_t length = read(socket_file_descriptor, sendbuf, sizeof(sendbuf));
        if (length > 0)
        {
            message = std::string(sendbuf, length);
            in_message_ready = true;
        }
    }
    // ��������� �����, ��������� ����������
    close(connection);
    return 0;
}

#endif  // _WIN32

auto Client::run() -> void
{
    std::thread tr(&client_thread);
    tr.detach();
    //bool loop_flag = true;
    //while (loop_flag)
    //{
    //    //while (out_message_ready)
    //    //{
    //    //}
    //    //// std::cin >> message;
    //    //std::getline(std::cin, message);
    //    //out_message_ready = true;
    //    //if (message == "end")
    //    //{
    //    //    loop_flag = false;
    //    //}
    //    //while (!in_message_ready)
    //    //{
    //    //    if (server_error)
    //    //    {
    //    //        loop_flag = false;
    //    //        break;
    //    //    }
    //    //}
    //    //std::cout << message << std::endl;
    //    //in_message_ready = false;
    //}
    //std::cout << "Client stop!" << std::endl;
    //auto res{0};
    //std::cin >> res;
    //return;
}

auto Client::getOutMessageReady()const -> bool
{
    return out_message_ready;
}

auto Client::setOutMessageReady(bool flag)const -> void
{
    out_message_ready = flag;
}

auto Client::getInMessageReady()const -> bool
{
    return in_message_ready;
}

auto Client::setInMessageReady(bool flag)const -> void
{
    in_message_ready = flag;
}

auto Client::getMessage() -> const std::string&
{
    return message;
}

auto Client::setMessage(const std::string& msg)const -> void 
{
    message = msg;
}

auto Client::getServerError()const -> bool
{
    return server_error;
}
