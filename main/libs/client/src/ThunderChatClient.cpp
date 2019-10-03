#include "ThunderChatClient.hpp"

namespace client
{

    ThunderChatClient::ThunderChatClient(const std::string& ip, const std::string& username,
                                        const network::message::Team & team) noexcept
        : m_ip(ip)
        , m_username(username)
        , m_team(team)
        , m_s(socket(AF_INET, SOCK_STREAM, 0))
        , m_clientAddr()
        , m_clientAddrSize(sizeof(sockaddr))
        , m_client()
        , m_addrv4()
        , m_messageCallback()
        , m_disconnectCallback()
        , m_running(false)
        , m_clientThread(nullptr)
    {
        if (m_s < 0)
        {
            std::cout << "Error";
            return;
        }

        std::string just_ip;
        std::string port;
        std::size_t pos = m_ip.find_last_of(":");
        if (pos == m_ip.length()-6)
        {
            port = m_ip.substr(m_ip.length()-6, 5);
        }
        else
        {
            port = "8888";
        }
        just_ip = m_ip.substr(0, pos);
        
        int p = std::stoi(port);
        uint16_t p2 = p;

        m_client = network::Connexion(just_ip, p2);

       /* network::DNSresolver::DNSresolver(just_ip);
        std::vector<sockaddr_in> vect;
        vect = network::DNSresolver::all();
        
        if(!vect.empty) m_addrv4 = vect.back();*/

        m_addrv4.sin_family = AF_INET;
        m_addrv4.sin_port = htons(p2);
        if (inet_pton(AF_INET, just_ip.c_str(), &(m_addrv4.sin_addr)) < 0)
        {
            std::cout << "Error";
            return;
        }

        m_running = true;
        m_clientThread = std::make_unique<std::thread>([this]() { run(); });
    }

    ThunderChatClient::~ThunderChatClient() noexcept
    {
        m_running = false;
        if(m_clientThread->joinable())
        {
            m_clientThread->join();
        } 
        shutdown(m_s, SD_BOTH);
        closesocket(m_s);
    }

	bool ThunderChatClient::isRunning() 
	{ 
		return m_running; 
	}


    bool ThunderChatClient::Connect() noexcept
    {
        if (connect(m_s, reinterpret_cast<sockaddr*>(&m_addrv4), m_clientAddrSize) == 0)
        {
            SendToTeam("Tentative Connection");
            std::cout << "Client is connected." << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }

    void ThunderChatClient::OnMessage(const CallbackMsg& msg) noexcept
    {
        m_messageCallback.push_back(msg);
    }

    void ThunderChatClient::OnDisconnect(const CallbackDeco& deco) noexcept
    {
        m_disconnectCallback.push_back(deco);
    }

    void ThunderChatClient::SendToParty(const std::string& rawMessage)
    {
        network::message::Message msg(m_username, network::message::GLOBAL, m_team, rawMessage);
        if (msg.isCorrectlySized())
        {
            std::string message = msg.getJsonMessage().dump();
            send(m_s, message.c_str(), message.size(), 0);
        }
        else
        {
            std::for_each(m_disconnectCallback.begin(), m_disconnectCallback.end(), [](const CallbackDeco& callback) {
                    callback();
                });
                m_running = false;
                return;
        }
    }

    void ThunderChatClient::SendToTeam(const std::string& rawMessage)
    {
        network::message::Message msg(m_username, network::message::TEAM, m_team, rawMessage);
        if (msg.isCorrectlySized())
        {
            std::string message = msg.getJsonMessage().dump();
            send(m_s, message.c_str(), message.size(), 0);
        }
        else
        {
            std::for_each(m_disconnectCallback.begin(), m_disconnectCallback.end(), [](const CallbackDeco& callback) {
                    callback();
                });
                m_running = false;
                return;
        }
    }

	
    void ThunderChatClient::run() noexcept
    {
        while (m_running)
        {
            std::array<char, 1024> buffer;
            int length_msg = recv(m_s, buffer.data(), 1024, 0);
            if(length_msg < 0)
            {
                std::for_each(m_disconnectCallback.begin(), m_disconnectCallback.end(), [](const CallbackDeco& callback) {
                    callback();
                });
                m_running = false;
                return;
            }
            else
            {
                std::string received(buffer.data(), length_msg);
                auto msg = network::message::Message::getMessageFromJson(received);
                if (msg.has_value() && msg.value().isCorrectlySized())
                {
                    std::for_each(m_messageCallback.begin(), m_messageCallback.end(), [&msg](const CallbackMsg& callback) 
                    {
                        callback(msg.value());
                    });
                    std::string username(msg.value().getPlayerUsername());
                    std::string team;
                    std::string message;
                    if (msg.value().getMessageType())
                    {   
                        std::string type(" (to Team ");
                        if (msg.value().getMessageType()) { std::string team = "B) : "; }
                        else            {   std::string team = "A) : ";  }
                        std::string message = username + type + team + msg.value().getMessage();
                    }
                    else
                    {
                        std::string type(" (to Party) : ");
                        std::string message = username + type + msg.value().getMessage();
                    }
                    std::cout << message << std::endl;
                }
            }
        }
    }
}