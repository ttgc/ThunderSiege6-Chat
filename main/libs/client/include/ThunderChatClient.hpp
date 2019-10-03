#pragma once

#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include <string>
#include <thread>

#include "message.hpp"
#include "network.hpp"
#include "dnsresolver.hpp"
#include <atomic>

namespace client
{
    class ThunderChatClient 
    {
        public:
            using CallbackMsg = std::function<void(const network::message::Message& msg)>;
            using CallbackDeco = std::function<void()>;

            ThunderChatClient(const std::string& ip, const std::string& username,
                              const network::message::Team & team) noexcept;
		    virtual ~ThunderChatClient() noexcept;
		    ThunderChatClient(ThunderChatClient&& other) noexcept;

            ThunderChatClient(const ThunderChatClient& other) = delete;
		    ThunderChatClient& operator=(const ThunderChatClient& rhs) = delete;

            bool Connect() noexcept;

            void OnMessage(const CallbackMsg& msg) noexcept;

            void OnDisconnect(const CallbackDeco& deco) noexcept;

            void SendToParty(const std::string& msg);

            void SendToTeam(const std::string& msg);

			bool isRunning();

        private:
            void run() noexcept;
            std::string m_ip;
			std::string m_username;
            network::message::Team m_team;
            SOCKET m_s;
            sockaddr m_clientAddr;
            socklen_t m_clientAddrSize;
            SOCKET m_client;
            sockaddr_in m_addrv4;
			std::vector<CallbackMsg> m_messageCallback;
            std::vector<CallbackDeco> m_disconnectCallback;
            std::atomic_bool m_running;
			std::unique_ptr<std::thread> m_clientThread;
    };
}