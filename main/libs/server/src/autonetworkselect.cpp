#include <algorithm>
#include "autonetworkselect.hpp"

namespace server
{
	AutoNetworkSelect::AutoNetworkSelect(
		const std::vector<SOCKET>& read,
		const std::vector<SOCKET>& write,
		const std::vector<SOCKET>& except,
		const std::chrono::microseconds timeout) noexcept :
			m_reading(read.empty() ? nullptr : std::make_shared<fd_set>()),
			m_writing(write.empty() ? nullptr : std::make_shared<fd_set>()),
			m_except(except.empty() ? nullptr : std::make_shared<fd_set>()),
			m_timeout(), m_retCode(0)
	{
		m_timeout.tv_sec = static_cast<long>(timeout.count() / 1000000);
		m_timeout.tv_usec = static_cast<long>(timeout.count() % 1000000);

		std::vector<SOCKET> fullList(read.size() + write.size() + except.size());
		std::copy(read.begin(), read.end(), fullList.begin());
		std::copy(write.begin(), write.end(), fullList.begin() + read.size());
		std::copy(except.begin(), except.end(), fullList.begin() + read.size() + write.size());

		if (m_reading != nullptr) FD_ZERO(m_reading.get());
		if (m_writing != nullptr) FD_ZERO(m_writing.get());
		if (m_except != nullptr) FD_ZERO(m_except.get());
		std::for_each(read.begin(), read.end(), [this](const SOCKET& s) {
			FD_SET(s, m_reading.get());
		});
		std::for_each(write.begin(), write.end(), [this](const SOCKET& s) {
			FD_SET(s, m_writing.get());
		});
		std::for_each(except.begin(), except.end(), [this](const SOCKET& s) {
			FD_SET(s, m_except.get());
		});

		m_retCode = select(
			network::getNfds(fullList),
			m_reading.get(),
			m_writing.get(),
			m_except.get(),
			&m_timeout
		);
	}

	/*AutoNetworkSelect::AutoNetworkSelect(const AutoNetworkSelect& other) noexcept :
		m_reading(other.m_reading == nullptr ? nullptr : std::make_shared<fd_set>()),
		m_writing(other.m_writing == nullptr ? nullptr : std::make_shared<fd_set>()),
		m_except(other.m_except == nullptr ? nullptr : std::make_shared<fd_set>()),
		m_timeout(other.m_timeout), m_retCode(other.m_retCode)
	{
		if (m_reading != nullptr)
		{
			FD_ZERO(m_reading.get());
			std::for_each(
				other.m_reading->fd_array,
				other.m_reading->fd_array + other.m_reading->fd_count,
				[this](const SOCKET& s) {
					FD_SET(s, m_reading.get());
				}
			);
		}

		if (m_writing != nullptr) 
		{
			FD_ZERO(m_writing.get());
			std::for_each(
				other.m_writing->fd_array,
				other.m_writing->fd_array + other.m_writing->fd_count,
				[this](const SOCKET& s) {
					FD_SET(s, m_writing.get());
				}
			);
		}

		if (m_except != nullptr)
		{
			FD_ZERO(m_except.get());
			std::for_each(
				other.m_except->fd_array,
				other.m_except->fd_array + other.m_except->fd_count,
				[this](const SOCKET& s) {
					FD_SET(s, m_except.get());
				}
			);
		}
	}

	AutoNetworkSelect& AutoNetworkSelect::operator=(const AutoNetworkSelect& rhs) noexcept
	{
		if (this == &rhs) return *this;
		m_reading = rhs.m_reading == nullptr ? nullptr : std::make_shared<fd_set>();
		m_writing = rhs.m_writing == nullptr ? nullptr : std::make_shared<fd_set>();
		m_except = rhs.m_except == nullptr ? nullptr : std::make_shared<fd_set>();

		if (m_reading != nullptr)
		{
			FD_ZERO(m_reading.get());
			std::for_each(
				rhs.m_reading->fd_array,
				rhs.m_reading->fd_array + rhs.m_reading->fd_count,
				[this](const SOCKET& s) {
					FD_SET(s, m_reading.get());
				}
			);
		}

		if (m_writing != nullptr)
		{
			FD_ZERO(m_writing.get());
			std::for_each(
				rhs.m_writing->fd_array,
				rhs.m_writing->fd_array + rhs.m_writing->fd_count,
				[this](const SOCKET& s) {
					FD_SET(s, m_writing.get());
				}
			);
		}

		if (m_except != nullptr)
		{
			FD_ZERO(m_except.get());
			std::for_each(
				rhs.m_except->fd_array,
				rhs.m_except->fd_array + rhs.m_except->fd_count,
				[this](const SOCKET& s) {
					FD_SET(s, m_except.get());
				}
			);
		}

		m_timeout = rhs.m_timeout;
		m_retCode = rhs.m_retCode;
		return *this;
	}*/

	bool AutoNetworkSelect::isReadSet(const SOCKET& socket) const noexcept
	{
		return m_reading == nullptr ? false : FD_ISSET(socket, m_reading.get());
	}

	bool AutoNetworkSelect::isWriteSet(const SOCKET& socket) const noexcept
	{
		return m_writing == nullptr ? false : FD_ISSET(socket, m_writing.get());
	}

	bool AutoNetworkSelect::isExceptSet(const SOCKET& socket) const noexcept
	{
		return m_except == nullptr ? false : FD_ISSET(socket, m_except.get());
	}
}
