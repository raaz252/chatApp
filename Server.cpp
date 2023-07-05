
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

// Data structure to store connected client sockets
std::vector<SOCKET> connectedClients;

// Function to handle individual client connections
void handleClient(SOCKET clientSocket)
{
    char clientIP[INET_ADDRSTRLEN];
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);

    getpeername(clientSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
    std::cout << "Client connected: " << clientIP << std::endl;

    // Add the client socket to the connected clients list
    connectedClients.push_back(clientSocket);

    char message[4096];

    while (true) {
        int bytesRead = recv(clientSocket, message, sizeof(message) - 1, 0);
        if (bytesRead == SOCKET_ERROR) {
            std::cerr << "Error in receiving data from client" << std::endl;
            break;
        }
        else if (bytesRead == 0) {
            std::cout << "Client disconnected: " << clientIP << std::endl;
            break;
        }
        else {
            // Broadcast the message to all connected clients
            
            message[bytesRead] = '\0';
            std::string nMessage;
            //nMessage.reserve(4096 + INET_ADDRSTRLEN);
            nMessage += clientIP;
            nMessage += ":";
            nMessage += message;
            std::cout << clientIP << "{" << message << "}" << std::endl;
            for (const auto& socket : connectedClients) {
                if (socket != clientSocket) {
                    send(socket, nMessage.c_str(),int(nMessage.length()), 0);
                }
            }
        }
    }

    // Remove the client socket from the connected clients list
    auto it = std::find(connectedClients.begin(), connectedClients.end(), clientSocket);
    if (it != connectedClients.end()) {
        connectedClients.erase(it);
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    SOCKET serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started, waiting for connections..." << std::endl;

    while (true) {
        SOCKET clientSocket;
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Start a new thread to handle the client connection
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Detach the thread and allow it to run independently
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
