#ifndef __SERVER_H__
#define __SERVER_H__

#include "Client.h"



class Server
{
public:
	Server();
	~Server();

	void InitServer();

	void SendMsg(MessageType type, Convertable* message, Client client);
	void RecvMsg();

	int FindClient(Client* client, bool& onList);
	int AddClient(Client* client);
	int GetNumConnected();

	void PrintConnectedUsers();

	void CloseServer();

private:
	SOCKET m_serverSock;
	struct addrinfo* m_ptr = NULL, m_hints;

	std::vector<Client*> m_clients;
};


#endif // !__SERVER_H__
