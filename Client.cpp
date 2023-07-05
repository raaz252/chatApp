#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<string>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    SOCKET clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }
    wchar_t ipAddress[] = L"127.0.0.1";
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    InetPton(AF_INET, ipAddress, &(serverAddress.sin_addr)); // Server IP address

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server. Enter messages to send." << std::endl;

    while (true) {
        // Prompt the client to enter a message
        std::cout << "> ";
        std::string message;
        std::getline(std::cin, message);

        // Send the message to the server
        if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send data to server" << std::endl;
            break;
        }

        // Receive the response from the server
        char serverMessage[4096];
        int bytesRead = recv(clientSocket, serverMessage, sizeof(serverMessage) - 1, 0);
        if (bytesRead == SOCKET_ERROR) {
            std::cerr << "Error in receiving data from server" << std::endl;
            break;
        }
        else if (bytesRead == 0) {
            std::cout << "Server disconnected" << std::endl;
            break;
        }
        else {
            // Null-terminate the received message
            serverMessage[bytesRead] = '\0';

            // Print the received message from the server
            std::cout << serverMessage << std::endl;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
