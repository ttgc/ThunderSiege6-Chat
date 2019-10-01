#include "dnsresolver.hpp"
#include "..\include\dnsresolver.hpp"

namespace network
{
	DNSresolver::DNSresolver(const std::string& hostname) noexcept :
		m_hostname(hostname), m_address()
	{
		/*
		 * Make the DNS request
		 * https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/26/lec.html
		 * Use reinterpret_cast for casting
		 */
	}

	sockaddr_in DNSresolver::first() const noexcept
	{
		/*
		 * Send the first sockaddr (by copy) of m_address
		 */
		return sockaddr_in();
	}

	sockaddr_in DNSresolver::last() const noexcept
	{
		/*
		 * Send the last sockaddr (by copy) of m_address
		 */
		return sockaddr_in();
	}

	std::vector<sockaddr_in> DNSresolver::all() const noexcept
	{
		/*
		 * Build vector
		 * Use std::transform to iterate through m_address
		 */
		return std::vector<sockaddr_in>();
	}
}