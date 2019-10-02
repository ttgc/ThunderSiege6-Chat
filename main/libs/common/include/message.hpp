#pragma once
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

namespace network
{
	namespace message
	{
		// Type of the message (i.e. if the message should be sent to the player team only or to everyone)
		enum MessageType {GLOBAL = 0, TEAM = 1};

		// The player's team (A or B)
		enum Team {TEAM_A = 0, TEAM_B = 1};

		// Message class
		class Message
		{
		public:
			Message(
				const std::string& username,
				MessageType msgType,
				Team playerTeam,
				const std::string& message
			) noexcept;
			static std::optional<Message> getMessageFromJson(const std::string& message) noexcept;

			Message(const Message& other) = default;
			Message(Message&& other) = default;
			virtual ~Message() = default;
			Message& operator=(const Message& rhs) = default;
			Message& operator=(Message&& rhs) = default;

			std::string getMessage() const noexcept { return m_message; }
			MessageType getMessageType() const noexcept { return m_type; }
			Team getPlayerTeam() const noexcept { return m_team; }
			std::string getPlayerUsername() const noexcept { return m_player; }
			nlohmann::json getJsonMessage() const noexcept;

			virtual bool isCorrectlySized() const noexcept;

		protected:
			std::string m_message;
			MessageType m_type;
			Team m_team;
			std::string m_player;

		private:
			Message(const nlohmann::json& message) noexcept;
			static size_t s_maxsizeUsername;
			static size_t s_maxsizeMessage;
		};
	}
}

