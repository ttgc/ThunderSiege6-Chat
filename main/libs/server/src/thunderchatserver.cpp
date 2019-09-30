#include <execution>
#include <fmt/format.h>
#include "thunderchatserver.hpp"

namespace server
{
	ThunderChatServer::ThunderChatServer(const std::string& ip, uint16_t port) noexcept :
		m_connectCallback(), m_disconnectCallback(), m_serverThread(nullptr), m_running(false),
		m_returnCode(0), m_serverSocket(), m_clients()
	{
		m_serverSocket.m_ip = ip;
		m_serverSocket.m_port = port;

		m_serverSocket.m_socket = socket(AF_INET, SOCK_STREAM, 0);
		m_returnCode = 
			m_serverSocket.m_socket <= 0 ? static_cast<uint32_t>(m_serverSocket.m_socket) + 1 : 0;
		if (m_returnCode) return;

		sockaddr_in addrv4;
		addrv4.sin_family = AF_INET;
		addrv4.sin_port = htons(port);
		auto retCode = inet_pton(AF_INET, m_serverSocket.m_ip.c_str(), &(addrv4.sin_addr));
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		m_running = true;
		retCode = bind(m_serverSocket.m_socket, reinterpret_cast<sockaddr*>(&addrv4), sizeof(sockaddr));
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		retCode = listen(m_serverSocket.m_socket, 255);
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		m_serverThread = std::make_unique<std::thread>([this]() { run(); });
	}

	ThunderChatServer::~ThunderChatServer() noexcept
	{
		stop();
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
		if (isRunning())
		{
			m_running = false;

			if (m_serverThread != nullptr && m_serverThread->joinable())
			{
				m_serverThread->join();
			}

			std::for_each(m_clients.begin(), m_clients.end(), [this](Connexion cli) {
				shutdown(cli.m_socket, SD_BOTH);
				closesocket(cli.m_socket);
				std::for_each(std::execution::par, m_disconnectCallback.begin(), m_disconnectCallback.end(), [cli](CallbackType callback) {
					callback(fmt::format("{}:{}", cli.m_ip, cli.m_port));
				});
			});

			shutdown(m_serverSocket.m_socket, SD_BOTH);
			closesocket(m_serverSocket.m_socket);
		}
	}

	void ThunderChatServer::run()
	{
	}
}