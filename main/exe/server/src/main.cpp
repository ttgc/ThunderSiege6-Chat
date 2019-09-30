#include "thunderchatserver.hpp"
#include <iostream>
#include <chrono>

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
		using namespace std::chrono_literals;
		std::cout << "Server is running on : " << server.getIP() << ":" << server.getPort() << "\n";
		server.onConnect([](const std::string& client) { std::cout << client << " is now online\n"; });
		server.onDisconnect([](const std::string& client) { std::cout << client << " is now offline\n"; });
		std::this_thread::sleep_for(10s);
	}
	else
	{
		std::cout << "FATAL ERROR : " << server.getErrorCode() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}