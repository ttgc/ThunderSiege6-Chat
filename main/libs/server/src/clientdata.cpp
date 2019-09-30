#include "clientdata.hpp"

namespace server
{
	ClientData::ClientData() noexcept :
		m_username(""), m_team(network::message::TEAM_A), m_connexion(nullptr)
	{}

	ClientData::ClientData(
		const std::string& username,
		network::message::Team team,
		std::shared_ptr<network::Connexion>&& connexion) noexcept :
			m_username(username), m_team(team), m_connexion(std::move(connexion))
	{}
}
