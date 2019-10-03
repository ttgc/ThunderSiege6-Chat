#include "thunderchatserver.hpp"
#include <iostream>

#ifdef _WIN32
#include "winnetworkconfig.hpp"
#endif // WIN32

int main(void)
{
#ifdef _WIN32
	network::WinNetworkConfig api;
#endif // WIN32

    std::cout << "Hello world!" << std::endl;

	server::ThunderChatServer server("127.0.0.1", 8888);
	if (server.isRunning())
	{
		std::cout << "Server is running on : " << server.getIP() << ":" << server.getPort() << "\n";
		server.onConnect([](const std::string& client) { std::cout << client << " is now online\n"; });
		server.onDisconnect([](const std::string& client) { std::cout << client << " is now offline\n"; });
		std::cout << "Press return key to stop the server\n\n";
		std::cin.ignore();
	}
	else
	{
		std::cout << "FATAL ERROR : " << server.getErrorCode() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}