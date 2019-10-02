#include "ThunderChatClient.hpp"
#include "dnsresolver.hpp"
#include <iostream>
#include <string>

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
    std::string teamChoice;
    network::message::Team teamUser;

    std::cout << "Bonjour et bienvenue dans le chat de Thunder Siege 6 ! " << std::endl;
    std::cout << "Veuillez rentrer dans l'ordre :" << std::endl
              << "1. L'adresse du serveur sur laquelle vous connecter." << std::endl;
    std::cin >> ip;
    std::cout << "2. Le pseudonyme que vous souhaitez utiliser." << std::endl;
    while (username.length < 1 && username.length > 24)
    {
        std::cin >> username;
        if (username.length < 1 && username.length > 24)
        {
            std::cout << "Le pseudonyme choisi n'est pas conforme, il doit être inférieur a 24 caractères." << std::endl;
        }
    }
    std::cout << "3. L'Equipe que vous souhaitez rejoindre." << std::endl;
    std::cout << "A : Equipe A" << std::endl;
    std::cout << "B : Equipe B" << std::endl;
    while (teamChoice != "A" || teamChoice != "B")
    {
        std::cin >> teamChoice;
        if (teamChoice != "A" || teamChoice != "B")
        {
            std::cout << "L'Equipe choisie n'existe pas ! Veuillez choisir A ou B. " << std::endl;
        }
    }

    network::DNSresolver::DNSresolver(ip);
    std::vector<sockaddr_in> vect;
    vect = network::DNSresolver::all();
    if(vect != nullptr) ip = vect.end();

	if (teamChoice == "A") 
	{ 
		teamUser = network::message::TEAM_A; 
    }
    else if (teamChoice == "B")
    {
        teamUser = network::message::TEAM_B;
    }

    client::ThunderChatClient client(ip, username, teamUser);

    if (client.Connect())
    {
        while(client::ThunderChatClient::m_running)
        {
            std::string message;
            getline(cin, message);
            std::string send;
            if (message.rfind("/all ", 0) && message.length > 5)
            {
                message.substr(5);
                send = username + " (to Party) : " + message;
                client::ThunderChatClient::SendToParty(message);
            }
            else
            {
                send = username + " (to Team " + teamChoice + ") : " + message;
                client::ThunderChatClient::SendToTeam(message);
            }
            std::cout << send << std::endl;
        }
    }
}