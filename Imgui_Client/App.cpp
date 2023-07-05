#include "App.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
namespace MyApp
{
    
    
        SOCKET clientSocket;
        Client::Client(const std::string& serverIP, int serverPort) {};

        void Client::Connect() {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                std::cerr << "Failed to initialize winsock" << std::endl;
                return;
            }

            clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Failed to create socket" << std::endl;
                WSACleanup();
                return;
            }
            wchar_t ipAddress[] = L"127.0.0.2";
            sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(8080);
            InetPton(AF_INET, ipAddress, &(serverAddress.sin_addr)); // Server IP address

            if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
                std::cerr << "Failed to connect to server" << std::endl;
                closesocket(clientSocket);
                WSACleanup();
                return;
            }

            std::cout << "Connected to server. Enter messages to send." << std::endl;
            
            receiveThread = std::thread(&Client::ReceiveMessagesFromServer, this);
        }

        void Client::Disconnect() {
            // Implement the code to disconnect from the server
            // ...
            isRunning = false;

            // Join the receive thread before exiting
            if (receiveThread.joinable()) {
                receiveThread.join();
            }
        }

        void Client::SendMessageToServer(const std::string& message) {
            // Implement the code to send a message to the server
            // ...
            if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
                std::cerr << "Failed to send data to server" << std::endl;
                return;
            }
            std::cout << "Sent message to server: " << message << std::endl;
        }
        void Client::RenderUI()
        {
            
            static char inputText[256] = "";

            ImGui::SetNextWindowSize(ImVec2(600, 400)); // Set the window size to 400x300
            // ImGui::Begin("Chat App", nullptr, ImGuiWindowFlags_NoResize);

            ImGui::Begin("Chat App");

            // Display received messages
            for (const auto& message : messages) {
                ImGui::TextWrapped("%s", message.c_str());
            }

            ImGui::Separator();

            // Input field
            ImGui::InputText("Message", inputText, sizeof(inputText));

            ImGui::SameLine();

            // Send button
            if (ImGui::Button("Send") && std::strlen(inputText) > 0) {
                std::string message = inputText;
                messages.push_back(message);
                SendMessageToServer(message);
                std::memset(inputText, 0, sizeof(inputText));
            }

            ImGui::End();
        }
        
        void Client::ReceiveMessagesFromServer() {
            while (1) 
                {
                    memset(buffer, 0, sizeof(buffer));
                    bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                    std::string message = buffer;
                    
                    if (bytesRead == SOCKET_ERROR) {
                        std::cerr << "Error in receiving data from server" << std::endl;
                        break;
                    }
                    else if (bytesRead == 0) {
                        std::cout << "Server connection closed" << std::endl;
                        break;
                    }
                    else {
                        std::string message(buffer);
                        //std::cout << "Received message from server: " << message << std::endl;
                        messages.push_back(message);
                    }
                }
        }
 
    /*void RenderUI()
    {
        static std::vector<std::string> messages;
        static char inputText[256] = "";

        ImGui::SetNextWindowSize(ImVec2(600, 400)); // Set the window size to 400x300
       // ImGui::Begin("Chat App", nullptr, ImGuiWindowFlags_NoResize);
       
        ImGui::Begin("Chat App");

        // Display received messages
        for (const auto& message : messages) {
            ImGui::TextWrapped("%s", message.c_str());
        }

        ImGui::Separator();

        // Input field
        ImGui::InputText("Message", inputText, sizeof(inputText));

        ImGui::SameLine();

        // Send button
        if (ImGui::Button("Send") && std::strlen(inputText) > 0) {
            std::string message = inputText;
            SendMessageToServer(message);
            std::memset(inputText, 0, sizeof(inputText));
        }

        ImGui::End();
    }*/

}
