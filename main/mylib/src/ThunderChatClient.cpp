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
        : m_ip(ip)
        , m_username(username)
        , m_team(team)
        , m_s(socket(AF_INET, SOCK_STREAM, 0))
        , m_clientAddr()
        , m_clientAddrSize(sizeof(sockaddr))
        , m_client()
        , m_addrv4()
        , m_messageCallback()
        , m_disconnectCallback()
        , m_running(false)
        , m_clientThread(nullptr)
    {
        if (m_s < 0)
        {
            std::cout << "Error";
            return;
        }
        
        m_addrv4.sin_family = AF_INET;
        m_addrv4.sin_port = htons(8888);
        if (inet_pton(AF_INET, ip.c_str(), &(m_addrv4.sin_addr)) < 0)
        {
            std::cout << "Error";
            return;
        }

        m_running = true;
        m_clientThread = std::make_unique<std::thread>([this]() { run(); });
    }

    ThunderChatClient::~ThunderChatClient() noexcept
    {
        m_running = false;
        OnDisconnect(std::function<void()>); // QUESTION
        shutdown(m_s, SD_BOTH);
        closesocket(m_s);
    }

    // parametre faux, mettre la socket ?
    bool ThunderChatClient::Connect() noexcept
    {
        if (connect(m_s, reinterpret_cast<sockaddr*>(&m_addrv4), m_clientAddrSize))
        {
            SendToTeam("Tentative Connection");
            std::cout << "Client is connected." << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }

    void ThunderChatClient::OnMessage(const CallbackMsg& msg) noexcept
    {
        m_messageCallback.push_back(msg);
    }

    void ThunderChatClient::OnDisconnect(const CallbackDeco& deco) noexcept
    {
        m_disconnectCallback.push_back(deco);
    }

    void ThunderChatClient::SendToParty(const std::string& rawMessage)
    {
        network::message::Message msg(m_username, network::message::GLOBAL, m_team, rawMessage);
        if (msg.isCorrectlySized())
        {
            std::string message = msg.getJsonMessage().dump();
            send(m_s, message.c_str(), message.size(), 0);
        }
        else
        {
            std::cout << "Message au mauvais format\n";
        }
    }

    void ThunderChatClient::SendToTeam(const std::string& rawMessage)
    {
        network::message::Message msg(m_username, network::message::TEAM, m_team, rawMessage);
        if (msg.isCorrectlySized())
        {
            std::string message = msg.getJsonMessage().dump();
            auto a = send(m_s, message.c_str(), message.size(), 0);
        }
        else
        {
            std::cout << "Message au mauvais format\n";
        }
    }

    void ThunderChatClient::run() noexcept
    {
        while (m_running)
        {
            std::array<char, 1024> buffer;
            int length_msg = recv(m_s, buffer.data(), 1024, 0);
            if(length_msg < 0)
            {
                std::cout << "Error" << std::endl;
                return;
            }
            else
            {
                std::string received(buffer.data(), length_msg);
                auto msg = network::Message::getMessageFromJson(received);
                if(msg.has_value() && msg.isCorrectlySized())
                {
                    OnMessage(msg);
                    std::string username(msg.username);
                    std::string team;
                    std::string message;
                    if(msg.msg_type)
                    {   
                        std::string type(" (to Team ");
                        if(msg.team)    {   std::string team = "B) : ";  }
                        else            {   std::string team = "A) : ";  }
                        std::string message = username + type + team + msg.message;
                    }
                    else
                    {
                        std::string type(" (to Party) : ");
                        std::string message = username + type + msg.message;
                    }
                    std::cout << message << std::endl;
                }
            }
        }
    }
}