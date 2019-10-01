#include <iostream>
#include "ThunderChatClient.hpp"
#include <iostream>

#ifdef _WIN32
#include "winnetworkconfig.hpp"
#endif // WIN32


int main(void)
{
    
#ifdef _WIN32
		network::WinNetworkConfig api;
#endif // WIN32


		client::ThunderChatClient client("192.168.0.133", "JeanMi", network::message::TEAM_A);

		if (client.Connect()) { 

			std::cin.ignore();
			client.SendToParty("Wesh, tu suces ?");
		}
}