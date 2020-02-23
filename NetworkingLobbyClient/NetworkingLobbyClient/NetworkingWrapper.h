#ifndef __NETWORKINGWRAPPER_H__
#define __NETWORKINGWRAPPER_H__

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include "Number.h"
#include "String.h"
#include "Vector.h"
#include "ClientData.h"
#include "ChatData.h"

#pragma comment(lib, "Ws2_32.lib")

class NetworkingWrapper abstract
{
public:
	static bool StartupWinsock();
	static addrinfo SetupHints(int family = AF_INET, int sockType = SOCK_DGRAM, int protocol = IPPROTO_UDP, int flag=NULL);
	static bool ConnectTo(std::string ip, addrinfo hints, addrinfo** ptr);
	static SOCKET CreateSocket(IPPROTO protocol=IPPROTO_UDP);
	static bool BindSocket(SOCKET sock, addrinfo* ptr);

	static bool SendMsg(SOCKET sock, Convertable* message, addrinfo* ptr);
	static bool SendMsg(SOCKET sock, Convertable* message, sockaddr_in destinationSock);
	static sockaddr_in ReceiveMsg(SOCKET sock, std::string* message);

	static bool ShutdownSocket(SOCKET sock, addrinfo* ptr);
	static void CloseWinsock(SOCKET sock, addrinfo* ptr);
};

#endif


