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
	ChatActivity() 
	{ }

	void AddNew(int chatID, std::string chatName)
	{
		if (chatLogs.find(chatID) == chatLogs.end())
		{
			chatBoxName.insert(std::pair<int, std::string>(chatID, chatName));
			currentlyChatting.insert(std::pair<int, bool>(chatID, false));
			requestsSent.insert(std::pair<int, bool>(chatID, false));
			requestsReceived.insert(std::pair<int, bool>(chatID, false));
			chatBoxBuf.insert(std::pair<int, char*>(chatID, new char[BUF_LEN]));

			memset(chatBoxBuf.at(chatID), 0, BUF_LEN);

			std::vector<std::string> newLog;
			chatLogs.insert(std::pair<int, std::vector<std::string>>(chatID, newLog));
		}
	}

	void Remove(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			memset(chatBoxBuf.at(chatID), 0, BUF_LEN);
			chatBoxBuf.erase(chatID);
			chatBoxName.erase(chatID);
			currentlyChatting.erase(chatID);
			requestsSent.erase(chatID);
			requestsReceived.erase(chatID);

			chatLogs.erase(chatID);
		}
	}

	void SendRequest(int chatID)
	{
		//If you can't find the chat id in our list
		if (chatLogs.find(chatID) == chatLogs.end())
		{
			return;
		}
		requestsSent.at(chatID) = true;
	}

	void ReceiveRequest(int chatID)
	{
		if (chatLogs.find(chatID) == chatLogs.end())
		{
			return;
		}
		requestsReceived.at(chatID) = true;
	}

	void RespondRequest(int chatID, bool response)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			if (response)
			{
				currentlyChatting.at(chatID) = true;
				requestsReceived.at(chatID) = false;
				requestsSent.at(chatID) = false;
			}
			else
			{
				requestsReceived.at(chatID) = false;
				requestsSent.at(chatID) = false;
			}
		}
		else
		{
			printf("You messed up again...");
		}
	}

	void ReceiveResponse(int chatID, bool response)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			if (response)
			{
				currentlyChatting.at(chatID) = true;
				requestsReceived.at(chatID) = false;
				requestsSent.at(chatID) = false;
			}
			else
			{
				requestsReceived.at(chatID) = false;
				requestsSent.at(chatID) = false;
			}
		}
		else
		{
			printf("You messed up again...");
		}
	}

	void AddMessage(int chatID, std::string message)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			chatLogs.at(chatID).push_back(message);
		}
		else
		{
			printf("Shouldn't print this");
		}
	}
	
	void EndChat(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//Add message to log
			currentlyChatting.at(chatID) = false;
			requestsSent.at(chatID) = false;
			requestsReceived.at(chatID) = false;
		}
	}

	void ReceiveMessage(int chatID, std::string message)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//Add message to log
			chatLogs.at(chatID).push_back(message);
		}
		else
		{
			printf("You messed up, this shouldn't be here.");
		}
	}

	bool GetRequestSent(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//return bool
			return requestsSent.at(chatID);
		}
		else
		{
			return false;
		}
	}

	bool GetRequestReceived(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//return bool
			return requestsReceived.at(chatID);
		}
		else
		{
			return false;
		}
	}
	
	bool GetCurrentlyChatting(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//return bool
			return currentlyChatting.at(chatID);
		}
		else
		{
			return false;
		}
	}

	std::vector<std::string> GetChatLog(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//return chat
			return chatLogs.at(chatID);
		}
		else
		{
			return std::vector<std::string>();
		}
	}

	std::string GetChatName(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//return chat name
			return chatBoxName.at(chatID);
		}
		else
		{
			return "";
		}
	}

	char* GetChatBufRef(int chatID)
	{
		if (!(chatLogs.find(chatID) == chatLogs.end()))
		{
			//return chat name
			return chatBoxBuf.at(chatID);
		}
		else
		{
			return nullptr;
		}
	}

	
	std::unordered_map<int, char*> chatBoxBuf;
	std::unordered_map<int, std::string> chatBoxName;
	std::unordered_map<int, bool> currentlyChatting;
	std::unordered_map<int, bool> requestsSent;
	std::unordered_map<int, bool> requestsReceived;
	std::unordered_map<int, std::vector<std::string>> chatLogs;

	private:
		unsigned int BUF_LEN = 512;
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



	ChatActivity m_chatActivity;

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
