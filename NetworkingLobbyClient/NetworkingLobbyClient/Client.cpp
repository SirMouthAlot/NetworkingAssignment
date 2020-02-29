#include "Client.h"

int IDGen::m_ID = 0;

Client::Client(sockaddr_in address)
{
	m_clientAddress = address;
}

Client::~Client()
{
	if (m_connected)
	{
		DisconnectFromServer();
	}
}

void Client::InitClient()
{
	NetworkingWrapper::StartupWinsock();
	m_hints = NetworkingWrapper::SetupHints();
}

void Client::ConnectToServer(const char* ip, const char* username)
{
	NetworkingWrapper::ConnectTo(ip, m_hints, &m_ptr);
	m_cliSock = NetworkingWrapper::CreateSocket(IPPROTO_UDP);

	//Sends a connection request
	SendMsg(MessageType::MSG_CONNECT, &String(username), MessageFlags::NONE);

	m_username = username;

	Int* clientNum;
	MessageType type;
	clientNum = (Int*)RecvMsg(type);

	//Store it
	m_clientID = clientNum->m_int;
	m_connected = true;
}

void Client::DisconnectFromServer()
{
	//Sends a disconnect request
	SendMsg(MessageType::MSG_DISCONNECT, &String("Disconnect"), MessageFlags::NONE);
}

void Client::Update()
{
	PrintActivity::Output(m_activity);
}

void Client::SendMsg(MessageType type, Convertable* message, MessageFlags flags)
{
	//Combine all the shit together into one message
	std::string mess;
	mess += Int((int)type).ToString();
	mess += " ";
	mess += message->ToString();
	mess += " ";
	mess += Int((int)flags).ToString();

	NetworkingWrapper::SendMsg(m_cliSock, &String(mess), m_ptr);
}

Convertable* Client::RecvMsg(MessageType& type)
{
	//Gets message tyoe
	std::string mess;
	NetworkingWrapper::ReceiveMsg(m_cliSock, &mess);

	int messType;
	//Store message type
	sscanf_s(mess.c_str(), "%i", &messType);

	int messFlag;

	type = MessageType(messType);

	switch (type)
	{
	case MessageType::MSG_INT:
	{
		int temp;
		sscanf_s(mess.c_str(), "%i %i", &messType, &temp);

		std::string str = std::to_string(temp);

		return new Int(temp);

		break;
	}
	case MessageType::MSG_ADDCLIENT:
	{
		//Add the data to the Other client vector
		int temp;
		char name[90];
		memset(name, 0, 90);

		sscanf_s(mess.c_str(), "%i %i %s", &messType, &temp, name, 90);

		std::string str = std::to_string(temp);
		str += " ";
		str += std::string(name);

		ClientData data;
		data.SetValue(str);

		Client newClient;
		newClient.m_clientID = data.m_userNum;
		newClient.m_username = data.m_userName;

		//Adds new client
		m_otherClients.push_back(newClient);

		m_chatActivity.AddNew(newClient.m_clientID, newClient.m_username);
		m_gameActivity.AddNew(newClient.m_clientID, newClient.m_username);

		return nullptr;

		break;
	}
	case MessageType::MSG_REMOVECLIENT:
	{
		//Remove the data from the other client vector
		int temp;
		char name[90];
		memset(name, 0, 90);

		sscanf_s(mess.c_str(), "%i %i %s", &messType, &temp, name, 90);


		std::string tempName = name;
		ClientData data = ClientData(temp, tempName);

		for (int i = 0; i < m_otherClients.size(); i++)
		{
			if (data.m_userNum == m_otherClients[i].m_clientID)
			{
				//Removes client
				m_otherClients.erase(m_otherClients.begin() + i);
				m_otherClients.shrink_to_fit();

				m_chatActivity.Remove(data.m_userNum);
			}
		}

		return nullptr;

		break;
	}
	case MessageType::MSG_CHATREQUEST:
	{
		int requesterID;

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i", &messType, &requesterID);

		//Mess mess is the ID of the client that requested the chat
		return new Int(requesterID);

		break;
	}
	case MessageType::MSG_CHATRESPONSE:
	{
		int chatID;

		int response;

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %i", &messType, &chatID, &response);

		//Mess mess2 is the actual response to the request
		return new Response(chatID, (bool)response);

		break;
	}
	case MessageType::MSG_CHATSTRING:
	{
		int chatID;
		char messMess[256];
		memset(messMess, 0, 256);

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %[^\n]s", &messType, &chatID, messMess, 256);

		return new ChatMessage(chatID, messMess);

		break;
	}
	case MessageType::MSG_GAMEREQUEST:
	{
		int requesterID;

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i", &messType, &requesterID);

		//Mess mess is the ID of the client that requested the chat
		return new Int(requesterID);

		break;
	}
	case MessageType::MSG_GAMERESPONSE:
	{
		int chatID;
		int response;

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %i", &messType, &chatID, &response);

		//Mess mess2 is the actual response to the request
		return new Response(chatID, (bool)response);

		break;
	}
	case MessageType::MSG_GAMEPOSITION:
	{
		float x, y, z;
		int chatID;


		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %f %f %f %i", &messType, &chatID, &x, &y, &z, &messFlag);


		return new GamePosition(chatID, Vector3(x, y, z));
		break;
	}
	}
}

sockaddr_in Client::GetAddress() const
{
	return m_clientAddress;
}

void Client::PrintOtherClientData()
{
	for (int i = 0; i < m_otherClients.size(); i++)
	{
		printf("Client %i: %s ", m_otherClients[i].m_clientID, m_otherClients[i].m_username.c_str());
		m_otherClients[i].Update();
		printf("\n");
	}
}

void Client::ShutdownClient()
{
	NetworkingWrapper::ShutdownSocket(m_cliSock, m_ptr);
}

int IDGen::GetNew()
{
	return m_ID++;
}
