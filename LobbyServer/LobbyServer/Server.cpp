#include "Server.h"

Server::Server()
{
}

Server::~Server()
{
	for (int i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] != nullptr)
		{
			delete m_clients[i];
			m_clients[i] = nullptr;
		}
	}
}

void Server::InitServer()
{
	if (!NetworkingWrapper::StartupWinsock())
	{
		printf("Winsock failed to start up!");
	}

	m_hints = NetworkingWrapper::SetupHints(AF_INET, SOCK_DGRAM, IPPROTO_UDP, AI_PASSIVE);

	NetworkingWrapper::ConnectTo("", m_hints, &m_ptr);

	m_serverSock = NetworkingWrapper::CreateSocket(IPPROTO_UDP);
	NetworkingWrapper::BindSocket(m_serverSock, m_ptr);

}

void Server::SendMsg(MessageType type, Convertable* message, Client client)
{
	//Combine all the shit together into one message
	std::string mess;
	mess += Int((int)type).ToString();
	mess += " ";
	mess += message->ToString();

	//Send message first
	NetworkingWrapper::SendMsg(m_serverSock, &String(mess), client.GetAddress());
}

void Server::RecvMsg()
{
	std::string mess;

	sockaddr_in clientAddr;
	clientAddr = NetworkingWrapper::ReceiveMsg(m_serverSock, &mess);

	int messType;
	sscanf_s(mess.c_str(), "%i", &messType);

	//if client was on list
	bool onList = false;
	Client* client;
	int clientNum = -1;

	switch ((MessageType)messType)
	{
	case MessageType::MSG_CONNECT:
	{
		//Check if this client is on the list, if not add it
		client = new Client(clientAddr);

		//If client is in the list already
		FindClient(client, onList);
		if (onList)
		{
			delete client;
			client = nullptr;
		}
		else
		{
			char messMess[30];
			memset(messMess, 0, 30);

			int messFlag;
			sscanf_s(mess.c_str(), "%i %s %i", &messType, messMess, 30, &messFlag);
			std::string temp = std::string(messMess);
			temp.shrink_to_fit();

			client->m_username = temp;
		}

		break;
	}
	case MessageType::MSG_DISCONNECT:
	{
		client = new Client(clientAddr);

		//Find client on list
		clientNum = FindClient(client, onList);

		ClientData data = ClientData(m_clients[clientNum]->m_clientID, m_clients[clientNum]->m_username);

		for (int i = 0; i < m_clients.size(); i++)
		{
			if (i != clientNum)
			{
				//Tell the clients to remove this client from their list
				SendMsg(MessageType::MSG_REMOVECLIENT, &data, *m_clients[i]);
			}
		}

		if (onList)
		{
			if (m_clients[clientNum] != nullptr)
			{
				//Cleans up data
				delete m_clients[clientNum];
				m_clients[clientNum] = nullptr;
			}

			//Removes client from list
			m_clients.erase(m_clients.begin() + clientNum);
		}
		delete client;
		client = nullptr;
		break;
	}
	case MessageType::MSG_CHATREQUEST:
	{
		int idRequest;
		int idChat;
		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %i %i", &messType, &idRequest, &idChat, &messFlag);

		for (int i = 0; i < m_clients.size(); i++)
		{
			if (m_clients[i]->m_clientID == idChat)
				SendMsg(MessageType::MSG_CHATREQUEST, &Int(idRequest), *m_clients[i]);
		}



		break;
	}
	case MessageType::MSG_CHATRESPONSE:
	{
		int requesterID;
		int response;

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %i %i", &messType, &requesterID, &response, &messFlag);

		for (int i = 0; i < m_clients.size(); i++)
		{
			if (m_clients[i]->m_clientID == requesterID)
				SendMsg(MessageType::MSG_CHATRESPONSE, &Bool((bool)response), *m_clients[i]);
		}

		break;
	}
	case MessageType::MSG_CHATSTRING:
	{
		int chatID;
		char messMess[256];
		memset(messMess, 0, 256);

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %[^\n]s %i", &messType, &chatID, messMess, 256, &messFlag);


		for (int i = 0; i < m_clients.size(); i++)
		{
			if (m_clients[i]->m_clientID == chatID)
				SendMsg(MessageType::MSG_CHATSTRING, &String(messMess), *m_clients[i]);
		}

		break;
	}
	/*case MessageType::MSG_GAMEREQUEST:
	{
		char messMess[30];
		memset(messMess, 0, 30);

		int messFlag;
		sscanf_s(mess.c_str(), "%i %s %i", &messType, messMess, 30, &messFlag);

		break;
	}
	case MessageType::MSG_GAMERESPONSE:
	{
		char messMess[30];
		memset(messMess, 0, 30);

		int messFlag;
		sscanf_s(mess.c_str(), "%i %i %i %i", &messType, messMess, 30, &messFlag);



		break;
	}
	case MessageType::MSG_GAMEPOSITION:
	{
		char messMess[30];
		memset(messMess, 0, 30);

		int messFlag;
		sscanf_s(mess.c_str(), "%i %s %i", &messType, messMess, 30, &messFlag);

		break;
	}*/
	}

	if ((MessageType)messType == MessageType::MSG_CONNECT && !onList)
	{
		Client clientN = Client(clientAddr);
		int clientNumN = FindClient(&clientN, onList);

		m_clients[clientNumN]->m_clientID = IDGen::GetNew();

		//Tells client what number his client is stored at
		SendMsg(MessageType::MSG_INT, &Int(clientNumN), *m_clients[clientNumN]);

		for (int i = 0; i < m_clients.size(); i++)
		{
			if (i != clientNumN)
			{
				//Send the new client to all existing clients
				ClientData tempData = ClientData(m_clients[clientNumN]->m_clientID, m_clients[clientNumN]->m_username);
				SendMsg(MessageType::MSG_ADDCLIENT, &tempData, *m_clients[i]);

				//Send all other existing client data to new client
				ClientData otherTemp = ClientData(m_clients[i]->m_clientID, m_clients[i]->m_username);
				SendMsg(MessageType::MSG_ADDCLIENT, &otherTemp, clientN);
			}
		}
	}
}

int Server::FindClient(Client* client, bool& onList)
{
	auto it = std::find_if(m_clients.begin(), m_clients.end(), ClientCompare(client));

	if (it == m_clients.end())
	{
		onList = false;
		return AddClient(client);
	}

	//Returns the position in index
	onList = true;
	return (it - m_clients.begin());
}

int Server::AddClient(Client* client)
{
	m_clients.push_back(client);
	return m_clients.size() - 1;
}

int Server::GetNumConnected()
{
	return m_clients.size();
}

void Server::PrintConnectedUsers()
{
	for (int i = 0; i < m_clients.size(); i++)
	{
		printf("Client %i: %s ", m_clients[i]->m_clientID, m_clients[i]->m_username.c_str());
		m_clients[i]->Update();
		printf("\n");
	}
}

void Server::CloseServer()
{
	NetworkingWrapper::CloseWinsock(m_serverSock, m_ptr);
}
