#include "ThunderChatClient.hpp"
#include "network.hpp"
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace client
{

ThunderChatClient::ThunderChatClient(const std::string& ip, const std::string& username,
                                     const network::message::Team & team) noexcept
    : m_ip(ip), m_username(username), m_team(team), m_s(socket(AF_INET, SOCK_STREAM, 0))
{
    if (m_s < 0)
    {
        std::cout << "Error";
        return;
    }

    sockaddr_in addrv4;
    addrv4.sin_family = AF_INET;
    addrv4.sin_port = htons(8888);
    if (inet_pton(AF_INET, ip.c_str(), &(addrv4.sin_addr)) < 0)
    {
        std::cout << "Error";
        return;
    }

    sockaddr clientAddr;
    socklen_t clientAddrSize = 0;

    bool res(true);


    std::array<char, 512> ipClientStr;

    // Changer le message avec le username et le tag de team
    std::cout << "Client " << inet_ntop(clientAddr.sa_family, &clientAddr, ipClientStr.data(), 512)
              << " is connected." << std::endl;

    // pas de bind ni de listen

    // param ?
    res = Connect(m_s, clientAddr, clientAddrSize);

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


    shutdown(m_s, SD_BOTH);
    closesocket(m_s);
}

ThunderChatClient::ThunderChatClient(ThunderChatClient&& other) noexcept {}

// parametre faux, mettre la socket ?
bool ThunderChatClient::Connect(const SOCKET& s, const sockaddr& clientAddr,
                                socklen_t clientAddrSize) noexcept
{
    if (connect(s, &clientAddr, clientAddrSize))
    {
        SendToTeam("test");
        return true;
    }
    else
    {
        return false;
    }
}

void ThunderChatClient::OnDisconnect(const CallbackDeco& deco) noexcept
{
    m_disconnectCallback.push_back(deco);
}

/* où sont saisies les messages ? comment on les récupères ?
enum MessageType {GLOBAL = 0, TEAM = 1};
enum Team {TEAM_A = 0, TEAM_B = 1}; */
void ThunderChatClient::SendToParty(const std::string& rawMessage)
{
    /*std::string message;
    std::cin >> message;*/

    network::message::Message msg(m_username, network::message::GLOBAL, m_team, rawMessage);
    if (msg.isCorrectlySized())
    {
        std::string message = msg.getJsonMessage().dump();
        send(m_s, message.c_str(), message.size, 0);
    }
    else
    {
        std::cout << "Message au mauvais format\n";
    }
}

void ThunderChatClient::SendToTeam(const std::string& rawMessage)
{
//    std::string message;
//    std::cin >> message;
    network::message::Message msg(m_username, network::message::TEAM, m_team, rawMessage);
    if (msg.isCorrectlySized())
    {
        std::string message = msg.getJsonMessage().dump();
        send(m_s, message.c_str(), message.size, 0);
    }
    else
    {
        std::cout << "Message au mauvais format\n";
    }
}
bool ThunderChatClient::Connect(const SOCKET& s, const sockaddr& clientAddr,
                                socklen_t clientAddrSize) noexcept
{
    return false;
}
void ThunderChatClient::OnMessage(const CallbackMsg& msg) noexcept
{
    m_messageCallback.push_back(msg);
}
} // namespace client