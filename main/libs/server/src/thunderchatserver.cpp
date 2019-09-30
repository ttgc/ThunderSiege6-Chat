#include <execution>
#include <fmt/format.h>
#include "message.hpp"
#include "autonetworkselect.hpp"
#include "thunderchatserver.hpp"

namespace server
{
	ThunderChatServer::ThunderChatServer(const std::string& ip, uint16_t port) noexcept :
		m_connectCallback(), m_disconnectCallback(), m_acceptThread(nullptr), m_serverThread(nullptr),
		m_running(false), m_returnCode(0), m_serverSocket(ip, port), m_clients()
	{
		m_returnCode = 
			m_serverSocket.isActive() ? 0 : static_cast<uint32_t>(m_serverSocket.getSocket()) + 1;
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

		m_acceptThread = std::make_unique<std::thread>([this]() { acceptCallback(); });
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

			if (m_acceptThread != nullptr && m_acceptThread->joinable())
			{
				m_acceptThread->join();
			}
			if (m_serverThread != nullptr && m_serverThread->joinable())
			{
				m_serverThread->join();
			}
			m_acceptThread = nullptr;
			m_serverThread = nullptr;

			std::for_each(m_clients.begin(), m_clients.end(),
				[this](const ClientData& cli) {
					auto connexion = cli.getConnexion();
					if (connexion != nullptr && connexion->isActive())
					{
						cli.getConnexion()->close();
						std::for_each(
							std::execution::par,
							m_disconnectCallback.begin(),
							m_disconnectCallback.end(),
							[connexion](CallbackType callback) {
								callback(fmt::format("{}:{}", connexion->getIP(), connexion->getPort()));
							}
						);
					}
				}
			);

			m_serverSocket.close();
		}
	}

	void ThunderChatServer::acceptCallback() noexcept
	{
		const auto generator = [this]() -> ClientData {
			sockaddr clientAddr;
			socklen_t clientAddrSize = sizeof(sockaddr);
			SOCKET s = accept(m_serverSocket.getSocket(), &clientAddr, &clientAddrSize);
			if (s > 0)
			{
				auto cli = std::make_shared<network::NonBlockingConnexion>(s, clientAddr, clientAddrSize);
				if (cli->isActive())
				{
					std::for_each(
						std::execution::par,
						m_connectCallback.begin(),
						m_connectCallback.end(),
						[cli](CallbackType callback) {
							callback(fmt::format("{}:{}", cli->getIP(), cli->getPort()));
						}
					);

					bool loop(true), couldrecv(false);
					std::vector<SOCKET> vec { cli->getSocket() };
					int iter = 10;
					while (loop)
					{
						AutoNetworkSelect selector(
							vec, std::vector<SOCKET>(), std::vector<SOCKET>(),
							std::chrono::microseconds(1000)
						);
						if (selector.hasFailed()) return ClientData();
						couldrecv = selector.isReadSet(cli->getSocket());
						loop = (!couldrecv) && iter;
						--iter;
					}

					if (couldrecv)
					{
						std::array<char, 1024> buffer;
						int length = recv(cli->getSocket(), buffer.data(), 1024, 0);

						if (length > 0)
						{
							std::string received(buffer.data(), length);
							auto msg = network::message::Message::getMessageFromJson(received);
							if (msg.has_value())
							{
								ClientData data(
									msg.value().getPlayerUsername(),
									msg.value().getPlayerTeam(),
									std::move(cli)
								);
								return data;
							}
						}
					}
				}
			}
			return ClientData();
		};

		while (m_running)
		{
			std::replace_if(
				m_clients.begin(),
				m_clients.end(),
				[](const ClientData& cli) -> bool {
					return cli.getConnexion() != nullptr && !cli.getConnexion()->isActive();
				},
				generator()
			);
		}
	}

	void ThunderChatServer::run() noexcept
	{
		const size_t maxSize = 1024;
		std::array<char, maxSize> buffer;

		while (m_running)
		{
			//AutoNetworkSelect selector();
		}
	}
}