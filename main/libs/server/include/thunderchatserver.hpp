#pragma once

#include <string>
#include <functional>
#include <array>
#include <vector>
#include <thread>
#include <atomic>
#include "network.hpp"
#include "connexion.hpp"

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

	private:
		void run();

		std::vector<CallbackType> m_connectCallback;
		std::vector<CallbackType> m_disconnectCallback;

		std::unique_ptr<std::thread> m_serverThread;
		std::atomic_bool m_running;
		uint32_t m_returnCode;

		Connexion m_serverSocket;
		std::array<Connexion, 10> m_clients;
	};
}