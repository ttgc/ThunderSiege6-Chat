#pragma once
#include <chrono>
#include <memory>
#include "network.hpp"

namespace server
{
	class AutoNetworkSelect
	{
	public:
		AutoNetworkSelect(
			const std::vector<SOCKET>& read,
			const std::vector<SOCKET>& write,
			const std::vector<SOCKET>& except,
			const std::chrono::microseconds timeout
		) noexcept;
		AutoNetworkSelect(const AutoNetworkSelect& other) noexcept;
		AutoNetworkSelect(AutoNetworkSelect&& other) = default;
		~AutoNetworkSelect() = default;
		AutoNetworkSelect& operator=(const AutoNetworkSelect& rhs) noexcept;
		AutoNetworkSelect& operator=(AutoNetworkSelect&& rhs) = default;

		bool hasFailed() const noexcept { return (m_retCode <= 0); }
		int getReturnCode() const noexcept { return m_retCode; }
		timeval getTimeout() const noexcept { return m_timeout; }

		bool isReadSet(const SOCKET& socket) const noexcept;
		bool isWriteSet(const SOCKET& socket) const noexcept;
		bool isExceptSet(const SOCKET& socket) const noexcept;

	private:
		std::shared_ptr<fd_set> m_reading;
		std::shared_ptr<fd_set> m_writing;
		std::shared_ptr<fd_set> m_except;
		timeval m_timeout;
		int m_retCode;
	};
}
