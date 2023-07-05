#pragma once
#include <imgui.h>
#include <string>
#include <thread>
#include <vector> 


namespace MyApp
{
    class Client
    {
    public:
        std::vector<std::string>messages;
        Client(const std::string& serverIP, int serverPort);
        void Connect();
        void Disconnect();
        void SendMessageToServer(const std::string& message);
        void RenderUI();

    private:
        std::string serverIP;
        int serverPort;
        bool isRunning;
        std::thread receiveThread;
        char buffer[4096];
        int bytesRead;
        void ReceiveMessagesFromServer();
    };
}
