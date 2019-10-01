//#include "ThunderChatClient.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>

using SOCKET = int;
#define SD_BOTH SHUT_RDWR
#define closesocket(s) close(s)


/*
                "/usr/include/c++/7",
                "/usr/include/x86_64-linux-gnu/c++/7",
                "/usr/include/c++/7/backward",
                "/usr/lib/gcc/x86_64-linux-gnu/7/include",
                "/usr/local/include",
                "/usr/lib/gcc/x86_64-linux-gnu/7/include-fixed",
                "/usr/include/x86_64-linux-gnu",
                "/usr/include",
                "/usr/include/linux"
*/

namespace client 
{
    ThunderChatClient::ThunderChatClient(const std::string& ip, const std::string& username, const std::string& team) noexcept
    {
        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        if(s < 0)
        {
            std::cout << "Error";
            return EXIT_FAILURE;
        }        

        sockaddr_in addrv4;
        addrv4.sin_family = AF_INET;
        addrv4.sin_port = htons(8888);
        if(inet_pton(AF_INET, ip, &(addrv4.sin_addr)) < 0)
        {
            std::cout << "Error";
            return EXIT_FAILURE;   
        }



        sockaddr clientAddr;
        socklen_t clientAddrSize = 0;
        SOCKET client = accept(s, &clientAddr, &clientAddrSize);

        if(client < 0)
        {
            res = true;
            std::cout << "Error";
            closesocket(s);
            return false;
        }

        std::array<char, 512> ipClientStr;

        // Changer le message avec le username et le tag de team
        std::cout << "Client " << inet_ntop(clientAddr.sa_family, &clientAddr, ipClientStr.data(), 512) << " is connected." << std::endl;

    //pas de bind ni de listen

        bool res(true);
//param ?
        res = Connect(s, clientAddr, clentAddrSize);

/*
        std::array<char, 1024> buffer;
        int receivedBytes = recv(s, buffer.data(), 1024, 0);
        if(receivedBytes < 0)
        {
            std::cout << "Error" << std::endl;
            closesocket(s);
            return EXIT_FAILURE;   
        } else {
            // Sous quel format le message a-t-il été reçu ? et comment l'extraire ?
            std::string received(buffer.data(), length);

//            auto msg = network::message::Message.getMessageFromJson(received);
//            OnMessage(msg);
        }*/
    }

    ThunderChatClient::~ThunderChatClient() noexcept
    {
        shutdown(client, SD_BOTH);
        closesocket(client);

        shutdown(s, SD_BOTH);
        closesocket(s);
    }
    
    ThunderChatClient::ThunderChatClient(ThunderChatClient&& other) noexcept
    {

    }

//parametre faux, mettre la socket ?
    bool Connect(const SOCKET s, const sockaddr clientAddr, socklen_t clientAddrSize) noexcept
    {
        if(connect(s, clientAddr, clientAddrSize))
        {
            SendToTeam("test");
            return true;
        } else {
            return false;
        }
        
    }

    void OnMessage(const CallbackMsg& msg) noexcept
    {
        m_messageCallback.push_back(msg);
    }

    void OnDisconnect(const CallbackDeco& deco) noexcept
    {
        m_disconnectCallback.push_back(deco);
    }

/* où sont saisies les messages ? comment on les récupères ?
enum MessageType {GLOBAL = 0, TEAM = 1};
enum Team {TEAM_A = 0, TEAM_B = 1}; */
    void SendToParty(const std::string& msg)
    {
        std::string message;
        std::cin >> message;
        network::message::Message msg(&username, 0, &team, message);
        if(msg.isCorrectlySized())
        {
            msg.getJsonMessage().dump();
            std::string(buffer.begin(), receivedBytes).c_str();
            send(s, buffer.data(), receivedBytes, 0);
        } else {
            std::cout << "Message au mauvais format\n";
        }

    }

    void SendToTeam(const std::string& msg)
    {
        std::string message;
        std::cin >> message;
        network::message::Message msg(&username, 1, &team, message);
        if(msg.isCorrectlySized())
        {
            msg.getJsonMessage().dump();
            std::string(buffer.begin(), receivedBytes).c_str();
            send(s, buffer.data(), receivedBytes, 0);
        } else {
            std::cout << "Message au mauvais format\n";
        }
    }
}