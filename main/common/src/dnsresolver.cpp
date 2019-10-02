#include "dnsresolver.hpp"
#include <iostream>

namespace network
{
	DNSresolver::DNSresolver(const std::string& hostname) noexcept :
		m_hostname(hostname), m_address(nullptr)
	{
		struct addrinfo type;
		memset(&type, 0, sizeof(struct addrinfo));
		type.ai_family = AF_INET;
		auto addr = m_address.get();
		if(getaddrinfo(m_hostname.c_str(), NULL, &type, &addr) != 0) {
                    std::cout << "Error\n";
                    return;
		}
	}

	std::optional<sockaddr_in> DNSresolver::first() const noexcept
	{
		if(&m_address == nullptr) return std::nullopt;
		sockaddr_in* sockin_ipv4 = reinterpret_cast<sockaddr_in*>(m_address->ai_addr);
        return std::optional<sockaddr_in>(*sockin_ipv4);
	}

	std::vector<sockaddr_in> DNSresolver::all() const noexcept
	{
		std::vector<sockaddr_in> vect;
		auto iterator = m_address.get();
		while (m_address->ai_next != nullptr)
		{
			sockaddr_in* sockin_ipv4 = reinterpret_cast<sockaddr_in*>(m_address->ai_addr);
			vect.push_back(*sockin_ipv4);
			iterator = m_address->ai_next;
		}
		return vect;
	}
}