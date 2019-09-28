#include "network.hpp"
#include "winnetworkconfig.hpp"

namespace network
{
	WORD WinNetworkConfig::s_version = MAKEWORD(2, 2);

	WinNetworkConfig::WinNetworkConfig() noexcept : m_failure(false), m_data()
	{
		m_failure = (WSAStartup(s_version, &m_data) >= 0);
	}

	WinNetworkConfig::~WinNetworkConfig() noexcept
	{
		WSACleanup();
	}
}