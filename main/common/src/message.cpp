#include "message.hpp"

namespace network
{
	namespace message
	{
		size_t Message::s_maxsizeUsername = 24;
		size_t Message::s_maxsizeMessage = 512;

		Message::Message(
			const std::string& username,
			MessageType msgType,
			Team playerTeam,
			const std::string& message) noexcept :
				m_message(message), m_type(msgType), m_team(playerTeam), m_player(username)
		{}

		Message::Message(const nlohmann::json& message) noexcept :
			Message(message.at("msg"), message.at("msg_type"), message.at("team"), message.at("username"))
		{}

		nlohmann::json Message::getJsonMessage() const noexcept
		{
			nlohmann::json formatedMessage =
			{
				{"username", m_player},
				{"msg_type", m_type},
				{"team", m_team},
				{"msg", m_message}
			};

			return formatedMessage;
		}

		bool Message::isCorrectlySized() const noexcept
		{
			return (m_player.size() <= s_maxsizeUsername) && (m_message.size() <= s_maxsizeMessage);
		}
	}
}
