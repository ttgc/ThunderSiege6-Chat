#include "ThunderChatClient.hpp"


namespace client 
{
    ThunderChatClient::ThunderChatClient(const std::string& ip, const std::string& pseudo, const std::string& team) noexcept
    {

    }
    ThunderChatClient::~ThunderChatClient() noexcept
    {

    }
    ThunderChatClient::ThunderChatClient(ThunderChatClient&& other) noexcept
    {

    }

    bool Connect(const std::string& ip) noexcept
    {

    }

    void OnMessage(const CallbackMsg& msg) noexcept
    {
        m_messageCallback.push_back(msg);
    }

    void OnDisconnect(const CallbackDeco& deco) noexcept
    {
        m_disconnectCallback.push_back(deco);
    }

    void SendToParty(const std::string& msg)
    {

    }

    void SendToTeam(const std::string& msg)
    {

    }

}
