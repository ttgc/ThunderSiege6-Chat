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

    std::string ip;
    std::string username;
    std::string teamChose;
    network::message::Team teamUser;

    std::cout << "Bonjour et bienvenue dans le chat de Thunder Siege 6 ! " << std::endl;
    std::cout << "Veuillez rentrer dans l'ordre :" << std::endl
              << "1.L'adresse ip sur laquelle vous connecter." << std::endl;
    std::cin >> ip;
    std::cout << "2. Le pseudonyme que vous souhaitez utiliser." << std::endl;
    std::cin >> username;
    std::cout << "3. l'équipe que vous souhaitez rejoindre." << std::endl;
    std::cout << "A : Equipe A" << std::endl;
    std::cout << "B : Equipe B" << std::endl;
    std::cin >> teamChose;

	if (teamChose == "A") 
	{ 
		teamUser = network::message::TEAM_A; }
    else if (teamChose == "B")
    {
        teamUser = network::message::TEAM_B;
    }
    

    client::ThunderChatClient client(ip, username, teamUser);

    if (client.Connect())
    {

        std::cin.ignore();
        client.SendToParty("Test send to party");
    }
}