#pragma once

#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include "message.hpp"

namespace client
{
    class ThunderChatClient 
    {
        public:
            using CallbackMsg = std::function<void(const network::message::Message& msg)>;
            using CallbackDeco = std::function<void()>;

            ThunderChatClient(const std::string& ip, const std::string& pseudo, const std::string& team) noexcept;
		    virtual ~ThunderChatClient() noexcept;
		    ThunderChatClient(ThunderChatClient&& other) noexcept;

            ThunderChatClient(const ThunderChatClient& other) = delete;
		    ThunderChatClient& operator=(const ThunderChatClient& rhs) = delete;

            bool Connect(const std::string& ip) noexcept;

            void OnMessage(const CallbackMsg& msg) noexcept;

            void OnDisconnect(const CallbackDeco& deco) noexcept;

            void SendToParty(const std::string& msg);

            void SendToTeam(const std::string& msg);

        private:
            std::vector<CallbackMsg> m_messageCallback;
            std::vector<CallbackDeco> m_disconnectCllback;
    };
}