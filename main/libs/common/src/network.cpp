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
}
