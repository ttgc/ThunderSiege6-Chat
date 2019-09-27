#include "thunderchatserver.hpp"


namespace server
{
	ThunderChatServer::ThunderChatServer(const std::string& ip, int port) noexcept
	{
	}

	ThunderChatServer::~ThunderChatServer() noexcept
	{
		stop();
	}

	ThunderChatServer::ThunderChatServer(ThunderChatServer&& other) noexcept
	{
	}

	void ThunderChatServer::onConnect(const CallbackType& callback)
	{
		m_connectCallback.push_back(callback);
	}

	void ThunderChatServer::onDisconnect(const CallbackType& callback)
	{
		m_disconnectCallback.push_back(callback);
	}

	void ThunderChatServer::stop()
	{
	}
}