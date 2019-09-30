#pragma once

#include <string>
#include <functional>
#include <array>
#include <vector>
#include <thread>
#include <atomic>
#include "network.hpp"
#include "connexion.hpp"
#include "clientdata.hpp"

namespace server
{
	class ThunderChatServer
	{
	public:
		using CallbackType = std::function<void(const std::string& client)>;

		ThunderChatServer(const std::string& ip, uint16_t port) noexcept;
		virtual ~ThunderChatServer() noexcept;
		ThunderChatServer(ThunderChatServer&& other) noexcept = default;

		ThunderChatServer(const ThunderChatServer& other) = delete;
		ThunderChatServer& operator=(const ThunderChatServer& rhs) = delete;

		void onConnect(const CallbackType& callback) noexcept;
		void onDisconnect(const CallbackType& callback) noexcept;

		void stop() noexcept;

		bool isRunning() const noexcept { return m_running; }
		std::string getIP() const noexcept { return m_serverSocket.getIP(); }
		uint16_t getPort() const noexcept { return m_serverSocket.getPort(); }
		uint32_t getErrorCode() const noexcept { return m_returnCode; }

	private:
		void acceptCallback() noexcept;
		void run() noexcept;

		std::vector<CallbackType> m_connectCallback;
		std::vector<CallbackType> m_disconnectCallback;

		std::unique_ptr<std::thread> m_acceptThread;
		std::unique_ptr<std::thread> m_serverThread;
		std::atomic_bool m_running;
		uint32_t m_returnCode;

		network::NonBlockingConnexion m_serverSocket;
		std::array<ClientData, 10> m_clients;
	};
}