#pragma once
#include "message.hpp"
#include "connexion.hpp"

namespace server
{
	class ClientData
	{
	public:
		ClientData() noexcept;
		ClientData(
			const std::string& username,
			network::message::Team team,
			std::shared_ptr<network::Connexion>&& connexion
		) noexcept;
		ClientData(const ClientData& other) noexcept = default;
		ClientData(ClientData&& other) noexcept = default;
		~ClientData() noexcept = default;
		ClientData& operator=(const ClientData& rhs) = default;
		ClientData& operator=(ClientData&& rhs) = default;

		std::string getUsername() const noexcept { return m_username; }
		network::message::Team getTeam() const noexcept { return m_team; }
		std::shared_ptr<network::Connexion> getConnexion() const noexcept { return m_connexion; }

	private:
		std::string m_username;
		network::message::Team m_team;
		std::shared_ptr<network::Connexion> m_connexion;
	};
}