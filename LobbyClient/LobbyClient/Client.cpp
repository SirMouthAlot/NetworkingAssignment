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
		ShutdownClient();
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

	Int clientNum;
	MessageType type;
	RecvMsg(type, &clientNum);

	//Store it
	m_clientID = clientNum.m_int;
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

void Client::RecvMsg(MessageType& type, Convertable* message)
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
		message->SetValue(str);

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

		break;
	}
	case MessageType::MSG_REMOVECLIENT:
	{
		//Remove the data from the other client vector
		int temp;
		char name[90];
		memset(name, 0, 90);

		sscanf_s(mess.c_str(), "%i %i %s", &messType, &temp, name, 90);

		std::string str = std::to_string(temp);
		str += " ";
		str += std::string(name);

		ClientData data;
		data.SetValue(str);

		for (int i = 0; i < m_otherClients.size(); i++)
		{
			if (data.m_userNum == m_otherClients[i].m_clientID)
			{
				//Removes client
				m_otherClients.erase(m_otherClients.begin() + i);
				m_otherClients.shrink_to_fit();
			}
		}

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
