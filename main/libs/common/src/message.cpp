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

		std::optional<Message> Message::getMessageFromJson(const nlohmann::json & message) noexcept
		{
			if (message.contains("username") && message.contains("msg_type") &&
				message.contains("team") && message.contains("msg") &&
				message.at("msg_type").is_number_unsigned() &&
				message.at("team").is_number_unsigned() &&
				(message.at("msg_type") == GLOBAL || message.at("msg_type") == TEAM) &&
				(message.at("team") == TEAM_A || message.at("team") == TEAM_B))
			{
				return std::make_optional<Message>(std::move(Message(message)));
			}
			return std::nullopt;
		}

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

		Message::Message(const nlohmann::json& message) noexcept :
			Message(message.at("msg"), message.at("msg_type"), message.at("team"), message.at("username"))
		{}
	}
}
