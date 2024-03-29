#pragma once
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <string.h>
#include <memory>
#include <wchar.h>
#include "network.hpp"

namespace network
{
	class DNSresolver
	{
	public:
		DNSresolver(const std::string& hostname) noexcept;
		DNSresolver(const DNSresolver& other) = delete;
		DNSresolver(DNSresolver&& other) = default;
		~DNSresolver() = default;
		DNSresolver& operator=(const DNSresolver& rhs) = delete;
		DNSresolver& operator=(DNSresolver&& rhs) = default;

		std::optional<sockaddr_in> first() const noexcept;
		std::vector<sockaddr_in> all() const noexcept;

	private:
		std::string m_hostname;
		std::shared_ptr<addrinfo> m_address;
	};
}

