#include "NetworkingWrapper.h"

bool NetworkingWrapper::StartupWinsock()
{
	//Initialize winsock
	WSADATA wsa;

	int error;
	error = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (error != 0) {
		printf("Failed to initialize %d\n", error);
		return false;
	}
	return true;
}

addrinfo NetworkingWrapper::SetupHints(int family, int sockType, int protocol, int flag)
{
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = family;
	hints.ai_socktype = sockType;
	hints.ai_protocol = protocol;
	hints.ai_flags = flag;

	return hints;
}

bool NetworkingWrapper::ConnectTo(std::string ip, addrinfo hints, addrinfo** ptr)
{
	if (ip == "")
	{
		if (getaddrinfo(NULL, "8888", &hints, ptr) != 0) {
			printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
			WSACleanup();
			return false;
		}
	}
	else
	{
		if (getaddrinfo(ip.c_str(), "8888", &hints, ptr) != 0) 
		{
			printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
			WSACleanup();
			return false;
		}
	}

	return true;
}

SOCKET NetworkingWrapper::CreateSocket(IPPROTO protocol)
{
	SOCKET temp = socket(AF_INET, SOCK_DGRAM, protocol);

	if (temp == INVALID_SOCKET) {
		printf("Failed creating a socket %d\n", WSAGetLastError());
		WSACleanup();
	}

	return temp;
}

bool NetworkingWrapper::BindSocket(SOCKET sock, addrinfo* ptr)
{
	if (bind(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
		printf("Bind failed: %d\n", WSAGetLastError());
		closesocket(sock);
		freeaddrinfo(ptr);
		WSACleanup();
		return false;
	}

	return true;
}

bool NetworkingWrapper::SendMsg(SOCKET sock, Convertable* message, addrinfo* ptr)
{
	//Make sure there's a conversion between T and string
	std::string converted = message->ToString();

	if (sendto(sock, converted.c_str(), (int)converted.size(), 0,
		ptr->ai_addr, ptr->ai_addrlen) == SOCKET_ERROR)
	{
		printf("Message failed to send!\n");
		return false;
	}

	return true;
}

bool NetworkingWrapper::SendMsg(SOCKET sock, Convertable* message, sockaddr_in destinationSock)
{
	//Make sure there's a conversion between T and string
	std::string converted = message->ToString();

	if (sendto(sock, converted.c_str(), (int)converted.size(), 0,
		(const sockaddr*)&destinationSock, sizeof(destinationSock)) == SOCKET_ERROR)
	{
		printf("Message failed to send!\n");
		return false;
	}

	return true;
}

sockaddr_in NetworkingWrapper::ReceiveMsg(SOCKET sock, std::string* message)
{
	// Receive msg from client
	const unsigned int BUF_LEN = 512;
	char recv_buf[BUF_LEN];

	//!!!!!REMEMBER TO SET MEMORY TO 0s!!!!!//
	memset(recv_buf, 0, BUF_LEN);

	// Struct that will hold the IP address of the client that sent the message (we don't have accept() anymore to learn the address)
	struct sockaddr_in fromAddr;
	int fromlen;
	fromlen = sizeof(fromAddr);

	if (recvfrom(sock, recv_buf, sizeof(recv_buf), 0,
		(struct sockaddr*) & fromAddr, &fromlen) == SOCKET_ERROR)
	{
		printf("recvfrom() failed...%d\n", WSAGetLastError());
	}

	//Turns recv buffer into a string
	std::string temp;
	temp += recv_buf;
	*message = temp;

	return fromAddr;
}

bool NetworkingWrapper::ShutdownSocket(SOCKET sock, addrinfo* ptr)
{
	//Shutdown the socket
	if (shutdown(sock, SD_BOTH) == SOCKET_ERROR) {
		printf("Shutdown failed!  %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return false;
	}

	CloseWinsock(sock, ptr);
	
	return true;
}


void NetworkingWrapper::CloseWinsock(SOCKET sock, addrinfo* ptr)
{
	closesocket(sock);
	freeaddrinfo(ptr);
	WSACleanup();
}
