#include <execution>
#include <fmt/format.h>
#include "thunderchatserver.hpp"

namespace server
{
	ThunderChatServer::ThunderChatServer(const std::string& ip, uint16_t port) noexcept :
		m_connectCallback(), m_disconnectCallback(), m_acceptThread(nullptr), m_serverThread(nullptr),
		m_running(false), m_returnCode(0), m_serverSocket(ip, port), m_clients()
	{
		m_returnCode = 
			m_serverSocket.isActive() ? 0 : m_serverSocket.getSocket() + 1;
		if (m_returnCode) return;

		sockaddr_in addrv4;
		addrv4.sin_family = AF_INET;
		addrv4.sin_port = htons(port);
		auto retCode = inet_pton(AF_INET, m_serverSocket.getIP().c_str(), &(addrv4.sin_addr));
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		m_running = true;
		retCode = bind(m_serverSocket.getSocket(), reinterpret_cast<sockaddr*>(&addrv4), sizeof(sockaddr));
		m_returnCode = retCode < 0 ? -retCode : 0;
		if (m_returnCode) return;

		retCode = listen(m_serverSocket.getSocket(), 255);
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
			m_serverThread = nullptr;

			std::for_each(m_clients.begin(), m_clients.end(), [this](network::Connexion& cli) {
				cli.close();
				std::for_each(std::execution::par, m_disconnectCallback.begin(), m_disconnectCallback.end(), [&cli](CallbackType callback) {
					callback(fmt::format("{}:{}", cli.getIP(), cli.getPort()));
				});
			});

			m_serverSocket.close();
		}
	}

	void ThunderChatServer::acceptCallback() noexcept
	{
		const auto generator = [this]() -> std::tuple<SOCKET, sockaddr, socklen_t> {
			sockaddr clientAddr;
			socklen_t clientAddrSize = sizeof(sockaddr);
			SOCKET s = accept(m_serverSocket.getSocket(), &clientAddr, &clientAddrSize);
			return std::make_tuple(s, clientAddr, clientAddrSize);
		};

		std::replace_if(
			m_clients.begin(),
			m_clients.end(),
			[](const network::Connexion& cli) -> bool { return !cli.isActive(); },
			network::Connexion(generator())
		);
	}

	void ThunderChatServer::run() noexcept
	{
	}
}