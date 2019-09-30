#pragma once 
#include <string> 
#include "network.hpp" 

namespace network
{
	class Connexion
	{
	public:
		Connexion() noexcept;
		Connexion(const std::string& ip, uint16_t port) noexcept;
		Connexion(SOCKET sock, sockaddr addr, socklen_t size) noexcept;
		explicit Connexion(std::tuple<SOCKET, sockaddr, socklen_t> existingConnexion) noexcept;
		virtual ~Connexion() noexcept;

		Connexion(const Connexion& other) noexcept = delete;
		Connexion(Connexion&& other) noexcept = default;
		Connexion& operator=(const Connexion& rhs) noexcept = delete;
		Connexion& operator=(Connexion&& rhs) noexcept = default;

		bool isActive() const noexcept { return m_active; }
		SOCKET getSocket() const noexcept { return m_socket; }
		std::string getIP() const noexcept { return m_ip; }
		uint16_t getPort() const noexcept { return m_port; }

		void close() noexcept;

	private:
		bool m_active = false;
		SOCKET m_socket = 0;
		std::string m_ip = "0.0.0.0";
		uint16_t m_port = 0;
	};
}