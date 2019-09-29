#include "thunderchatserver.hpp"
#include "network.hpp"

namespace server
{
	ThunderChatServer::ThunderChatServer(const std::string& ip, int port) noexcept :
		m_connectCallback(), m_disconnectCallback(), m_serverThread(nullptr), m_running(false),
		m_returnCode(0), m_clients()
	{
		m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		m_returnCode = m_serverSocket < 0 ? -m_serverSocket : 0;
		if (m_returnCode) return;

		sockaddr_in addrv4;
		addrv4.sin_family = AF_INET;
		addrv4.sin_port = htons(port);
		auto retCode = inet_pton(AF_INET, ip.c_str(), &(addrv4.sin_addr));
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		retCode = bind(m_serverSocket, reinterpret_cast<sockaddr*>(&addrv4), sizeof(sockaddr));
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		retCode = listen(m_serverSocket, 255);
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		m_serverThread = std::make_unique<std::thread>([this]() { run(); });
	}

	ThunderChatServer::~ThunderChatServer() noexcept
	{
		stop();
		if (m_serverThread->joinable())
		{
			m_serverThread->join();
		}
	}

	void ThunderChatServer::onConnect(const CallbackType& callback) noexcept
	{
		m_connectCallback.push_back(callback);
	}

	void ThunderChatServer::onDisconnect(const CallbackType& callback) noexcept
	{
		m_disconnectCallback.push_back(callback);
	}

	void ThunderChatServer::stop() noexcept
	{
		m_running = false;
	}

	void ThunderChatServer::run()
	{
		m_running = true;
	}
}