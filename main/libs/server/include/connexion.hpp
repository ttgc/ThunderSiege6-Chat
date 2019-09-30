#pragma once 
#include <string> 
#include "network.hpp" 

namespace server
{
	struct Connexion
	{
		SOCKET m_socket = 0;
		std::string m_ip = "0.0.0.0";
		uint16_t m_port = 0;
	};
}