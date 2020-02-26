#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "NetworkingWrapper.h"
#include <unordered_map>

enum class MessageType
{
	MSG_INT,				//Message type int
	MSG_ADDCLIENT,			//Message type Add client to list
	MSG_REMOVECLIENT,		//Message type Remove client from list
	MSG_CHATREQUEST,		//Message type Send a chat request
	MSG_CHATRESPONSE,		//Message type Respond to a chat invite
	MSG_CHATSTRING,			//Message type Send a chat message
	MSG_GAMEREQUEST,		//Message type Request to play a game
	MSG_GAMERESPONSE,		//Message type Respond to a chat invite
	MSG_GAMEPOSITION,		//Message type Send Game Position
	MSG_CONNECT,			//Message type Connect Request
	MSG_DISCONNECT,			//Message type Disconnect Request
};

enum class MessageFlags
{

	BROADCAST_ALL,			//Broadcast to all clients connected to the server
	BROADCAST_RELATED,		//Broadcast to only clients grouped with this client
	NONE					//Do nothing
};

struct ChatActivity
{
	ChatActivity() { }

	void SendRequest(int chatID)
	{
		//If you can't find the chat id in our list
		if (chatLogs.find(chatID) == chatLogs.end())
		{
			requestsRejected.insert(std::pair<int, bool>(chatID, false));
			requestsAccepted.insert(std::pair<int, bool>(chatID, false));
			requestsSent.insert(std::pair<int, bool>(chatID, true));
			requestsReceived.insert(std::pair<int, bool>(chatID, false));

			std::vector<std::string> newLog;
			chatLogs.insert(std::pair<int, std::vector<std::string>>(chatID, newLog));
		}
	}

	void ReceiveRequest(int chatID)
	{

	}

	std::unordered_map<int, bool> requestsRejected;
	std::unordered_map<int, bool> requestsAccepted;
	std::unordered_map<int, bool> requestsSent;
	std::unordered_map<int, bool> requestsReceived;
	std::unordered_map<int, std::vector<std::string>> chatLogs;
};

struct PrintActivity
{
	static void Output(Activity doing)
	{
		switch (doing)
		{
		case Activity::AVAILABLE:
			printf("-- Available");
			break;
		case Activity::BUSY_CHATTING:
			printf("-- Busy Chatting");
			break;
		case Activity::BUSY_PLAYINGGAME:
			printf("-- Busy Playing A Game");
			break;
		}
	}

	static std::string Return(Activity doing)
	{
		switch (doing)
		{
		case Activity::AVAILABLE:
			return "-- Available";
			break;
		case Activity::BUSY_CHATTING:
			return "-- Busy Chatting";
			break;
		case Activity::BUSY_PLAYINGGAME:
			return "-- Busy Playing A Game";
			break;
		}
	}
};

class IDGen
{
public:
	static int GetNew();

private:
	static int m_ID;
};

class Client
{
public:
	Client() { }
	Client(sockaddr_in address);
	~Client();

	void InitClient();

	void ConnectToServer(const char* ip, const char* username);
	void DisconnectFromServer();

	void Update();

	void SendMsg(MessageType type, Convertable* message, MessageFlags flags);
	Convertable* RecvMsg(MessageType& type);

	sockaddr_in GetAddress() const;

	void PrintOtherClientData();
	void ShutdownClient();




	int m_clientID = -1;
	std::string m_username;
	Activity m_activity = Activity::AVAILABLE;
	bool m_connected = false;

	std::vector<Client> m_otherClients;
private:
	SOCKET m_cliSock;
	struct addrinfo* m_ptr = NULL, m_hints;

	sockaddr_in m_clientAddress;
};

struct ClientCompare
{
public:
	ClientCompare(Client* client) : m_client(client) { }

	bool operator()(Client* compare) const
	{
		bool addressSame = (m_client->GetAddress().sin_addr.S_un.S_addr == compare->GetAddress().sin_addr.S_un.S_addr);
		bool familySame = m_client->GetAddress().sin_family == compare->GetAddress().sin_family;
		bool portSame = m_client->GetAddress().sin_port == compare->GetAddress().sin_port;

		return (addressSame && familySame && portSame);
	}

	Client* m_client;
};


#endif // !__CLIENT_H__
