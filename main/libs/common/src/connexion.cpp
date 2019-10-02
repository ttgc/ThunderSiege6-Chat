#include <array>
#include "connexion.hpp"

namespace network
{
	Connexion::Connexion() noexcept : m_active(false), m_socket(), m_ip(), m_port() {}

	Connexion::Connexion(const std::string& ip, uint16_t port) noexcept :
		m_active(false), m_socket(), m_ip(ip), m_port(port)
	{
		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket > 0) m_active = true;
	}

	Connexion::Connexion(SOCKET sock, sockaddr addr, socklen_t size) noexcept :
		m_active(true), m_socket(sock), m_ip(),
		m_port(ntohs(reinterpret_cast<sockaddr_in*>(&addr)->sin_port))
	{
		std::array<char, INET_ADDRSTRLEN> ip;
		auto retCode = inet_ntop(addr.sa_family, &addr, ip.data(), INET_ADDRSTRLEN);
		auto end = std::remove_if(ip.begin(), ip.end(), [](char c) -> bool {
			return ((c < '0') || (c > '9')) && (c != '.');
		});
		m_ip = std::string(ip.begin(), end);
		if (retCode == nullptr) m_active = false;
	}

	Connexion::~Connexion() noexcept
	{
		close();
	}

	void Connexion::close() noexcept
	{
		if (isActive())
		{
			shutdown(m_socket, SD_BOTH);
			closesocket(m_socket);
			m_active = false;
		}
	}

	NonBlockingConnexion::NonBlockingConnexion() noexcept : Connexion()
	{
		network::setNonBlockingSocket(m_socket);
	}

	NonBlockingConnexion::NonBlockingConnexion(const std::string& ip, uint16_t port) noexcept :
		Connexion(ip, port)
	{
		network::setNonBlockingSocket(m_socket);
	}

	NonBlockingConnexion::NonBlockingConnexion(SOCKET sock, sockaddr addr, socklen_t size) noexcept :
		Connexion(sock, addr, size)
	{
		network::setNonBlockingSocket(m_socket);
	}
}