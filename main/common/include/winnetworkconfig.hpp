#pragma once

namespace network
{
	// Windows Network API configuration class
	class WinNetworkConfig
	{
	public:
		WinNetworkConfig() noexcept;
		~WinNetworkConfig() noexcept;
		WinNetworkConfig(WinNetworkConfig&& other) noexcept = default;
		WinNetworkConfig& operator=(WinNetworkConfig&& rhs) noexcept = default;

		WinNetworkConfig(const WinNetworkConfig& other) = delete;
		WinNetworkConfig& operator=(const WinNetworkConfig& rhs) = delete;

		bool hasFailed() const noexcept { return m_failure; }
		WSADATA getData() const noexcept { return m_data; }

	private:
		bool m_failure; // Indicate if the API has been initialized successful or not
		WSADATA m_data; // Data get by init of the API

		static WORD s_version; // Requested version of the API
	};
}