#include "Client.h"
#include <thread>
#include <Windows.h>

Client client;
bool hasFocus = false;

void ReceiveStrings()
{
	while (true)
	{
		String temp;
		MessageType type;
		client.RecvMsg(type, &temp);
		std::cout << temp.m_string << std::endl;
	}
}

void UpdateFocus()
{
	while (true)
	{
		HWND thisWindow = GetConsoleWindow();
		HWND focusedWindow = GetForegroundWindow();

		if (thisWindow == focusedWindow)
		{
			hasFocus = true;
		}
		else
		{
			hasFocus = false;
		}
	}
}


void DisconnectAndClose()
{
	while (true)
	{
		if (hasFocus)
		{
			if (GetAsyncKeyState(VK_ESCAPE))
			{
				client.~Client();

				exit(0);
			}
		}
	}
}

int main()
{
	client.InitClient();

	std::string ip;
	std::string username;
	printf("Please write the server IP you wish to connect to: ");
	std::getline(std::cin, ip);
	printf("Please enter a username: ");
	std::getline(std::cin, username);

	client.ConnectToServer(ip.c_str(), username.c_str());

	//Receive strings now runs independantly
	std::thread t1, t2, t3;
	t1 = std::thread(ReceiveStrings);
	t2 = std::thread(UpdateFocus);
	t3 = std::thread(DisconnectAndClose);
	t1.detach();
	t2.detach();
	t3.detach();

	printf("You have connected!\n");

	while (true)
	{
		client.PrintOtherClientData();
		system("cls");
	}

	return 0;
}