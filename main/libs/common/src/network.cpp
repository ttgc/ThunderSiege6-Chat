#include <algorithm>
#include "network.hpp"

namespace network
{
	void setNonBlockingSocket(SOCKET& s)
	{
#ifdef _WIN32
		u_long nonBlocking = 1;
		ioctlsocket(s, FIONBIO, &nonBlocking);
#else // _WIN32
		int flags = fcntl(s, F_GETFL, 0);
		flags = (flags | O_NONBLOCK);
		fcntl(s, F_SETFL, flags);
#endif // _WIN32
	}

	int getNfds(const std::vector<SOCKET>& sockets)
	{
#ifdef _WIN32
		return 0;
#else // _WIN32
		auto maxs = std::max_element(sockets.begin(), sockets.end());
		return (maxs != sockets.end()) ? *maxs : 0;
#endif // _WIN32
	}
}
