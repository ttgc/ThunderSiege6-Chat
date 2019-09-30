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
		std::array<char, 512> ip;
		inet_ntop(addr.sa_family, &addr, ip.data(), 512);
		m_ip = std::string(ip.begin(), ip.end());
	}

	Connexion::Connexion(std::tuple<SOCKET, sockaddr, socklen_t> existingConnexion) noexcept :
		Connexion(std::get<0>(existingConnexion), std::get<1>(existingConnexion), std::get<2>(existingConnexion))
	{}

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
}