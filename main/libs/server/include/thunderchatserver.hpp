#pragma once

#include <string>
#include <functional>
#include <array>
#include <vector>

namespace server
{
	class ThunderChatServer
	{
	public:
		using CallbackType = std::function<void(const std::string& client);

		ThunderChatServer(const std::string& ip, int port) noexcept;
		virtual ~ThunderChatServer() noexcept;
		ThunderChatServer(ThunderChatServer&& other) noexcept;

		ThunderChatServer(const ThunderChatServer& other) = delete;
		ThunderChatServer& operator=(const ThunderChatServer& rhs) = delete;

		void onConnect(const CallbackType& callback);
		void onDisconnect(const CallbackType& callback);

		void stop();

	private:
		std::vector<CallbackType> m_connectCallback;
		std::vector<CallbackType> m_disconnectCallback;

		//std::array<Connexion, 10> m_clients;
	};
}